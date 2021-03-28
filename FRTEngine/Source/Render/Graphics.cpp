#include "Graphics.h"
#include "Exception.h"
#include "Debug/Debug.h"
#include "Event.h"
#include "App.h"
#include "Window.h"
#include "MathLib.h"

#include <string>
#include "Utils/Logger/Logger.h"


void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter)
{
    *ppAdapter = nullptr;
    for (UINT adapterIndex = 0; ; ++adapterIndex)
    {
        IDXGIAdapter1* pAdapter = nullptr;
        if (DXGI_ERROR_NOT_FOUND == pFactory->EnumAdapters1(adapterIndex, &pAdapter))
        {
            // No more adapters to enumerate.
            break;
        }

        // Check to see if the adapter supports Direct3D 12, but don't create the
        // actual device yet.
        if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
        {
            *ppAdapter = pAdapter;
            return;
        }
        pAdapter->Release();
    }
}

namespace frt
{

Graphics::Graphics(Window* owner, HWND hWindow) :
    _owner(owner),
    _frameIndex(0),
    _pCbvDataBegin(nullptr),
    _viewport(0.0f, 0.0f, static_cast<float>(1280), static_cast<float>(720)),
    _scissorRect(static_cast<LONG>(0), static_cast<LONG>(0), static_cast<LONG>(1280), static_cast<LONG>(720)),
    _fenceValues{},
    _rtvDescriptorSize(0),
    _cbvSrvDescriptorSize(0),
    _constantBufferData{},
    currentRotation{ DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0) },
    baseTrasform{
        DirectX::XMMatrixTranslation(0, 0, zOffset) //*
        //DirectX::XMMatrixPerspectiveLH(1, 1. / _aspectRatio, .05, 1)
    }
{
    //_hWindow = hWindow;
    Init(hWindow);
    _constantBufferData.transform = DirectX::XMMatrixTranspose(
        currentRotation *
        baseTrasform
    );
}

void Graphics::Init(HWND hWindow)
{
    LoadPipeline(hWindow);
    LoadAssets();
}

void Graphics::Update()
{
    //CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart(), _frameIndex, _rtvDescriptorSize);
    //_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    //const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    //_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);


    if (moveDirections[0]) // W
        MoveForwardY();
    if (moveDirections[1]) // A
        MoveBackwardX();
    if (moveDirections[2]) // S
        MoveBackwardY();
    if (moveDirections[3]) // D
        MoveForwardX();

}

void Graphics::Render()
{
    // Record all the commands we need to render the scene into the command list.
    PopulateCommandList();

    // Execute the command list.
    ID3D12CommandList* ppCommandLists[] = { _commandList.Get() };
    _commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Present the frame.
    THROW_IF_FAILED(_swapChain->Present(1, 0));

    WaitForPreviousFrame();
}

void Graphics::Destroy()
{

}

#pragma region transformations

void Graphics::MoveForwardY()
{
    bufferTranslation = DirectX::XMMatrixTranslation(0, 0, -(zOffset + .125));

    if (bRotate)
    {
        currentRotation *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(2));
    }
    if (bMove)
    {
        currentOffsetY += translationSpeed;
        if (currentOffsetY > offsetBoundsY)
        {
            currentOffsetY = -offsetBoundsY;
        }
    }

    bufferTranslation2 = DirectX::XMMatrixTranslation(currentOffsetX, currentOffsetY, (zOffset + .125));
    _constantBufferData.transform = DirectX::XMMatrixTranspose(
        bufferTranslation *
        currentRotation *
        bufferTranslation2 *
        DirectX::XMMatrixPerspectiveLH(1, 1, perspectiveNearZ, perspectiveFarZ)
        //DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fovDegrees), 1., perspectiveNearZ, perspectiveFarZ)
    );

    memcpy(_pCbvDataBegin, &_constantBufferData, sizeof(_constantBufferData));
}

