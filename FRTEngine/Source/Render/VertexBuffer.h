#pragma once
#include "Render/GraphicsResource.h"
#include "Tools/DXHelper.h"

#include "Utils/Logger/Logger.h"
#include <utility>


namespace frt
{
class FRTENGINE_API VertexBuffer : public GraphicsResource
{
public:
    VertexBuffer() = delete;
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&&) = delete;

    template<typename V>
    VertexBuffer(Graphics* owner, V* vertices, UINT verticesNum, unsigned bufferSlot)
        : GraphicsResource(owner), _vertexBufferView{}, _vertexBufferSlot{bufferSlot}
    {
        const UINT vertexSize = sizeof(V);
        const UINT vertexBufferSize = verticesNum * vertexSize;

        // Note: using upload heaps to transfer static data like vert buffers is not 
        // recommended. Every time the GPU needs it, the upload heap will be marshalled 
        // over. Please read up on Default Heap usage. An upload heap is used here for 
        // code simplicity and because there are very few verts to actually transfer.
        CD3DX12_HEAP_PROPERTIES heapPropsDefault(D3D12_HEAP_TYPE_DEFAULT);
        CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
        THROW_IF_FAILED(GetDevice()->CreateCommittedResource(
            &heapPropsDefault,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&_vertexBuffer)));

        CD3DX12_HEAP_PROPERTIES heapPropsUpload(D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC descUpload = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
        THROW_IF_FAILED(GetDevice()->CreateCommittedResource(
            &heapPropsUpload,
            D3D12_HEAP_FLAG_NONE,
            &descUpload,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&_vertexBufferUploadHeap)));

        NAME_D3D12_OBJECT(_vertexBuffer);


        // Copy data to the intermediate upload heap and then schedule a copy
        // from the upload heap to the vertex buffer.
        D3D12_SUBRESOURCE_DATA vertexData = {};
        vertexData.pData = vertices;
        vertexData.RowPitch = vertexBufferSize;
        vertexData.SlicePitch = vertexData.RowPitch;

        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(_vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
        UpdateSubresources<1>(GetCommandList(), _vertexBuffer.Get(), _vertexBufferUploadHeap.Get(), 0, 0, 1, &vertexData);
        GetCommandList()->ResourceBarrier(1, &barrier);

         //Initialize the vertex buffer view.
        _vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
        _vertexBufferView.StrideInBytes = vertexSize;
        _vertexBufferView.SizeInBytes = vertexBufferSize;
    }

    virtual void Update() override;
    inline virtual void PopulateCommandList() override
    {
        GetCommandList()->IASetVertexBuffers(_vertexBufferSlot, 1, &_vertexBufferView);
    }

protected:
    ComPtr<ID3D12Resource> _vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;
    ComPtr<ID3D12Resource> _vertexBufferUploadHeap;
    unsigned _vertexBufferSlot;
};
}

