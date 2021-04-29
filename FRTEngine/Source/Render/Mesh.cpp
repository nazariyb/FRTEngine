#include "Mesh.h"
#include <Tools\d3dx12.h>
#include <Exception.h>
#include "Utils\Logger\Logger.h"
#include "Render\IndexBuffer.h"
#include "Render\VertexBuffer.h"


namespace frt
{

const std::vector<unsigned char> Mesh::_indices =
{
    // front
    0, 1, 2,
    2, 1, 3,

    // right
    4, 5, 6,
    6, 5, 7,

    // left
    8, 9, 10,
    10, 9, 11,

    // back
    12, 13, 14,
    14, 13, 15,

    // top
    16, 17, 18,
    18, 17, 19,

    // bottom
    20, 21, 22,
    22, 21, 23,
};

const unsigned int Mesh::_indexBufferSize = Mesh::_indices.size();

Mesh::Mesh(float radius, DirectX::XMFLOAT3 initialPosition)
    : _radius(radius)
    , _initialPosition(initialPosition)
    , _texture{}
    , _indexBuffer{}
    , _vertexBuffer{}
{
    Resize(_radius);
}

Mesh::Mesh(float radius)
    : Mesh(1.f, {})
{}

Mesh::Mesh()
    : Mesh(1.f)
{}

Mesh::~Mesh()
{
    delete _vertexBuffer;
    delete _indexBuffer;

    for (size_t i = 0; i < _constantBuffers.size(); ++i)
    {
        delete _constantBuffers.at(i);
    }
}

void Mesh::Resize(float newRadius)
{
    const float x = _initialPosition.x;
    const float y = _initialPosition.x;
    const float z = _initialPosition.x;

    // front side
    _vertices[0] = { { x + newRadius, y - newRadius, z - newRadius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };  //  ∟
    _vertices[1] = { { x + newRadius, y + newRadius, z - newRadius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };  //  Γ
    _vertices[2] = { { x - newRadius, y - newRadius, z - newRadius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };  //  ┘
    _vertices[3] = { { x - newRadius, y + newRadius, z - newRadius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };  //  ┐

    // right side
    _vertices[4] = { { x + newRadius, y + newRadius, z - newRadius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };  //  ∟
    _vertices[5] = { { x + newRadius, y - newRadius, z - newRadius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };  //  Γ
    _vertices[6] = { { x + newRadius, y + newRadius, z + newRadius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };  //  ┘
    _vertices[7] = { { x + newRadius, y - newRadius, z + newRadius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };  //  ┐

    // left side
    _vertices[8] = { { x - newRadius, y + newRadius, z + newRadius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };  //  ∟
    _vertices[9] = { { x - newRadius, y - newRadius, z + newRadius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };  //  Γ
    _vertices[10] = { { x - newRadius, y + newRadius, z - newRadius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };  //  ┘
    _vertices[11] = { { x - newRadius, y - newRadius, z - newRadius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };  //  ┐

    // back side
    _vertices[12] = { { x - newRadius, y - newRadius, z + newRadius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };  //  ∟
    _vertices[13] = { { x - newRadius, y + newRadius, z + newRadius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };  //  Γ
    _vertices[14] = { { x + newRadius, y - newRadius, z + newRadius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };  //  ┘
    _vertices[15] = { { x + newRadius, y + newRadius, z + newRadius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };  //  ┐

    // top side
    _vertices[16] = { { x + newRadius, y + newRadius, z - newRadius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };  //  ∟
    _vertices[17] = { { x + newRadius, y + newRadius, z + newRadius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };  //  Γ
    _vertices[18] = { { x - newRadius, y + newRadius, z - newRadius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };  //  ┘
    _vertices[19] = { { x - newRadius, y + newRadius, z + newRadius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };  //  ┐

    // bottom side
    _vertices[20] = { { x - newRadius, y - newRadius, z - newRadius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };  //  ∟
    _vertices[21] = { { x - newRadius, y - newRadius, z + newRadius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };  //  Γ
    _vertices[22] = { { x + newRadius, y - newRadius, z - newRadius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };  //  ┘
    _vertices[23] = { { x + newRadius, y - newRadius, z + newRadius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };  //  ┐

    using namespace DirectX;

    for (size_t i = 0; i < _indices.size(); i += 3)
    {
        Vertex& v0 = _vertices[_indices[i]];
        Vertex& v1 = _vertices[_indices[i + 1]];
        Vertex& v2 = _vertices[_indices[i + 2]];
        const XMVECTOR pos0 = XMLoadFloat3(&v0.position);
        const XMVECTOR pos1 = XMLoadFloat3(&v1.position);
        const XMVECTOR pos2 = XMLoadFloat3(&v2.position);

        const auto normal = XMVector3Normalize(XMVector3Cross((pos2 - pos0), (pos1 - pos0)));

        XMStoreFloat3(&v0.normal, normal);
        XMStoreFloat3(&v1.normal, normal);
        XMStoreFloat3(&v2.normal, normal);
    }
}

void Mesh::InitializeConstantBuffers(Graphics* graphics)
{
    for (UINT i = 0; i < graphics->FrameCount; ++i)
    {
        _constantBuffers.push_back(new ConstantBuffer<SceneObjectConstantBuffer>(graphics, {}));
    }
}

void Mesh::UpdateConstantBuffer(const SceneObjectConstantBuffer& newBuffer)
{
    for (ConstantBuffer<SceneObjectConstantBuffer>* constantBuffer : _constantBuffers)
    {
        constantBuffer->Update(newBuffer);
    }
}

void Mesh::Update()
{

}

void Mesh::PopulateCommandList()
{
    _vertexBuffer->PopulateCommandList();
    _indexBuffer->PopulateCommandList();
    
    for (ConstantBuffer<SceneObjectConstantBuffer>* constantBuffer : _constantBuffers)
    {
        constantBuffer->PopulateCommandList();
    }
}

void Mesh::InitializeGraphicsResources(Graphics* graphics)
{
    _vertexBuffer = new VertexBuffer(graphics, GetVertices(), GetVertexDataSize());
    _indexBuffer = new IndexBuffer(graphics, GetIndices(), GetIndexDataSize());
}

Mesh::Vertex* Mesh::GetVertices()
{
    return _vertices;
}

UINT8* Mesh::GetIndices()
{
    return const_cast<UINT8*>(&_indices[0]);
}

std::vector<UINT8> Mesh::GenerateTextureData(uint32_t color1, uint32_t color2)
{
    const UINT rowPitch = TextureWidth * TexturePixelSize;
    const UINT cellPitch = rowPitch >> 3;        // The width of a cell in the checkboard texture.
    const UINT cellHeight = TextureWidth >> 3;    // The height of a cell in the checkerboard texture.
    const UINT textureSize = rowPitch * TextureHeight;

    std::vector<UINT8> data(textureSize);
    UINT8* pData = &data[0];

    for (UINT n = 0; n < textureSize; n += TexturePixelSize)
    {
        UINT x = n % rowPitch;
        UINT y = n / rowPitch;
        UINT i = x / cellPitch;
        UINT j = y / cellHeight;

        if (i % 2 == j % 2)
        {
            pData[n + 0] = (color1 >> (8 * 3)) & 0x000000ff;    // R
            pData[n + 1] = (color1 >> (8 * 2)) & 0x000000ff;    // G
            pData[n + 2] = (color1 >> (8 * 1)) & 0x000000ff;    // B
            pData[n + 3] = (color1 >> (8 * 0)) & 0x000000ff;    // A
        }
        else
        {
            pData[n + 0] = (color2 >> (8 * 3)) & 0x000000ff;    // R
            pData[n + 1] = (color2 >> (8 * 2)) & 0x000000ff;    // G
            pData[n + 2] = (color2 >> (8 * 1)) & 0x000000ff;    // B
            pData[n + 3] = (color2 >> (8 * 0)) & 0x000000ff;    // A
        }
    }
    return data;
}

}
