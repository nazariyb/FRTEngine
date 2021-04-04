#include "Mesh.h"
#include <Tools\d3dx12.h>
#include <Exception.h>

namespace frt
{

Mesh::Mesh(float radius, DirectX::XMFLOAT3 initialPosition)
    : _radius(radius), _initialPosition(initialPosition), _texture{}
{
    const float x = initialPosition.x;
    const float y = initialPosition.x;
    const float z = initialPosition.x;

    // front side
    _vertices[ 0] = { { x - radius, y - radius, z - radius }, { 0.0f, 0.0f } };  //  ∟
    _vertices[ 1] = { { x - radius, y + radius, z - radius }, { 0.0f, 1.0f } };  //  Γ
    _vertices[ 2] = { { x + radius, y - radius, z - radius }, { 1.0f, 0.0f } };  //  ┘
    _vertices[ 3] = { { x + radius, y + radius, z - radius }, { 1.0f, 1.0f } };  //  ┐
    
    // right side
    _vertices[ 4] = { { x + radius, y - radius, z - radius }, { 0.0f, 0.0f } };  //  ∟
    _vertices[ 5] = { { x + radius, y + radius, z - radius }, { 0.0f, 1.0f } };  //  Γ
    _vertices[ 6] = { { x + radius, y - radius, z + radius }, { 1.0f, 0.0f } };  //  ┘
    _vertices[ 7] = { { x + radius, y + radius, z + radius }, { 1.0f, 1.0f } };  //  ┐
    
    // left side
    _vertices[ 8] = { { x - radius, y - radius, z + radius }, { 0.0f, 0.0f } };  //  ∟
    _vertices[ 9] = { { x - radius, y + radius, z + radius }, { 0.0f, 1.0f } };  //  Γ
    _vertices[10] = { { x - radius, y - radius, z - radius }, { 1.0f, 0.0f } };  //  ┘
    _vertices[11] = { { x - radius, y + radius, z - radius }, { 1.0f, 1.0f } };  //  ┐

    // back side
    _vertices[12] = { { x + radius, y - radius, z + radius }, { 0.0f, 0.0f } };  //  ∟
    _vertices[13] = { { x + radius, y + radius, z + radius }, { 0.0f, 1.0f } };  //  Γ
    _vertices[14] = { { x - radius, y - radius, z + radius }, { 1.0f, 0.0f } };  //  ┘
    _vertices[15] = { { x - radius, y + radius, z + radius }, { 1.0f, 1.0f } };  //  ┐

    // top side
    _vertices[16] = { { x - radius, y + radius, z - radius }, { 0.0f, 0.0f } };  //  ∟
    _vertices[17] = { { x - radius, y + radius, z + radius }, { 0.0f, 1.0f } };  //  Γ
    _vertices[18] = { { x + radius, y + radius, z - radius }, { 1.0f, 0.0f } };  //  ┘
    _vertices[19] = { { x + radius, y + radius, z + radius }, { 1.0f, 1.0f } };  //  ┐

    // bottom side
    _vertices[20] = { { x + radius, y - radius, z - radius }, { 0.0f, 0.0f } };  //  ∟
    _vertices[21] = { { x + radius, y - radius, z + radius }, { 0.0f, 1.0f } };  //  Γ
    _vertices[22] = { { x - radius, y - radius, z - radius }, { 1.0f, 0.0f } };  //  ┘
    _vertices[23] = { { x - radius, y - radius, z + radius }, { 1.0f, 1.0f } };  //  ┐

}

void Mesh::Update(ComPtr<ID3D12Device> device, D3D12_VERTEX_BUFFER_VIEW* vertexBufferView, D3D12_INDEX_BUFFER_VIEW* indexBufferView)
{
    // Note: using upload heaps to transfer static data like vert buffers is not 
    // recommended. Every time the GPU needs it, the upload heap will be marshalled 
    // over. Please read up on Default Heap usage. An upload heap is used here for 
    // code simplicity and because there are very few verts to actually transfer.
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    auto desc = CD3DX12_RESOURCE_DESC::Buffer(_vertexBufferSize);
    THROW_IF_FAILED(device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&_vertexBuffer)));

    // Copy the triangle data to the vertex buffer.
    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
    THROW_IF_FAILED(_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
    memcpy(pVertexDataBegin, _vertices, sizeof(_vertices));
    _vertexBuffer->Unmap(0, nullptr);

    // Initialize the vertex buffer view.
    vertexBufferView->BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
    vertexBufferView->StrideInBytes = sizeof(Vertex);
    vertexBufferView->SizeInBytes = _vertexBufferSize;

    // index buffer
    CD3DX12_HEAP_PROPERTIES heapProps_(D3D12_HEAP_TYPE_UPLOAD);
    auto desc_ = CD3DX12_RESOURCE_DESC::Buffer(_indexBufferSize);
    THROW_IF_FAILED(device->CreateCommittedResource(
        &heapProps_,
        D3D12_HEAP_FLAG_NONE,
        &desc_,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&_indexBuffer)));

    // Copy the triangle data to the vertex buffer.
    UINT8* pIndexDataBegin;
    CD3DX12_RANGE readRangeIndex(0, 0);        // We do not intend to read from this resource on the CPU.
    THROW_IF_FAILED(_indexBuffer->Map(0, &readRangeIndex, reinterpret_cast<void**>(&pIndexDataBegin)));
    memcpy(pIndexDataBegin, &_indices[0], sizeof(_indices));
    _indexBuffer->Unmap(0, nullptr);

    // Initialize the vertex buffer view.
    indexBufferView->BufferLocation = _indexBuffer->GetGPUVirtualAddress();
    indexBufferView->SizeInBytes = _indexBufferSize;
}

void Mesh::InitializeTextureData(uint32_t color1, uint32_t color2)
{
    const UINT rowPitch = TextureWidth * TexturePixelSize;
    const UINT cellPitch = rowPitch >> 3;        // The width of a cell in the checkboard texture.
    const UINT cellHeight = TextureWidth >> 3;    // The height of a cell in the checkerboard texture.
    const UINT textureSize = rowPitch * TextureHeight;

    _texture.resize(textureSize);
    UINT8* pData = &_texture[0];

    for (UINT n = 0; n < textureSize; n += TexturePixelSize)
    {
        UINT x = n % rowPitch;
        UINT y = n / rowPitch;
        UINT i = x / cellPitch;
        UINT j = y / cellHeight;

        if (i % 2 == j % 2)
        {
            pData[n + 0] = (color1 >> (8 * 3)) & 0x0000ff;    // R
            pData[n + 1] = (color1 >> (8 * 2)) & 0x0000ff;    // G
            pData[n + 2] = (color1 >> (8 * 1)) & 0x0000ff;    // B
            pData[n + 3] = (color1 >> (8 * 0)) & 0x0000ff;    // A
        }
        else
        {
            pData[n + 0] = (color2 >> (8 * 3)) & 0x0000ff;    // R
            pData[n + 1] = (color2 >> (8 * 2)) & 0x0000ff;    // G
            pData[n + 2] = (color2 >> (8 * 1)) & 0x0000ff;    // B
            pData[n + 3] = (color2 >> (8 * 0)) & 0x0000ff;    // A
        }
    }
}

}
