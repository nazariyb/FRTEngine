#include "FrameResource.h"
#include "Exception.h"
#include "App.h"

#include "Tools/d3dx12.h"
#include "Tools/DXHelper.h"
#include "Camera.h"

namespace frt
{

FrameResource::FrameResource(ID3D12Device* device, UINT rowCount, UINT columnCount, UINT materialCount, float spacingInterval)
    : _fenceValue(0)
    , _rowCount(rowCount)
    , _columnCount(columnCount)
    , _materialCount(materialCount)
    , _spacingInterval(spacingInterval)
{
    _modelMatrices.resize(_rowCount * _columnCount);

    // The command allocator is used by the main sample class when 
    // resetting the command list in the main update loop. Each frame 
    // resource needs a command allocator because command allocators 
    // cannot be reused until the GPU is done executing the commands 
    // associated with it.
    THROW_IF_FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocator)));
    THROW_IF_FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&_bundleAllocator)));

    // Create an upload heap for the constant buffers.
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    auto desc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(SceneConstantBuffer) * _rowCount * _columnCount);
    THROW_IF_FAILED(device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&_cbvUploadHeap)));

    // Map the constant buffers. Note that unlike D3D11, the resource 
    // does not need to be unmapped for use by the GPU. In this sample, 
    // the resource stays 'permenantly' mapped to avoid overhead with 
    // mapping/unmapping each frame.
    CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
    THROW_IF_FAILED(_cbvUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&_constantBuffers)));

    // Update all of the model matrices once; our cities don't move so 
    // we don't need to do this ever again.
    SetPositions(spacingInterval, -spacingInterval);
}

FrameResource::~FrameResource()
{
    _cbvUploadHeap->Unmap(0, nullptr);
    _constantBuffers = nullptr;

    //_constantBuffersForObjects = nullptr;
    //_cbvUploadHeapForObjects->Unmap(0, nullptr);
}

void FrameResource::InitBundle(ID3D12Device* device, ID3D12PipelineState* pipelineState,
                               UINT frameResourceIndex, UINT numIndices, D3D12_INDEX_BUFFER_VIEW* indexBufferViewDesc,
                               D3D12_VERTEX_BUFFER_VIEW* vertexBufferViewDesc, ID3D12DescriptorHeap* cbvSrvDescriptorHeap,
                               UINT cbvSrvDescriptorSize, ID3D12DescriptorHeap* samplerDescriptorHeap, ID3D12RootSignature* rootSignature)
{
    THROW_IF_FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_BUNDLE, _bundleAllocator.Get(), pipelineState, IID_PPV_ARGS(&_bundle)));
    NAME_D3D12_OBJECT(_bundle);

    PopulateCommandList(_bundle.Get(), frameResourceIndex, numIndices, indexBufferViewDesc,
                        vertexBufferViewDesc, cbvSrvDescriptorHeap, cbvSrvDescriptorSize, samplerDescriptorHeap, rootSignature);

    THROW_IF_FAILED(_bundle->Close());
}

