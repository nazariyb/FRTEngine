#include "IndexBuffer.h"

#include "Exception.h"

namespace frt
{
IndexBuffer::IndexBuffer(Graphics* owner, UINT8* indices, UINT indicesNum)
    : GraphicsResource(owner), _indexBufferView{}
{
    const UINT indexBufferSize = indicesNum;

    CD3DX12_HEAP_PROPERTIES heapPropsDefault(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC descDefault = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
    THROW_IF_FAILED(GetDevice()->CreateCommittedResource(
        &heapPropsDefault,
        D3D12_HEAP_FLAG_NONE,
        &descDefault,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&_indexBuffer)));

    CD3DX12_HEAP_PROPERTIES heapPropsUpload(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC descUpload = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
    THROW_IF_FAILED(GetDevice()->CreateCommittedResource(
        &heapPropsUpload,
        D3D12_HEAP_FLAG_NONE,
        &descUpload,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&_indexBufferUploadHeap)));

    NAME_D3D12_OBJECT(_indexBuffer);

    // Copy data to the intermediate upload heap and then schedule a copy 
    // from the upload heap to the index buffer.
    D3D12_SUBRESOURCE_DATA indexData = {};
    indexData.pData = indices;
    indexData.RowPitch = indexBufferSize;
    indexData.SlicePitch = indexData.RowPitch;

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(_indexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
    UpdateSubresources<1>(GetCommandList(), _indexBuffer.Get(), _indexBufferUploadHeap.Get(), 0, 0, 1, &indexData);
    GetCommandList()->ResourceBarrier(1, &barrier);

    // Initialize the vertex buffer view.
    _indexBufferView.BufferLocation = _indexBuffer->GetGPUVirtualAddress();
    _indexBufferView.Format = DXGI_FORMAT_R8_UINT;
    _indexBufferView.SizeInBytes = indexBufferSize;
}

void IndexBuffer::Update()
{}

}