void Graphics::MoveBackwardY()
{
    bufferTranslation = DirectX::XMMatrixTranslation(0, 0, -(zOffset + .125));

    if (bRotate)
    {
        currentRotation *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-2));
    }
    if (bMove)
    {
        currentOffsetY -= translationSpeed;
        if (currentOffsetY < -offsetBoundsY)
        {
            currentOffsetY = offsetBoundsY;
        }
    }
    
    bufferTranslation2 = DirectX::XMMatrixTranslation(currentOffsetX, currentOffsetY, (zOffset + .125));
    _constantBufferData.transform = DirectX::XMMatrixTranspose(
        bufferTranslation *
        currentRotation *
        bufferTranslation2 *
        DirectX::XMMatrixPerspectiveLH(1, 1, perspectiveNearZ, perspectiveFarZ)
        //DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fovDegrees), 1., perspectiveNearZ, perspectiveFarZ)
    );

    memcpy(_pCbvDataBegin, &_constantBufferData, sizeof(_constantBufferData));
}

void Graphics::MoveBackwardX()
{
    bufferTranslation = DirectX::XMMatrixTranslation(0, 0, -(zOffset + .125));

    if (bRotate)
    {
        currentRotation *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(2));
    }
    if (bMove)
    {
        currentOffsetX -= translationSpeed;
        if (currentOffsetX < -offsetBoundsX)
        {
            currentOffsetX = offsetBoundsX;
        }
    }

    bufferTranslation2 = DirectX::XMMatrixTranslation(currentOffsetX, currentOffsetY, (zOffset + .125));
    _constantBufferData.transform = DirectX::XMMatrixTranspose(
        bufferTranslation *
        currentRotation *
        bufferTranslation2 *
        DirectX::XMMatrixPerspectiveLH(1, 1, perspectiveNearZ, perspectiveFarZ)
        //DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fovDegrees), 1., perspectiveNearZ, perspectiveFarZ)
    );

    memcpy(_pCbvDataBegin, &_constantBufferData, sizeof(_constantBufferData));
}

void Graphics::MoveForwardX()
{
    bufferTranslation = DirectX::XMMatrixTranslation(0, 0, -(zOffset + .125));
    
    if (bRotate)
    {
        currentRotation *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(-2));
    }
    if (bMove)
    {
        currentOffsetX += translationSpeed;
        if (currentOffsetX > offsetBoundsX)
        {
            currentOffsetX = -offsetBoundsX;
        }
    }
    
    bufferTranslation2 = DirectX::XMMatrixTranslation(currentOffsetX, currentOffsetY, (zOffset + .125));
    _constantBufferData.transform = DirectX::XMMatrixTranspose(
        bufferTranslation *
        currentRotation *
        bufferTranslation2 *
        DirectX::XMMatrixPerspectiveLH(1, 1, perspectiveNearZ, perspectiveFarZ)
        //DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fovDegrees), 1., perspectiveNearZ, perspectiveFarZ)
    );

    memcpy(_pCbvDataBegin, &_constantBufferData, sizeof(_constantBufferData));
}

#pragma endregion

