#pragma once

#include "FRTEngine.h"
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include "WindowsMinimal.h"
#include "ITickable.h"
#include "Render/ConstantBuffer.h"
#include <memory>

namespace frt
{
using Microsoft::WRL::ComPtr;

class FRTENGINE_API Mesh : public ITickable
{
public:
    Mesh();
    Mesh(float radius, DirectX::XMFLOAT3 initialPosition);
    virtual ~Mesh();

    virtual void Update() override;
    virtual void PopulateCommandList() override;

    void InitializeGraphicsResources(class Graphics* graphics);

public:

    struct SceneObjectConstantBuffer
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

    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT2 uv;
    };

    void InitializeConstantBuffers(Graphics* graphics);
    void UpdateConstantBuffer(const SceneObjectConstantBuffer& newBuffer);

    Vertex* GetVertices();
    UINT8* GetIndices();

    inline unsigned int GetVertexDataSize() const { return _vertexBufferSize; };
    inline unsigned int GetIndexDataSize() const { return _indexBufferSize; };

    static std::vector<UINT8> GenerateTextureData(uint32_t color1, uint32_t color2);

protected:
    std::vector<ConstantBuffer<SceneObjectConstantBuffer>*> _constantBuffers;
    class IndexBuffer* _indexBuffer;
    class VertexBuffer* _vertexBuffer;

    unsigned int _radius;
    DirectX::XMFLOAT3 _initialPosition;

    static const unsigned int _vertexBufferSize = 24;
    Vertex _vertices[_vertexBufferSize];

    const std::vector<unsigned char> _indices = 
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
    const unsigned int _indexBufferSize = _indices.size();

    std::vector<unsigned char> _texture;
public:
    static const unsigned int TextureWidth = 256;
    static const unsigned int TextureHeight = 256;
    static const unsigned int TexturePixelSize = 4;    // The number of bytes used to represent a pixel in the texture.;
private:
    //ComPtr<ID3D12Resource> _vertexBuffer;
    //ComPtr<ID3D12Resource> _indexBuffer;
};
}

