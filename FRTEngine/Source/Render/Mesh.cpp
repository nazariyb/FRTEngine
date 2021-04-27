#include "Mesh.h"
#include <Tools\d3dx12.h>
#include <Exception.h>
#include "Utils\Logger\Logger.h"
#include "Render\IndexBuffer.h"
#include "Render\VertexBuffer.h"


namespace frt
{

Mesh::Mesh(float radius, DirectX::XMFLOAT3 initialPosition)
    : _radius(radius), _initialPosition(initialPosition), _texture{}
{
    const float x = initialPosition.x;
    const float y = initialPosition.x;
    const float z = initialPosition.x;

    // front side
    _vertices[ 0] = { { x + radius, y - radius, z - radius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };  //  ∟
    _vertices[ 1] = { { x + radius, y + radius, z - radius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };  //  Γ
    _vertices[ 2] = { { x - radius, y - radius, z - radius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };  //  ┘
    _vertices[ 3] = { { x - radius, y + radius, z - radius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };  //  ┐
    
    // right side
    _vertices[ 4] = { { x + radius, y + radius, z - radius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };  //  ∟
    _vertices[ 5] = { { x + radius, y - radius, z - radius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };  //  Γ
    _vertices[ 6] = { { x + radius, y + radius, z + radius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };  //  ┘
    _vertices[ 7] = { { x + radius, y - radius, z + radius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };  //  ┐
    
    // left side
    _vertices[ 8] = { { x - radius, y + radius, z + radius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };  //  ∟
    _vertices[ 9] = { { x - radius, y - radius, z + radius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };  //  Γ
    _vertices[10] = { { x - radius, y + radius, z - radius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };  //  ┘
    _vertices[11] = { { x - radius, y - radius, z - radius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };  //  ┐

    // back side
    _vertices[12] = { { x - radius, y - radius, z + radius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };  //  ∟
    _vertices[13] = { { x - radius, y + radius, z + radius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };  //  Γ
    _vertices[14] = { { x + radius, y - radius, z + radius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };  //  ┘
    _vertices[15] = { { x + radius, y + radius, z + radius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };  //  ┐

    // top side
    _vertices[16] = { { x + radius, y + radius, z - radius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };  //  ∟
    _vertices[17] = { { x + radius, y + radius, z + radius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };  //  Γ
    _vertices[18] = { { x - radius, y + radius, z - radius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };  //  ┘
    _vertices[19] = { { x - radius, y + radius, z + radius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };  //  ┐

    // bottom side
    _vertices[20] = { { x - radius, y - radius, z - radius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };  //  ∟
    _vertices[21] = { { x - radius, y - radius, z + radius }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };  //  Γ
    _vertices[22] = { { x + radius, y - radius, z - radius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };  //  ┘
    _vertices[23] = { { x + radius, y - radius, z + radius }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };  //  ┐

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

        Logger::DebugLogInfo(
            "normal for # " + std::to_string(_indices[i]) + "," + std::to_string(_indices[i + 1]) + "," + std::to_string(_indices[i + 2])
            + " : <" + std::to_string(normal.m128_f32[0]) + ", " + std::to_string(normal.m128_f32[1]) + ", " + std::to_string(normal.m128_f32[2]) + ">"
        );
    }

}

Mesh::Mesh()
    : Mesh(0.5f, {})
{
}

Mesh::~Mesh()
{
    delete _vertexBuffer;
    delete _indexBuffer;

    for (size_t i = 0; i < _constantBuffers.size(); ++i)
    {
        delete _constantBuffers.at(i);
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