void Graphics::LoadPipeline(HWND hWindow)
{
    UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
    // Enable the D3D12 debug layer.
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();

            // Enable additional debug layers.
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
#endif

    ComPtr<IDXGIFactory4> factory;
    THROW_IF_FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

    ComPtr<IDXGIAdapter1> hardwareAdapter;
    GetHardwareAdapter(factory.Get(), &hardwareAdapter);

    THROW_IF_FAILED(D3D12CreateDevice(
        hardwareAdapter.Get(),
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&_device)
    ));

    // Describe and create the command queue.
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    THROW_IF_FAILED(_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_commandQueue)));

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = FrameCount;
    swapChainDesc.BufferDesc.Width = 1280;
    swapChainDesc.BufferDesc.Height = 720;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    //swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    //swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    //swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    //swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    //swapChainDesc.Flags = 0;
    swapChainDesc.OutputWindow = hWindow;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;
    //swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;

    ComPtr<IDXGISwapChain> swapChain;
    THROW_IF_FAILED(factory->CreateSwapChain(
        _commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
        &swapChainDesc,
        &swapChain
    ));

    THROW_IF_FAILED(swapChain.As(&_swapChain));

    _frameIndex = _swapChain->GetCurrentBackBufferIndex();

    // Create descriptor heaps.
    {
        // Describe and create a render target view (RTV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = FrameCount;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        THROW_IF_FAILED(_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvHeap)));

        // Describe and create a shader resource view (SRV) heap for the texture.
        D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
        srvHeapDesc.NumDescriptors = 2;
        srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        THROW_IF_FAILED(_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap)));

        _rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        _cbvSrvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        // Describe and create a constant buffer view (CBV) descriptor heap.
        // Flags indicate that this descriptor heap can be bound to the pipeline 
        // and that descriptors contained in it can be referenced by a root table.
        //D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
        //cbvHeapDesc.NumDescriptors = 1;
        //cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        //cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        //THROW_IF_FAILED(_device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&_cbvHeap)));
    }

    // Create frame resources.
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());

        // Create a RTV for each frame.
        for (UINT n = 0; n < FrameCount; n++)
        {
            THROW_IF_FAILED(_swapChain->GetBuffer(n, IID_PPV_ARGS(&_renderTargets[n])));
            _device->CreateRenderTargetView(_renderTargets[n].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, _rtvDescriptorSize);

            THROW_IF_FAILED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocators[n])));
            THROW_IF_FAILED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&_bundleAllocators[n])));
        }
    }
}

