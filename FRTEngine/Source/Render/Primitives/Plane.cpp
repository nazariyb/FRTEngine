#include "Plane.h"


namespace frt
{
    // Plane::Plane()
    //     : Plane(1.0f)
    // {
    // }
    //
    // Plane::Plane(float radius)
    //     : Plane(1.f, 1.f, {})
    // {
    // }

    Plane::Plane(float width, float height, DirectX::XMFLOAT3 initialPosition, Orientation orientation)
        : Mesh(width, initialPosition), _width(width), _height(height), _orientation(orientation)
    {
        vertexBufferSlot = static_cast<unsigned>(_orientation) + 1;
        Plane::Resize(_radius);
    }

    Plane::~Plane()
    {
    }

    void Plane::Resize(float newRadius)
    {
        Mesh::Resize(newRadius);

        const float x = _initialPosition.x;
        const float y = _initialPosition.y;
        const float z = _initialPosition.z;

        float xHeightOffset{0.f}, xWidthOffset{0.f}, xXOffset{0.f};
        if (_orientation == X)
        {
            xHeightOffset = _height / 2.f;
            xWidthOffset = _width / 2.f;
            xXOffset = 0.3f;
        }

        float yHeightOffset{0.f}, yWidthOffset{0.f}, yYOffset{0.f};
        if (_orientation == Y)
        {
            yHeightOffset = _height / 2.f;
            yWidthOffset = _width / 2.f;
            yYOffset = 0.3f;
        }

        const float xOffset = yWidthOffset + xXOffset;
        const float yOffset = xHeightOffset + yYOffset;

        // front side
        _vertices[0] = {{x + xOffset, y - yOffset, z - xWidthOffset}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[1] = {{x + xOffset, y + yOffset, z - xWidthOffset}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[2] = {{x - xOffset, y - yOffset, z - xWidthOffset}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[3] = {{x - xOffset, y + yOffset, z - xWidthOffset}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐

        // right side
        _vertices[4] = {{x + xOffset, y - yOffset, z - xWidthOffset}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[5] = {{x + xOffset, y + yOffset, z - xWidthOffset}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[6] = {{x + xOffset, y - yOffset, z + xWidthOffset}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[7] = {{x + xOffset, y + yOffset, z + xWidthOffset}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐

        // left side
        _vertices[8]  = {{x - xOffset, y - yOffset, z + xWidthOffset}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[9]  = {{x - xOffset, y + yOffset, z + xWidthOffset}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[10] = {{x - xOffset, y - yOffset, z - xWidthOffset}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[11] = {{x - xOffset, y + yOffset, z - xWidthOffset}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐

        // back side
        _vertices[12] = {{x - xOffset, y - yOffset, z + xWidthOffset}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[13] = {{x - xOffset, y + yOffset, z + xWidthOffset}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[14] = {{x + xOffset, y - yOffset, z + xWidthOffset}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[15] = {{x + xOffset, y + yOffset, z + xWidthOffset}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐

        // top side
        _vertices[16] = {{x + xOffset, y + yOffset, z - yHeightOffset}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[17] = {{x + xOffset, y + yOffset, z + yHeightOffset}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[18] = {{x - xOffset, y + yOffset, z - yHeightOffset}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[19] = {{x - xOffset, y + yOffset, z + yHeightOffset}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐

        // bottom side
        _vertices[20] = {{x - xOffset, y - yOffset, z - yHeightOffset}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}; //  ∟
        _vertices[21] = {{x - xOffset, y - yOffset, z + yHeightOffset}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}; //  Γ
        _vertices[22] = {{x + xOffset, y - yOffset, z - yHeightOffset}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}; //  ┘
        _vertices[23] = {{x + xOffset, y - yOffset, z + yHeightOffset}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}; //  ┐

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
