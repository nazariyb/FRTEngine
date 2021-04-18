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
    std::bitset<6> moveDirections;
    int currentCubeIndex = 0;

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

    inline void SetCubeIndex(int NewIndex)
    {
        //Roll = 0.f;
        //Pitch = 0.f;
        //Yaw = 0.f;
        //currentCubeIndex = (NewIndex - 1) % MaterialCount;
    }
    // TMP: end

private:
    Window* _owner;

    static const UINT FrameCount = 2;

    static const UINT RowCount = 2;
    static const UINT ColumnCount = 4;
    static const UINT MaterialCount = RowCount * ColumnCount;
    static const bool bUseBundles = false; // TODO:
    static const float SpacingInterval;

    //struct Vertex
    //{
    //    DirectX::XMFLOAT3 position;
    //    DirectX::XMFLOAT2 uv;
    //    //DirectX::XMFLOAT4 color;
    //};

    //struct SceneConstantBuffer
    //{
    //    DirectX::XMFLOAT4 offset;
    //    DirectX::XMMATRIX transform;
    //    float padding[44]; // Padding so the constant buffer is 256-byte aligned.
    //};
    //static_assert((sizeof(SceneConstantBuffer) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

    // Pipeline objects.
    CD3DX12_VIEWPORT _viewport;
    CD3DX12_RECT _scissorRect;
    ComPtr<IDXGISwapChain3> _swapChain;
    ComPtr<ID3D12Device> _device;
    ComPtr<ID3D12Resource> _renderTargets[FrameCount];
    ComPtr<ID3D12CommandAllocator> _commandAllocator;
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

    // App resources.
    ComPtr<ID3D12Resource> _vertexBuffer;
    ComPtr<ID3D12Resource> _indexBuffer;

    D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;
    D3D12_INDEX_BUFFER_VIEW _indexBufferView;

    class VertexBuffer* _myVertexBuffer;
    class IndexBuffer* _myIndexBuffer;

    UINT _indicesNum;
    ComPtr<ID3D12Resource> _textures[MaterialCount];

    Camera _camera;

    // Frame resources.
    std::vector<FrameResource*> _frameResources;
    FrameResource* _currentFrameResource;
    UINT _currentFrameResourceIndex;

    // Synchronization objects.
    UINT _frameIndex;
    UINT _frameCounter;
    HANDLE _fenceEvent;
    ComPtr<ID3D12Fence> _fence;
    UINT64 _fenceValue;

    float _aspectRatio = 1280. / 720.;

    void LoadPipeline(HWND hWindow);
    void LoadAssets();
    void PopulateCommandList(FrameResource* pFrameResource);
    void CreateFrameResources();

    Mesh* _mesh;
};
}

