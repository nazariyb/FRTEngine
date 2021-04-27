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
    static_assert((sizeof(SceneObjectConstantBuffer) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

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
    static inline unsigned int GetIndexDataSize() { return _indexBufferSize; };

    static std::vector<UINT8> GenerateTextureData(uint32_t color1, uint32_t color2);

protected:
    std::vector<ConstantBuffer<SceneObjectConstantBuffer>*> _constantBuffers;
    class IndexBuffer* _indexBuffer;
    class VertexBuffer* _vertexBuffer;

    unsigned int _radius;
    DirectX::XMFLOAT3 _initialPosition;

    static const unsigned int _vertexBufferSize = 24;
    Vertex _vertices[_vertexBufferSize];

    static const std::vector<unsigned char> _indices;
    static const unsigned int _indexBufferSize;

    std::vector<unsigned char> _texture;

public:
    static const unsigned int TextureWidth = 256;
    static const unsigned int TextureHeight = 256;
    static const unsigned int TexturePixelSize = 4;    // The number of bytes used to represent a pixel in the texture.;
};
}