void FrameResource::PopulateCommandList(ID3D12GraphicsCommandList* commandList,
                                        UINT frameResourceIndex, UINT numIndices, D3D12_INDEX_BUFFER_VIEW* indexBufferViewDesc,
                                        D3D12_VERTEX_BUFFER_VIEW* vertexBufferViewDesc, ID3D12DescriptorHeap* cbvSrvDescriptorHeap,
                                        UINT cbvSrvDescriptorSize, ID3D12DescriptorHeap* samplerDescriptorHeap, ID3D12RootSignature* rootSignature)
{
    // If the root signature matches the root signature of the caller, then
    // bindings are inherited, otherwise the bind space is reset.
    commandList->SetGraphicsRootSignature(rootSignature);

    ID3D12DescriptorHeap* ppHeaps[] = { cbvSrvDescriptorHeap, samplerDescriptorHeap };
    commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList->IASetIndexBuffer(indexBufferViewDesc);
    commandList->IASetVertexBuffers(0, 1, vertexBufferViewDesc);
    commandList->SetGraphicsRootDescriptorTable(0, cbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
    commandList->SetGraphicsRootDescriptorTable(1, samplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

    // Calculate the descriptor offset due to multiple frame resources.
    // (_materialCount + 1) SRVs + how many CBVs we have currently.
    UINT frameResourceDescriptorOffset = (_materialCount + 1) + (frameResourceIndex * _rowCount * _columnCount);
    CD3DX12_GPU_DESCRIPTOR_HANDLE cbvSrvHandle(cbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), frameResourceDescriptorOffset, cbvSrvDescriptorSize);

    for (UINT i = 0; i < _rowCount; i++)
    {
        for (UINT j = 0; j < _columnCount; j++)
        {
            // Set the cube's root constant for dynamically indexing into the material array.
            commandList->SetGraphicsRoot32BitConstant(3, (i * _columnCount) + j, 0);

            // Set this cube's CBV table and move to the next descriptor.
            commandList->SetGraphicsRootDescriptorTable(2, cbvSrvHandle);
            cbvSrvHandle.Offset(cbvSrvDescriptorSize);

            commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
        }
    }
}

void XM_CALLCONV FrameResource::UpdateConstantBuffers(DirectX::FXMMATRIX view, DirectX::CXMMATRIX projection, class Camera* camera)
{
    const std::vector<DirectX::XMFLOAT4X4>& meshes = App::GetInstance()->GetWorld()->GetMeshes();

    float x, z;
    for (UINT i = 0; i < _rowCount; i++)
    {
        FLOAT offsetZ = i * -_spacingInterval;
        for (UINT j = 0; j < _columnCount; j++)
        {
            FLOAT offsetX = j * _spacingInterval;
            int index = i * _columnCount + j;

            x = offsetX;
            z = offsetZ;

            if (index == 0)
                DirectX::XMStoreFloat4x4(&_modelMatrices[index],
                                         DirectX::XMMatrixMultiply(DirectX::XMMatrixTranslation(-.5f, 0.f, 0.5f),
                                                                   DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&meshes[index]),
                                                                                             DirectX::XMMatrixTranslation(offsetX + 0.5f, 0.f, offsetZ - 0.5f))));
            else if (index == 1)
                DirectX::XMStoreFloat4x4(&_modelMatrices[index],
                                         DirectX::XMMatrixMultiply(DirectX::XMMatrixTranslation(0.5f, 0.f, 0.5f),
                                                                   DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&meshes[0]),
                                                                                             DirectX::XMMatrixTranslation(offsetX - 0.5f, 0.f, offsetZ - 0.5f))));
            else if (index == 4)
                DirectX::XMStoreFloat4x4(&_modelMatrices[index],
                                         DirectX::XMMatrixMultiply(DirectX::XMMatrixTranslation(-.5f, 0.f, -.5f),
                                                                   DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&meshes[0]),
                                                                                             DirectX::XMMatrixTranslation(offsetX + 0.5f, 0.f, offsetZ + 0.5f))));
            else if (index == 5)
                DirectX::XMStoreFloat4x4(&_modelMatrices[index],
                                         DirectX::XMMatrixMultiply(DirectX::XMMatrixTranslation(0.5f, 0.f, -.5f),
                                                                   DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&meshes[0]),
                                                                                             DirectX::XMMatrixTranslation(offsetX - 0.5f, 0.f, offsetZ + 0.5f))));
            else
                DirectX::XMStoreFloat4x4(&_modelMatrices[index],
                                         DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&meshes[index]),
                                                                   DirectX::XMMatrixTranslation(offsetX, 0.f, offsetZ)));

        }
    }

    DirectX::XMMATRIX model;
    DirectX::XMFLOAT4X4 mvp;

    for (UINT i = 0; i < _rowCount; i++)
    {
        for (UINT j = 0; j < _columnCount; j++)
        {
            const UINT index = i * _columnCount + j;
            model = DirectX::XMLoadFloat4x4(&_modelMatrices[index]);

            // Compute the model-view-projection matrix.
            XMStoreFloat4x4(&mvp, XMMatrixTranspose(model * view * projection));
            
            // Copy this matrix into the appropriate location in the upload heap subresource.
            memcpy(&_constantBuffers[index], &mvp, sizeof(mvp));

            DirectX::XMStoreFloat4x4(&_constantBuffers[index].modelView, XMMatrixTranspose(model * view));
            DirectX::XMStoreFloat4(&_constantBuffers[index].lightPosition, DirectX::XMVector3Transform(DirectX::XMVectorSet(-2.f, 2.f, 0.f, 0.f), view));
            DirectX::XMStoreFloat4(&_constantBuffers[index].diffuseColor,  DirectX::XMVectorSet(1.f, 1.f, 1.f, 1.f));
            DirectX::XMStoreFloat4(&_constantBuffers[index].ambient, DirectX::XMVectorSet(0.15f, 0.15f, 0.15f, 1.0f));
            _constantBuffers[index].diffuseIntensity = 1.0f;
            _constantBuffers[index].attenuationConst = 1.0f;
            _constantBuffers[index].attenuationLinear = 0.045;
            _constantBuffers[index].attenuationQuad = 0.0075f;
            _constantBuffers[index].specularIntesity = 1.f;
            _constantBuffers[index].specularPower = 30.f;
        }
    }
}

void FrameResource::RotateCube(UINT Index, float Roll, float Pitch, float Yaw)
{
    for (UINT i = 0; i < _rowCount; i++)
    {
        FLOAT offsetZ = i * -_spacingInterval;
        for (UINT j = 0; j < _columnCount; j++)
        {
            FLOAT offsetX = j * _spacingInterval;

            int index = i * _columnCount + j;
            if (index != Index)
            {
                // The y position is based off of the city's row and column 
                // position to prevent z-fighting.
                DirectX::XMStoreFloat4x4(&_modelMatrices[index],
                                         DirectX::XMMatrixTranslation(offsetX, 0.f, offsetZ));
            }
            else
            {
                DirectX::XMStoreFloat4x4(&_modelMatrices[index],
                                         DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll),
                                                                   DirectX::XMMatrixTranslation(offsetX, 0.f, offsetZ)));

            }
        }
    }
    //DirectX::XMStoreFloat4x4(&_modelMatrices[Index],
    //                         DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&_modelMatrices[Index]), DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll)));
}

void FrameResource::SetPositions(FLOAT intervalX, FLOAT intervalZ)
{
    for (UINT i = 0; i < _rowCount; i++)
    {
        FLOAT offsetZ = i * intervalZ;
        for (UINT j = 0; j < _columnCount; j++)
        {
            FLOAT offsetX = j * intervalX;

            // The y position is based off of the city's row and column 
            // position to prevent z-fighting.
            DirectX::XMStoreFloat4x4(&_modelMatrices[i * _columnCount + j],
                                     DirectX::XMMatrixTranslation(offsetX, 0.f, offsetZ));
        }
    }
}

}