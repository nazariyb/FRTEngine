#pragma once
#include "FRTEngine.h"
#include "Render/Graphics.h"
#include "Render/GraphicsResource.h"
#include "Exception.h"


namespace frt
{
template<typename C>
class FRTENGINE_API ConstantBuffer : public GraphicsResource
{
public:
    ConstantBuffer(Graphics* owner, C buffer)
        : GraphicsResource(owner)
    {
        const UINT bufferSize = sizeof(C);

        // Create an upload heap for the constant buffers.
        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
        THROW_IF_FAILED(GetDevice()->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&_cbvUploadHeap)));

        // Map the constant buffers. Note that unlike D3D11, the resource 
        // does not need to be unmapped for use by the GPU. In this sample, 
        // the resource stays 'permanently' mapped to avoid overhead with 
        // mapping/unmapping each frame.
        CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
        THROW_IF_FAILED(_cbvUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&_buffer)));

        memcpy(&_buffer[0], &buffer, bufferSize);

        const UINT buffersNum = PostIncConstBuffersNum();

        CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(GetCbvSrvHeap()->GetCPUDescriptorHandleForHeapStart(),
                                                   (_owner->MaterialCount + 1) + buffersNum,
                                                   GetCbvSrvDescriptorSize());    // Move past the SRVs.

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
        cbvDesc.BufferLocation = _cbvUploadHeap->GetGPUVirtualAddress();
        cbvDesc.SizeInBytes = bufferSize;
        GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
    }

    virtual void Update() override {}
    virtual void PopulateCommandList() override {}

    void Update(C buffer)
    {
        memcpy(&_buffer[0], &buffer, sizeof(C));
    }

protected:
    C* _buffer;
    ComPtr<ID3D12Resource> _cbvUploadHeap;
};
}