void Graphics::LoadAssets()
{
    // Create an empty root signature.
    {
        D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

        // This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
        featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

        if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
        {
            featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
        }

        CD3DX12_DESCRIPTOR_RANGE1 ranges[2];
        ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
        ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

        CD3DX12_ROOT_PARAMETER1 rootParameters[2];
        rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
        rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_VERTEX);

        D3D12_STATIC_SAMPLER_DESC sampler = {};
        sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
        sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
        sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
        sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
        sampler.MipLODBias = 0;
        sampler.MaxAnisotropy = 0;
        sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
        sampler.MinLOD = 0.0f;
        sampler.MaxLOD = D3D12_FLOAT32_MAX;
        sampler.ShaderRegister = 0;
        sampler.RegisterSpace = 0;
        sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

        // Allow input layout and deny unnecessary access to certain pipeline stages.
        D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;// |
            //D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
            //D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
            //D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
            //D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

        CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
        rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, &sampler, rootSignatureFlags);

        ComPtr<ID3DBlob> signature;
        ComPtr<ID3DBlob> error;
        THROW_IF_FAILED(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));
        THROW_IF_FAILED(_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature)));
    }

    // Create the pipeline state, which includes compiling and loading shaders.
    {
        ComPtr<ID3DBlob> vertexShader;
        ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
        // Enable better shader debugging with the graphics debugging tools.
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags = 0;
#endif

        // FIXME: paths
        THROW_IF_FAILED(D3DCompileFromFile(L"D:\\FRT\\FRTEngine\\FRTEngine\\shaders.hlsl", nullptr, nullptr, "VShader", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
        THROW_IF_FAILED(D3DCompileFromFile(L"D:\\FRT\\FRTEngine\\FRTEngine\\shaders.hlsl", nullptr, nullptr, "PShader", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));

        // Define the vertex input layout.
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
            //{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        // Describe and create the graphics pipeline state object (PSO).
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
        psoDesc.pRootSignature = _rootSignature.Get();
        psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
        psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilState.DepthEnable = FALSE;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 2;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count = 1;
        THROW_IF_FAILED(_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&_pipelineState)));
    }

    // Create the vertex buffer.
    {
        // Define the geometry for a triangle.
        Vertex triangleVertices[] =
        {
            // front side
            { { -.125f, -.125f * _aspectRatio, 0.25f }, { 0.0f, 0.0f } },  //  ∟  // 0
            { { -.125f, 0.125f * _aspectRatio, 0.25f }, { 0.0f, 1.0f } },  //  Γ  // 1
            { { 0.125f, -.125f * _aspectRatio, 0.25f }, { 1.0f, 0.0f } },  //  ┘  // 2
            { { 0.125f, 0.125f * _aspectRatio, 0.25f }, { 1.0f, 1.0f } },  //  ┐  // 3

            // right side
            { { 0.125f, -.125f * _aspectRatio, 0.25f }, { 0.0f, 0.0f } },  //  ∟  // 4
            { { 0.125f, 0.125f * _aspectRatio, 0.25f }, { 0.0f, 1.0f } },  //  Γ  // 5
            { { 0.125f, -.125f * _aspectRatio, 0.50f }, { 1.0f, 0.0f } },  //  ┘  // 6
            { { 0.125f, 0.125f * _aspectRatio, 0.50f }, { 1.0f, 1.0f } },  //  ┐  // 7

            //{ { 0.3f, -0.05f * _aspectRatio, 0.25f }, { 1.0f, 0.0f } },  //  ┘  // 6
            //{ { 0.3f, 0.3f * _aspectRatio, 0.25f }, { 1.0f, 1.0f } },  //  ┐  // 7

            // left side
            //{ { -0.05f, -0.05f * _aspectRatio, 0.25f }, { 0.0f, 0.0f } },  //  ∟  // 8
            //{ { -0.05f, 0.3f * _aspectRatio, 0.25f }, { 0.0f, 1.0f } },  //  Γ  // 9

            { { -.125f, -.125f * _aspectRatio, 0.50f }, { 0.0f, 0.0f } },  //  ∟  // 8
            { { -.125f, 0.125f * _aspectRatio, 0.50f }, { 0.0f, 1.0f } },  //  Γ  // 9
            { { -.125f, -.125f * _aspectRatio, 0.25f }, { 1.0f, 0.0f } },  //  ┘  // 10
            { { -.125f, 0.125f * _aspectRatio, 0.25f }, { 1.0f, 1.0f } },  //  ┐  // 11

            // back side
            { { 0.125f, -.125f * _aspectRatio, 0.50f }, { 0.0f, 0.0f } },  //  ∟  // 12
            { { 0.125f, 0.125f * _aspectRatio, 0.50f }, { 0.0f, 1.0f } },  //  Γ  // 13
            { { -.125f, -.125f * _aspectRatio, 0.50f }, { 1.0f, 0.0f } },  //  ┘  // 14
            { { -.125f, 0.125f * _aspectRatio, 0.50f }, { 1.0f, 1.0f } },  //  ┐  // 15

            // top side
            { { -.125f, 0.125f * _aspectRatio, 0.25f }, { 0.0f, 0.0f } },  //  ∟  // 16
            
            { { -.125f, 0.125f * _aspectRatio, 0.50f }, { 0.0f, 1.0f } },  //  Γ  // 17
            //{ { -0.05f, 0.3f * _aspectRatio, 0.25f }, { 0.0f, 1.0f } },  //  Γ  // 17
            
            { { 0.125f, 0.125f * _aspectRatio, 0.25f }, { 1.0f, 0.0f } },  //  ┘  // 18

            { { 0.125f, 0.125f * _aspectRatio, 0.50f }, { 1.0f, 1.0f } },  //  ┐  // 19
            //{ { 0.3f, 0.3f * _aspectRatio, 0.25f }, { 1.0f, 1.0f } },  //  ┐  // 19

            // bottom side (mirrored)
            { { 0.125f, -.125f * _aspectRatio, 0.25f }, { 0.0f, 0.0f } },  //  ∟  // 20
            //{ { -0.05f, -0.05f * _aspectRatio, 0.00f }, { 0.0f, 0.0f } },  //  ∟  // 20

            { { 0.125f, -.125f * _aspectRatio, 0.50f }, { 0.0f, 1.0f } },  //  Γ  // 21
            //{ { -0.05f, -0.05f * _aspectRatio, 0.25f }, { 0.0f, 1.0f } },  //  Γ  // 21

            { { -.125f, -.125f * _aspectRatio, 0.25f }, { 1.0f, 0.0f } },  //  ┘  // 22
            //{ { 0.3f, -0.05f * _aspectRatio, 0.00f }, { 1.0f, 0.0f } },  //  ┘  // 22

            { { -.125f, -.125f * _aspectRatio, 0.50f }, { 1.0f, 1.0f } },  //  ┐  // 23
            //{ { 0.3f, -0.05f * _aspectRatio, 0.25f }, { 1.0f, 1.0f } },  //  ┐  // 23
        };

        const unsigned char indices[] =
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


        const UINT vertexBufferSize = sizeof(triangleVertices);
        const UINT indexBufferSize = sizeof(indices);
        Logger::DebugLogInfo("Index buffer size: " + std::to_string(indexBufferSize));

        // Note: using upload heaps to transfer static data like vert buffers is not 
        // recommended. Every time the GPU needs it, the upload heap will be marshalled 
        // over. Please read up on Default Heap usage. An upload heap is used here for 
        // code simplicity and because there are very few verts to actually transfer.
        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
        auto desc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
        THROW_IF_FAILED(_device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&_vertexBuffer)));

        // Copy the triangle data to the vertex buffer.
        UINT8* pVertexDataBegin;
        CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
        THROW_IF_FAILED(_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
        memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
        _vertexBuffer->Unmap(0, nullptr);

        // Initialize the vertex buffer view.
        _vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
        _vertexBufferView.StrideInBytes = sizeof(Vertex);
        _vertexBufferView.SizeInBytes = vertexBufferSize;

        // index buffer
        CD3DX12_HEAP_PROPERTIES heapProps_(D3D12_HEAP_TYPE_UPLOAD);
        auto desc_ = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
        THROW_IF_FAILED(_device->CreateCommittedResource(
            &heapProps_,
            D3D12_HEAP_FLAG_NONE,
            &desc_,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&_indexBuffer)));

        // Copy the triangle data to the vertex buffer.
        UINT8* pIndexDataBegin;
        CD3DX12_RANGE readRangeIndex(0, 0);        // We do not intend to read from this resource on the CPU.
        THROW_IF_FAILED(_indexBuffer->Map(0, &readRangeIndex, reinterpret_cast<void**>(&pIndexDataBegin)));
        memcpy(pIndexDataBegin, indices, sizeof(indices));
        _indexBuffer->Unmap(0, nullptr);

        // Initialize the vertex buffer view.
        _indexBufferView.BufferLocation = _indexBuffer->GetGPUVirtualAddress();
        //_indexBufferView.StrideInBytes = sizeof(Vertex);
        _indexBufferView.SizeInBytes = indexBufferSize;
    }

    // Create and record the bundle.
    {
        THROW_IF_FAILED(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_BUNDLE, _bundleAllocators[_frameIndex].Get(), _pipelineState.Get(), IID_PPV_ARGS(&_bundle)));
        _bundle->SetGraphicsRootSignature(_rootSignature.Get());
        _bundle->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        _bundle->IASetVertexBuffers(0, 1, &_vertexBufferView);
        _bundle->IASetIndexBuffer(&_indexBufferView);
        for (UINT i = 0; i < _indexBufferView.SizeInBytes / (3 * sizeof(unsigned char)); i++)
        {
            Logger::DebugLogInfo("Draw triangle #" + std::to_string(i));
            _bundle->DrawIndexedInstanced(3, 1, i * 3, 0, 0);
        }
        //_bundle->DrawIndexedInstanced(3, 1, 0, 0, 0);
        //_bundle->DrawIndexedInstanced(3, 1, 3, 0, 0);
        //_bundle->DrawInstanced(3, 1, 0, 0);
        //_bundle->DrawInstanced(3, 1, 3, 3);
        THROW_IF_FAILED(_bundle->Close());
    }

    CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(_srvHeap->GetCPUDescriptorHandleForHeapStart(), 0, _cbvSrvDescriptorSize);

    // Create the command list.
    THROW_IF_FAILED(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocators[_frameIndex].Get(), _pipelineState.Get(), IID_PPV_ARGS(&_commandList)));

    // Note: ComPtr's are CPU objects but this resource needs to stay in scope until
    // the command list that references it has finished executing on the GPU.
    // We will flush the GPU at the end of this method to ensure the resource is not
    // prematurely destroyed.
    ComPtr<ID3D12Resource> textureUploadHeap;

    // Create the texture.
    {
        // Describe and create a Texture2D.
        D3D12_RESOURCE_DESC textureDesc = {};
        textureDesc.MipLevels = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.Width = TextureWidth;
        textureDesc.Height = TextureHeight;
        textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        textureDesc.DepthOrArraySize = 1;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

        const CD3DX12_HEAP_PROPERTIES textureHeapProps(D3D12_HEAP_TYPE_DEFAULT);

        THROW_IF_FAILED(_device->CreateCommittedResource(
            &textureHeapProps,
            D3D12_HEAP_FLAG_NONE,
            &textureDesc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&_texture)));

        const UINT64 uploadBufferSize = GetRequiredIntermediateSize(_texture.Get(), 0, 1);

        // Create the GPU upload buffer.
        const CD3DX12_HEAP_PROPERTIES textureUploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);
        const CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

        THROW_IF_FAILED(_device->CreateCommittedResource(
            &textureUploadHeapProps,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&textureUploadHeap)));

        // Copy data to the intermediate upload heap and then schedule a copy 
        // from the upload heap to the Texture2D.
        std::vector<UINT8> texture = GenerateTextureData();

        D3D12_SUBRESOURCE_DATA textureData = {};
        textureData.pData = &texture[0];
        textureData.RowPitch = TextureWidth * TexturePixelSize;
        textureData.SlicePitch = textureData.RowPitch * TextureHeight;

        UpdateSubresources(_commandList.Get(), _texture.Get(), textureUploadHeap.Get(), 0, 0, 1, &textureData);
        const CD3DX12_RESOURCE_BARRIER depthStencilTextureDesc = CD3DX12_RESOURCE_BARRIER::Transition(_texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        _commandList->ResourceBarrier(1, &depthStencilTextureDesc);

        // Describe and create a SRV for the texture.
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        _device->CreateShaderResourceView(_texture.Get(), &srvDesc, srvHandle);
        srvHandle.Offset(1, _cbvSrvDescriptorSize);
    }

    // Create the constant buffer.
    {
        const UINT constantBufferSize = sizeof(SceneConstantBuffer);    // CB size is required to be 256-byte aligned.

        const CD3DX12_HEAP_PROPERTIES cbUploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);
        const CD3DX12_RESOURCE_DESC cbResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize);
        THROW_IF_FAILED(_device->CreateCommittedResource(
            &cbUploadHeapProps,
            D3D12_HEAP_FLAG_NONE,
            &cbResourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&_constantBuffer)));

        // Describe and create a constant buffer view.
        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
        cbvDesc.BufferLocation = _constantBuffer->GetGPUVirtualAddress();
        cbvDesc.SizeInBytes = constantBufferSize;
        _device->CreateConstantBufferView(&cbvDesc, srvHandle);
        srvHandle.Offset(1, _cbvSrvDescriptorSize);

        // Map and initialize the constant buffer. We don't unmap this until the
        // app closes. Keeping things mapped for the lifetime of the resource is okay.
        CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
        THROW_IF_FAILED(_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&_pCbvDataBegin)))
        memcpy(_pCbvDataBegin, &_constantBufferData, sizeof(_constantBufferData));
    }

    //Command lists are created in the recording state, but there is nothing
    //to record yet. The main loop expects it to be closed, so close it now.
    THROW_IF_FAILED(_commandList->Close());

    // Close the command list and execute it to begin the initial GPU setup.
    ID3D12CommandList* ppCommandLists[] = { _commandList.Get() };
    _commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Create synchronization objects and wait until assets have been uploaded to the GPU.
    {
        THROW_IF_FAILED(_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));
        _fenceValues[_frameIndex]++;

        // Create an event handle to use for frame synchronization.
        _fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (_fenceEvent == nullptr)
        {
            THROW_IF_FAILED(HRESULT_FROM_WIN32(GetLastError()));
        }

        // Wait for the command list to execute; we are reusing the same command 
        // list in our main loop but for now, we just want to wait for setup to 
        // complete before continuing.
        WaitForPreviousFrame();
    }
}

