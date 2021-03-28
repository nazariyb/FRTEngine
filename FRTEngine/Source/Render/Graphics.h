#pragma once

#include "FRTEngine.h"

#include <initguid.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <bitset>

#include "Tools/d3dx12.h"


namespace frt
{
using Microsoft::WRL::ComPtr;
class Window;

class FRTENGINE_API Graphics
{
public:
    Graphics(Window* owner, HWND hWindow);

    virtual void Init(HWND hWindow);
    virtual void Update();
    virtual void Render();
    virtual void Destroy();

    // TMP: start
    void MoveForwardY();
    void MoveBackwardX();
    void MoveBackwardY();
    void MoveForwardX();

    std::bitset<4> moveDirections;
    const float translationSpeed = 0.04f;
    const float offsetBoundsX = 1.25f;
    const float offsetBoundsY = 1.42f;
    float currentOffsetX = 0;
    float currentOffsetY = 0;
    DirectX::XMMATRIX currentRotation;
    DirectX::XMMATRIX bufferTranslation;
    DirectX::XMMATRIX bufferTranslation2;
    DirectX::XMMATRIX baseTrasform;
    bool bRotate = true;
    bool bMove = true;
    const float zOffset = .25f;
    const float fovDegrees = 120.f;
    const float perspectiveNearZ = .05f;
    const float perspectiveFarZ = 10.f;
    // TMP: end

private:
    Window* _owner;

    static const UINT FrameCount = 2;
    static const UINT TextureWidth = 256;
    static const UINT TextureHeight = 256;
    static const UINT TexturePixelSize = 4;    // The number of bytes used to represent a pixel in the texture.;

    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 uv;
        //DirectX::XMFLOAT4 color;
    };

    struct SceneConstantBuffer
    {
        DirectX::XMFLOAT4 offset;
        DirectX::XMMATRIX transform;
        float padding[44]; // Padding so the constant buffer is 256-byte aligned.
    };
    static_assert((sizeof(SceneConstantBuffer) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

    // Pipeline objects.
    CD3DX12_VIEWPORT _viewport;
    CD3DX12_RECT _scissorRect;
    ComPtr<IDXGISwapChain3> _swapChain;
    ComPtr<ID3D12Device> _device;
    ComPtr<ID3D12Resource> _renderTargets[FrameCount];
    ComPtr<ID3D12CommandAllocator> _commandAllocators[FrameCount];
    ComPtr<ID3D12CommandAllocator> _bundleAllocators[FrameCount];
    ComPtr<ID3D12CommandQueue> _commandQueue;
    ComPtr<ID3D12RootSignature> _rootSignature;
    ComPtr<ID3D12DescriptorHeap> _rtvHeap;
    ComPtr<ID3D12DescriptorHeap> _srvHeap;
    //ComPtr<ID3D12DescriptorHeap> _cbvHeap;
    ComPtr<ID3D12PipelineState> _pipelineState;
    ComPtr<ID3D12GraphicsCommandList> _commandList;
    ComPtr<ID3D12GraphicsCommandList> _bundle;
    UINT _rtvDescriptorSize;
    UINT _cbvSrvDescriptorSize;

    // App resources.
    ComPtr<ID3D12Resource> _vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;
    ComPtr<ID3D12Resource> _indexBuffer;
    D3D12_INDEX_BUFFER_VIEW _indexBufferView;
    ComPtr<ID3D12Resource> _texture;
    ComPtr<ID3D12Resource> _constantBuffer;
    SceneConstantBuffer _constantBufferData;
    UINT8* _pCbvDataBegin;

    // Synchronization objects.
    UINT _frameIndex;
    HANDLE _fenceEvent;
    ComPtr<ID3D12Fence> _fence;
    UINT64 _fenceValues[FrameCount];

    float _aspectRatio = 1280. / 720.;

    void LoadPipeline(HWND hWindow);
    void LoadAssets();
    std::vector<UINT8> GenerateTextureData();
    void PopulateCommandList();
    void WaitForPreviousFrame();
};
}

