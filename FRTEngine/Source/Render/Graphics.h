#pragma once

#include "FRTEngine.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "Tools/d3dx12.h"


namespace frt
{
using Microsoft::WRL::ComPtr;

class FRTENGINE_API Graphics
{
public:
    Graphics(HWND hWindow);

    virtual void Init(HWND hWindow);
    virtual void Update();
    virtual void Render();
    virtual void Destroy();


private:
    static const UINT FrameCount = 2;

    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
    };

    // Pipeline objects.
    D3D12_VIEWPORT _viewport;
    D3D12_RECT _scissorRect;
    ComPtr<IDXGISwapChain3> _swapChain;
    ComPtr<ID3D12Device> _device;
    ComPtr<ID3D12Resource> _renderTargets[FrameCount];
    ComPtr<ID3D12CommandAllocator> _commandAllocator;
    ComPtr<ID3D12CommandQueue> _commandQueue;
    ComPtr<ID3D12RootSignature> _rootSignature;
    ComPtr<ID3D12DescriptorHeap> _rtvHeap;
    ComPtr<ID3D12PipelineState> _pipelineState;
    ComPtr<ID3D12GraphicsCommandList> _commandList;
    UINT _rtvDescriptorSize;

    // App resources.
    ComPtr<ID3D12Resource> _vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;

    // Synchronization objects.
    UINT _frameIndex;
    HANDLE _fenceEvent;
    ComPtr<ID3D12Fence> _fence;
    UINT64 _fenceValue;

    float _aspectRatio = 1280. / 720.;

    void LoadPipeline(HWND hWindow);
    void LoadAssets();
    void PopulateCommandList();
    void WaitForPreviousFrame();
};
}

