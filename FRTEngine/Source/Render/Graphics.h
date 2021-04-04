#pragma once

#include "FRTEngine.h"

#include <initguid.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <bitset>

#include "Render/Mesh.h"
#include "Tools/d3dx12.h"
#include "Camera.h"
#include "FrameResource.h"


//TMP:
#define NEW_FEATURES 1

namespace frt
{
using Microsoft::WRL::ComPtr;
class Window;

class FRTENGINE_API Graphics
{
public:
    Graphics(Window* owner, HWND hWindow);
    virtual ~Graphics();

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
    const float translationSpeed = 0.06f;
    const float offsetBoundsX = 1.25f;
    const float offsetBoundsY = 1.42f;
    float currentOffsetX = 0;
    float currentOffsetY = 0;
    DirectX::XMMATRIX currentRotation;
    DirectX::XMMATRIX currentTranslation;
    bool bRotate = true;
    bool bMove = true;
    const float zOffset = 4.f;
    const float fovDegrees = 90.f;
    const float perspectiveNearZ = 1.f;
    const float perspectiveFarZ = 1000.f;

    void OnKeyDown(UINT8 key)
    {
        _camera.OnKeyDown(key);
    }

    void OnKeyUp(UINT8 key)
    {
        _camera.OnKeyUp(key);
    }
    // TMP: end

private:
    Window* _owner;

    static const UINT FrameCount = 2;
    static const UINT TextureWidth = 256;
    static const UINT TextureHeight = 256;
    static const UINT TexturePixelSize = 4;    // The number of bytes used to represent a pixel in the texture.;

#if NEW_FEATURES
    static const UINT RowCount = 3;
    static const UINT ColumnCount = 2;
    static const UINT MaterialCount = RowCount * ColumnCount;
    static const bool bUseBundles = false; // TODO:
    static const float SpacingInterval;
#endif

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
#if NEW_FEATURES
    ComPtr<ID3D12CommandAllocator> _commandAllocator;
#else
    ComPtr<ID3D12CommandAllocator> _commandAllocators[FrameCount];
    ComPtr<ID3D12CommandAllocator> _bundleAllocators[FrameCount];
#endif
    ComPtr<ID3D12CommandQueue> _commandQueue;
    ComPtr<ID3D12RootSignature> _rootSignature;
    ComPtr<ID3D12DescriptorHeap> _rtvHeap;
    ComPtr<ID3D12DescriptorHeap> _srvHeap;
    ComPtr<ID3D12PipelineState> _pipelineState;
    ComPtr<ID3D12GraphicsCommandList> _commandList;
    ComPtr<ID3D12GraphicsCommandList> _bundle;

#if NEW_FEATURES
    ComPtr<ID3D12Resource> _depthStencil; // TOOD:
    ComPtr<ID3D12DescriptorHeap> _dsvHeap;
    ComPtr<ID3D12DescriptorHeap> _samplerHeap;
#endif


    UINT _rtvDescriptorSize;
    UINT _cbvSrvDescriptorSize;

    // App resources.
    ComPtr<ID3D12Resource> _vertexBuffer;
    ComPtr<ID3D12Resource> _indexBuffer;

    D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;
    D3D12_INDEX_BUFFER_VIEW _indexBufferView;

    ComPtr<ID3D12Resource> _constantBuffer;
    SceneConstantBuffer _constantBufferData;
    UINT8* _pCbvDataBegin;

#if NEW_FEATURES
    UINT _indicesNum;
    ComPtr<ID3D12Resource> _textures[MaterialCount];
    Camera _camera;
#else
    ComPtr<ID3D12Resource> _texture;
#endif

#if NEW_FEATURES
    // Frame resources.
    std::vector<FrameResource*> _frameResources;
    FrameResource* _currentFrameResource;
    UINT _currentFrameResourceIndex;
#endif

    // Synchronization objects.
    UINT _frameIndex;
#if NEW_FEATURES
    UINT _frameCounter;
#endif
    HANDLE _fenceEvent;
    ComPtr<ID3D12Fence> _fence;
#if NEW_FEATURES
    UINT64 _fenceValue;
#else
    UINT64 _fenceValues[FrameCount];
#endif

    float _aspectRatio = 1280. / 720.;

    void LoadPipeline(HWND hWindow);
    void LoadAssets();
    std::vector<UINT8> GenerateTextureData();
#if NEW_FEATURES
    void PopulateCommandList(FrameResource* pFrameResource);
#else
    void PopulateCommandList();
#endif
    void WaitForPreviousFrame();
    void CreateFrameResources();

    Mesh* _mesh;
};
}

