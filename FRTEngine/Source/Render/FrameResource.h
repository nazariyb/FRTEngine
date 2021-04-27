#pragma once
#include "FRTEngine.h"
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>


namespace frt
{

using Microsoft::WRL::ComPtr;

class FRTENGINE_API FrameResource
{
public:
    //FrameResource(ID3D12Device* device, UINT rowCount, UINT columnCount, UINT materialCount, float spacingInterval);
    ~FrameResource();

    //void InitBundle(ID3D12Device* device, ID3D12PipelineState* pipelineState,
    //                UINT frameResourceIndex, UINT numIndices, D3D12_INDEX_BUFFER_VIEW* indexBufferViewDesc, D3D12_VERTEX_BUFFER_VIEW* vertexBufferViewDesc,
    //                ID3D12DescriptorHeap* cbvSrvDescriptorHeap, UINT cbvSrvDescriptorSize, ID3D12DescriptorHeap* samplerDescriptorHeap, ID3D12RootSignature* rootSignature);

    //void PopulateCommandList(ID3D12GraphicsCommandList* commandList,
    //                         UINT frameResourceIndex, UINT numIndices, D3D12_INDEX_BUFFER_VIEW* indexBufferViewDesc, D3D12_VERTEX_BUFFER_VIEW* vertexBufferViewDesc,
    //                         ID3D12DescriptorHeap* cbvSrvDescriptorHeap, UINT cbvSrvDescriptorSize, ID3D12DescriptorHeap* samplerDescriptorHeap, ID3D12RootSignature* rootSignature);

    void XM_CALLCONV UpdateConstantBuffers(DirectX::FXMMATRIX view, DirectX::CXMMATRIX projection, class Camera* camera);

    //void RotateCube(UINT Index, float Roll, float Pitch, float Yaw);

public:
    struct SceneConstantBuffer
    {
        DirectX::XMFLOAT4X4 mvp;   // Model-view-projection
        DirectX::XMFLOAT4X4 modelView;
        DirectX::XMFLOAT4 lightPosition;
        DirectX::XMFLOAT4 diffuseColor;
        DirectX::XMFLOAT4 ambient;
        DirectX::XMFLOAT4 padding1;
        FLOAT diffuseIntensity;
        FLOAT attenuationConst;
        FLOAT attenuationLinear;
        FLOAT attenuationQuad;
        FLOAT specularIntesity;
        FLOAT specularPower;
        FLOAT padding2[10];
    };
    //static_assert((sizeof(SceneConstantBuffer) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

    //struct ObjectConstantBuffer
    //{
    //    FLOAT specularIntesity;
    //    FLOAT specularPower;
    //    FLOAT padding[62];
    //};
    //static_assert((sizeof(ObjectConstantBuffer) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

    //ComPtr<ID3D12CommandAllocator> _commandAllocator;
    //ComPtr<ID3D12CommandAllocator> _bundleAllocator;
    //ComPtr<ID3D12GraphicsCommandList> _bundle;
    
    SceneConstantBuffer* _constantBuffers;
    ComPtr<ID3D12Resource> _cbvUploadHeap;
    
    //ObjectConstantBuffer* _constantBuffersForObjects;
    //ComPtr<ID3D12Resource> _cbvUploadHeapForObjects;

    //UINT64 _fenceValue;
    //
    std::vector<DirectX::XMFLOAT4X4> _modelMatrices;
    UINT _rowCount;
    UINT _columnCount;
    //UINT _materialCount;
    float _spacingInterval;

private:
    //void SetPositions(FLOAT intervalX, FLOAT intervalZ);

};
}

