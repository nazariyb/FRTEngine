#pragma once

#include "FRTEngine.h"

#include <initguid.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "Tools/d3dx12.h"
#include "Camera.h"


namespace frt
{
using Microsoft::WRL::ComPtr;
class Window;

class FRTENGINE_API Graphics
{
    friend class GraphicsResource;

public:
    Graphics(Window* owner, HWND hWindow);
    virtual ~Graphics();

    virtual void Init(HWND hWindow);
    virtual void Update();
    virtual void Render();
    virtual void Destroy();

    // TMP: start
    const float translationSpeed = 0.06f;
    const float offsetBoundsX = 1.25f;
    const float offsetBoundsY = 1.42f;
    float currentOffsetX = 0;
    float currentOffsetY = 0;
    float Roll = 0.f;
    float Pitch = 0.f;
    float Yaw = 0.f;
    DirectX::XMMATRIX currentRotation;
    DirectX::XMMATRIX currentTranslation;
    bool bRotate = true;
    bool bMove = true;
    const float zOffset = 4.f;
    const float fovDegrees = 90.f;
    const float perspectiveNearZ = 1.f;
    const float perspectiveFarZ = 1000.f;

    inline void OnKeyDown(UINT8 key)
    {
        _camera.OnKeyDown(key);
    }

    inline void OnKeyUp(UINT8 key)
    {
        _camera.OnKeyUp(key);
    }

    void BeforeFirstTick();
    // TMP: end

private:
    Window* _owner;

public:
    static const UINT FrameCount = 2;
private:

    static const UINT RowCount = 10;
    static const UINT ColumnCount = 20;
public:
    static const UINT MaterialCount = RowCount * ColumnCount;
private:
    static const bool bUseBundles = false; // TODO:
    static const float SpacingInterval;

    UINT _constBuffersNum = 0;

    // Pipeline objects.
    CD3DX12_VIEWPORT _viewport;
    CD3DX12_RECT _scissorRect;
    ComPtr<IDXGISwapChain3> _swapChain;
    ComPtr<ID3D12Device> _device;
    ComPtr<ID3D12Resource> _renderTargets[FrameCount];
    ComPtr<ID3D12CommandAllocator> _commandAllocator;
    ComPtr<ID3D12CommandAllocator> _commandAllocators[FrameCount];
    ComPtr<ID3D12CommandQueue> _commandQueue;
    ComPtr<ID3D12RootSignature> _rootSignature;
    ComPtr<ID3D12DescriptorHeap> _rtvHeap;
    ComPtr<ID3D12DescriptorHeap> _cbvSrvHeap;
    ComPtr<ID3D12PipelineState> _pipelineState;
    ComPtr<ID3D12GraphicsCommandList> _commandList;
    ComPtr<ID3D12GraphicsCommandList> _bundle;

    ComPtr<ID3D12Resource> _depthStencil;
    ComPtr<ID3D12DescriptorHeap> _dsvHeap;
    ComPtr<ID3D12DescriptorHeap> _samplerHeap;


    UINT _rtvDescriptorSize;
    UINT _cbvSrvDescriptorSize;

    std::vector<UINT64> _fenceValues;

    UINT _indicesNum;
    ComPtr<ID3D12Resource> _textures[MaterialCount];

public:
    Camera _camera;
private:

public:
    UINT _currentFrameResourceIndex;
private:
    // Synchronization objects.
    UINT _frameIndex;
    UINT _frameCounter;
    HANDLE _fenceEvent;
    ComPtr<ID3D12Fence> _fence;
    UINT64 _fenceValue;

public:
    float _aspectRatio = 1280. / 720.;
private:

    void LoadPipeline(HWND hWindow);
    void LoadAssets();
    void PopulateCommandList();
};
}

