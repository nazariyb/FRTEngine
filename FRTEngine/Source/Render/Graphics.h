#pragma once

#include "FRTEngine.h"

#include <initguid.h>
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
    static const UINT TextureWidth = 256;
    static const UINT TextureHeight = 256;
    static const UINT TexturePixelSize = 4;    // The number of bytes used to represent a pixel in the texture.;

    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 uv;
        //DirectX::XMFLOAT4 color;
    };

    // Pipeline objects.
    CD3DX12_VIEWPORT _viewport;
    CD3DX12_RECT _scissorRect;
    ComPtr<IDXGISwapChain3> _swapChain;
    ComPtr<ID3D12Device> _device;
    ComPtr<ID3D12Resource> _renderTargets[FrameCount];
    ComPtr<ID3D12CommandAllocator> _commandAllocator;
    ComPtr<ID3D12CommandAllocator> _bundleAllocator;
    ComPtr<ID3D12CommandQueue> _commandQueue;
    ComPtr<ID3D12RootSignature> _rootSignature;
    ComPtr<ID3D12DescriptorHeap> _rtvHeap;
    ComPtr<ID3D12DescriptorHeap> _srvHeap;
    ComPtr<ID3D12PipelineState> _pipelineState;
    ComPtr<ID3D12GraphicsCommandList> _commandList;
    ComPtr<ID3D12GraphicsCommandList> _bundle;
    UINT _rtvDescriptorSize;

    // App resources.
    ComPtr<ID3D12Resource> _vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;
    ComPtr<ID3D12Resource> _texture;

    // Synchronization objects.
    UINT _frameIndex;
    HANDLE _fenceEvent;
    ComPtr<ID3D12Fence> _fence;
    UINT64 _fenceValue;

    float _aspectRatio = 1280. / 720.;

    void LoadPipeline(HWND hWindow);
    void LoadAssets();
    std::vector<UINT8> GenerateTextureData();
    void PopulateCommandList();
    void WaitForPreviousFrame();
};
}