std::vector<UINT8> Graphics::GenerateTextureData()
{
    const UINT rowPitch = TextureWidth * TexturePixelSize;
    const UINT cellPitch = rowPitch >> 3;        // The width of a cell in the checkboard texture.
    const UINT cellHeight = TextureWidth >> 3;    // The height of a cell in the checkerboard texture.
    const UINT textureSize = rowPitch * TextureHeight;

    std::vector<UINT8> data(textureSize);
    UINT8* pData = &data[0];

    for (UINT n = 0; n < textureSize; n += TexturePixelSize)
    {
        UINT x = n % rowPitch;
        UINT y = n / rowPitch;
        UINT i = x / cellPitch;
        UINT j = y / cellHeight;

        if (i % 2 == j % 2)
        {
            pData[n] = 0x00;        // R
            pData[n + 1] = 0x00;    // G
            pData[n + 2] = 0x00;    // B
            pData[n + 3] = 0xff;    // A
        }
        else
        {
            pData[n] = 0xff;        // R
            pData[n + 1] = 0xff;    // G
            pData[n + 2] = 0xff;    // B
            pData[n + 3] = 0xff;    // A
        }
    }

    return data;
}

void Graphics::PopulateCommandList()
{
    // Command list allocators can only be reset when the associated 
    // command lists have finished execution on the GPU; apps should use 
    // fences to determine GPU execution progress.
    THROW_IF_FAILED(_commandAllocators[_frameIndex]->Reset());

    // However, when ExecuteCommandList() is called on a particular command 
    // list, that command list can then be reset at any time and must be before 
    // re-recording.
    THROW_IF_FAILED(_commandList->Reset(_commandAllocators[_frameIndex].Get(), _pipelineState.Get()));

    // Set necessary state.
    _commandList->SetGraphicsRootSignature(_rootSignature.Get());

    ID3D12DescriptorHeap* ppHeaps[] = { _srvHeap.Get() };
    _commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(_srvHeap->GetGPUDescriptorHandleForHeapStart(), 0, _cbvSrvDescriptorSize);
    _commandList->SetGraphicsRootDescriptorTable(0, srvHandle);
    srvHandle.Offset(1, _cbvSrvDescriptorSize);
    _commandList->SetGraphicsRootDescriptorTable(1, srvHandle);
    
    _commandList->RSSetViewports(1, &_viewport);
    _commandList->RSSetScissorRects(1, &_scissorRect);
    
    // Indicate that the back buffer will be used as a render target.
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    _commandList->ResourceBarrier(1, &barrier);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart(), _frameIndex, _rtvDescriptorSize);
    _commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Record commands.
    const float clearColor[] = { 247. / 255., 202. / 255., 24. / 255., 1. };
    _commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // Execute the commands stored in the bundle.
    _commandList->ExecuteBundle(_bundle.Get());

    //_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //_commandList->IASetVertexBuffers(0, 1, &_vertexBufferView);
    //_commandList->DrawInstanced(3, 1, 0, 0);
    //_commandList->DrawIndexedInstanced(3, )


    // Indicate that the back buffer will now be used to present.
    barrier = CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    _commandList->ResourceBarrier(1, &barrier);

    THROW_IF_FAILED(_commandList->Close());
}

void Graphics::WaitForPreviousFrame()
{
    // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
    // This is code implemented as such for simplicity. More advanced samples 
    // illustrate how to use fences for efficient resource usage.

    // Signal and increment the fence value.
    const UINT64 fence = _fenceValues[_frameIndex];
    THROW_IF_FAILED(_commandQueue->Signal(_fence.Get(), fence));

    // Wait until the previous frame is finished.
    if (_fence->GetCompletedValue() < fence)
    {
        THROW_IF_FAILED(_fence->SetEventOnCompletion(fence, _fenceEvent));
        WaitForSingleObject(_fenceEvent, INFINITE);
    }

    _frameIndex = _swapChain->GetCurrentBackBufferIndex();
    _fenceValues[_frameIndex] = fence + 1;
}

}

