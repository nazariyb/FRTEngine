#include "Plane.h"


namespace frt
{
    Plane::Plane()
        : Plane(1.0f)
    {
    }

    Plane::Plane(float radius)
        : Plane(1.f, {})
    {
    }

    Plane::Plane(float radius, DirectX::XMFLOAT3 initialPosition)
        : Mesh(radius, initialPosition)
    {
        vertexBufferSlot = 1;
        Plane::Resize(_radius);
    }

    Plane::~Plane()
    {
    }

    void Plane::Resize(float newRadius)
    {
        Mesh::Resize(newRadius);
        
        const float x = _initialPosition.x;
        const float y = _initialPosition.x;
        const float z = _initialPosition.x;
        
        // front side
        _vertices[0] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[1] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[2] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[3] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐
        
        // right side
        _vertices[4] = {{x + 0.0001f, y + newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[5] = {{x + 0.0001f, y - newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[6] = {{x + 0.0001f, y + newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[7] = {{x + 0.0001f, y - newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐
        
        // left side
        _vertices[8] = {{x - 0.0001f, y + newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[9] = {{x - 0.0001f, y - newRadius, z + newRadius}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[10] = {{x - 0.0001f, y + newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[11] = {{x - 0.0001f, y - newRadius, z - newRadius}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐
        
        // back side
        _vertices[12] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[13] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[14] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[15] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐
        
        // top side
        _vertices[16] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[17] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[18] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[19] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐
        
        // bottom side
        _vertices[20] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[21] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[22] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[23] = {{x, y, z}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐
        
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

    void Plane::Update()
    {
        Mesh::Update();
    }
    
    void Plane::PopulateCommandList()
    {
        Mesh::PopulateCommandList();
    }
}
