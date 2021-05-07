#include "Cube.h"


namespace frt
{
    Cube::Cube()
        : Cube(1.0f)
    {
    }

    Cube::Cube(float radius)
        : Cube(1.f, {})
    {
    }

    Cube::Cube(float radius, DirectX::XMFLOAT3 initialPosition)
        : Mesh(radius, initialPosition)
    {
        vertexBufferSlot = 0;
        Cube::Resize(_radius);
    }

    Cube::~Cube()
    {
    }

    void Cube::Resize(float newRadius)
    {
        Mesh::Resize(newRadius);
        
        const float x = _initialPosition.x;
        const float y = _initialPosition.x;
        const float z = _initialPosition.x;
        
        // front side
        _vertices[0] = {{x + newRadius, y - newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[1] = {{x + newRadius, y + newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[2] = {{x - newRadius, y - newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[3] = {{x - newRadius, y + newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐
        
        // right side
        _vertices[4] = {{x + newRadius, y + newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[5] = {{x + newRadius, y - newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[6] = {{x + newRadius, y + newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[7] = {{x + newRadius, y - newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐
        
        // left side
        _vertices[8] = {{x - newRadius, y + newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[9] = {{x - newRadius, y - newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[10] = {{x - newRadius, y + newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[11] = {{x - newRadius, y - newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐
        
        // back side
        _vertices[12] = {{x - newRadius, y - newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[13] = {{x - newRadius, y + newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[14] = {{x + newRadius, y - newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[15] = {{x + newRadius, y + newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐
        
        // top side
        _vertices[16] = {{x + newRadius, y + newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[17] = {{x + newRadius, y + newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[18] = {{x - newRadius, y + newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[19] = {{x - newRadius, y + newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐
        
        // bottom side
        _vertices[20] = {{x - newRadius, y - newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[21] = {{x - newRadius, y - newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[22] = {{x + newRadius, y - newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[23] = {{x + newRadius, y - newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐
        
        using namespace DirectX;
        
        for (size_t i = 0; i < _indices.size(); i += 3)
        {
            Vertex& v0 = _vertices[_indices[i]];
            Vertex& v1 = _vertices[_indices[i + 1]];
            Vertex& v2 = _vertices[_indices[i + 2]];
            const XMVECTOR pos0 = XMLoadFloat3(&v0.position);
            const XMVECTOR pos1 = XMLoadFloat3(&v1.position);
            const XMVECTOR pos2 = XMLoadFloat3(&v2.position);
        
            const auto normal = XMVector3Normalize(XMVector3Cross(pos2 - pos0, pos1 - pos0));
        
            XMStoreFloat3(&v0.normal, normal);
            XMStoreFloat3(&v1.normal, normal);
            XMStoreFloat3(&v2.normal, normal);
        }
    }

    void Cube::Update()
    {
        Mesh::Update();
    }
    
    void Cube::PopulateCommandList()
    {
        Mesh::PopulateCommandList();
    }
}
