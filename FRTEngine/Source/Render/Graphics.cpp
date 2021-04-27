#include "Graphics.h"
#include "Exception.h"
#include "Debug/Debug.h"
#include "Event.h"
#include "App.h"
#include "Window.h"
#include "MathLib.h"
#include "pix3.h"
#include "Utils/Logger/Logger.h"
#include "Render/VertexBuffer.h"
#include "Render/ConstantBuffer.h"

#include <string>
#include <Tools\DXHelper.h>
#include <filesystem>
#include <string>
#include "IndexBuffer.h"

const float frt::Graphics::SpacingInterval = 1.f;


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

inline HRESULT ReadDataFromFile(LPCWSTR filename, byte** data, UINT* size)
{
    using namespace Microsoft::WRL;

#if WINVER >= _WIN32_WINNT_WIN8
    CREATEFILE2_EXTENDED_PARAMETERS extendedParams = {};
    extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
    extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
    extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
    extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
    extendedParams.lpSecurityAttributes = nullptr;
    extendedParams.hTemplateFile = nullptr;

    frt::Logger::DebugLogInfo("Current dir: " + std::string( (char*)(std::filesystem::current_path().c_str()) ));

    Wrappers::FileHandle file(CreateFile2(filename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, &extendedParams));
#else
    Wrappers::FileHandle file(CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN | SECURITY_SQOS_PRESENT | SECURITY_ANONYMOUS, nullptr));
#endif
    if (file.Get() == INVALID_HANDLE_VALUE)
    {
        throw std::exception();
    }

    FILE_STANDARD_INFO fileInfo = {};
    if (!GetFileInformationByHandleEx(file.Get(), FileStandardInfo, &fileInfo, sizeof(fileInfo)))
    {
        throw std::exception();
    }

    if (fileInfo.EndOfFile.HighPart != 0)
    {
        throw std::exception();
    }

    *data = reinterpret_cast<byte*>(malloc(fileInfo.EndOfFile.LowPart));
    *size = fileInfo.EndOfFile.LowPart;

    if (!ReadFile(file.Get(), *data, fileInfo.EndOfFile.LowPart, nullptr, nullptr))
    {
        throw std::exception();
    }

    return S_OK;
}

namespace frt
{
using DirectX::XMFLOAT3;

Graphics::Graphics(Window* owner, HWND hWindow) :
    _owner(owner),
    _frameIndex(0),
    _viewport(0.0f, 0.0f, static_cast<float>(1280), static_cast<float>(720)),
    _scissorRect(static_cast<LONG>(0), static_cast<LONG>(0), static_cast<LONG>(1280), static_cast<LONG>(720)),
    _fenceValue(0),
    _frameCounter(0),
    _currentFrameResourceIndex(0),
    _rtvDescriptorSize(0),
    _cbvSrvDescriptorSize(0),
    currentRotation{ DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0) },
    currentTranslation{ DirectX::XMMatrixTranslation(0.f, 0.f, 0.f) }
{
    App::GetInstance()->GetWorld()->Reserve(MaterialCount);
    _fenceValues.resize(FrameCount * MaterialCount);
    //_hWindow = hWindow;
    Init(hWindow);
}

Graphics::~Graphics()
{
    //delete _mesh;
    Destroy();
}

void Graphics::Init(HWND hWindow)
{
    _camera.Init({ (ColumnCount / 2.f) * SpacingInterval - (SpacingInterval / 2.f), 3, 25 });
    _camera.SetMoveSpeed(SpacingInterval * 2.f);

    LoadPipeline(hWindow);
    LoadAssets();
}

void Graphics::Update()
{
    //CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart(), _frameIndex, _rtvDescriptorSize);
    //_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    //const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    //_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    if (_frameCounter == 500)
    {
        // Update window text with FPS value.
        wchar_t fps[64];
        //swprintf_s(fps, L"%ufps", m_timer.GetFramesPerSecond());
        //SetCustomWindowText(fps);
        _frameCounter = 0;
    }

    _frameCounter++;

    // Get current GPU progress against submitted workload. Resources still scheduled 
    // for GPU execution cannot be modified or else undefined behavior will result.
    const UINT64 lastCompletedFence = _fence->GetCompletedValue();

    // Move to the next frame resource.
    _currentFrameResourceIndex = (_currentFrameResourceIndex + 1) % FrameCount;
    //_currentFrameResource = _frameResources[_currentFrameResourceIndex];

    // Make sure that this frame resource isn't still in use by the GPU.
    // If it is, wait for it to complete.
    //if (_currentFrameResource->_fenceValue != 0 && _currentFrameResource->_fenceValue > lastCompletedFence)
    if (_fenceValues[_currentFrameResourceIndex] != 0 && _fenceValues[_currentFrameResourceIndex] > lastCompletedFence)
    {
        THROW_IF_FAILED(_fence->SetEventOnCompletion(_fenceValues[_currentFrameResourceIndex], _fenceEvent));
        WaitForSingleObject(_fenceEvent, INFINITE);
    }

    _camera.Update(1. / 60.);
    //_camera.Update(static_cast<float>(_timer.GetElapsedSeconds()));
}

void Graphics::Render()
{
    PIXBeginEvent(_commandQueue.Get(), 0, L"Render");

    // Record all the commands we need to render the scene into the command list.
    PopulateCommandList();

    // Execute the command list.
    ID3D12CommandList* ppCommandLists[] = { _commandList.Get() };
    _commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    PIXEndEvent(_commandQueue.Get());

    // Present and update the frame index for the next frame.
    THROW_IF_FAILED(_swapChain->Present(1, 0));
    _frameIndex = _swapChain->GetCurrentBackBufferIndex();

    // Signal and increment the fence value.
    //_currentFrameResource->_fenceValue = _fenceValue;
    _fenceValues[_currentFrameResourceIndex] = _fenceValue;
    THROW_IF_FAILED(_commandQueue->Signal(_fence.Get(), _fenceValue));
    _fenceValue++;
}

void Graphics::Destroy()
{
    // Ensure that the GPU is no longer referencing resources that are about to be
    // cleaned up by the destructor.
    {
        const UINT64 fence = _fenceValue;
        //const UINT64 fence = _fenceValues[_currentFrameResourceIndex];
        const UINT64 lastCompletedFence = _fence->GetCompletedValue();

        // Signal and increment the fence value.
        THROW_IF_FAILED(_commandQueue->Signal(_fence.Get(), _fenceValue));
        _fenceValue++;

        // Wait until the previous frame is finished.
        if (lastCompletedFence < fence)
        {
            THROW_IF_FAILED(_fence->SetEventOnCompletion(fence, _fenceEvent));
            WaitForSingleObject(_fenceEvent, INFINITE);
        }
    }
}

void Graphics::BeforeFirstTick()
{
    ComPtr<ID3D12Resource> textureUploadHeap;
    ComPtr<ID3D12Resource> materialsUploadHeap;

    // Create the texture.
    {
        // All of these materials use the same texture desc.
        D3D12_RESOURCE_DESC textureDesc = {};
        textureDesc.MipLevels = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.Width = Mesh::TextureWidth;
        textureDesc.Height = Mesh::TextureHeight;
        textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        textureDesc.DepthOrArraySize = 1;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

        {
            // Generate texture data
            std::vector<std::vector<UINT8>> textures(MaterialCount);
            for (UINT i = 0; i < MaterialCount; ++i)
            {
                const CD3DX12_HEAP_PROPERTIES textureHeapProps(D3D12_HEAP_TYPE_DEFAULT);

                THROW_IF_FAILED(_device->CreateCommittedResource(
                    &textureHeapProps,
                    D3D12_HEAP_FLAG_NONE,
                    &textureDesc,
                    D3D12_RESOURCE_STATE_COPY_DEST,
                    nullptr,
                    IID_PPV_ARGS(&_textures[i])));

                NAME_D3D12_OBJECT_INDEXED(_textures, i);

                textures[i] = Mesh::GenerateTextureData(0x222222ff, 0xffffffff);
            }


            // Upload texture data to the default heap resources.
            {
                const UINT subresourceCount = textureDesc.DepthOrArraySize * textureDesc.MipLevels;
                const UINT64 uploadBufferStep = GetRequiredIntermediateSize(_textures[0].Get(), 0, subresourceCount); // All of our textures are the same size in this case.
                const UINT64 uploadBufferSize = uploadBufferStep * MaterialCount;
                const CD3DX12_HEAP_PROPERTIES textureUploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);
                const CD3DX12_RESOURCE_DESC textureUploadDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
                THROW_IF_FAILED(_device->CreateCommittedResource(
                    &textureUploadHeapProps,
                    D3D12_HEAP_FLAG_NONE,
                    &textureUploadDesc,
                    D3D12_RESOURCE_STATE_GENERIC_READ,
                    nullptr,
                    IID_PPV_ARGS(&materialsUploadHeap)));

                for (int i = 0; i < MaterialCount; ++i)
                {
                    // Copy data to the intermediate upload heap and then schedule 
                    // a copy from the upload heap to the appropriate texture.
                    D3D12_SUBRESOURCE_DATA textureData = {};
                    textureData.pData = &textures[i][0];
                    textureData.RowPitch = static_cast<LONG_PTR>((4U * textureDesc.Width));
                    textureData.SlicePitch = textureData.RowPitch * textureDesc.Height;

                    auto bar_ = CD3DX12_RESOURCE_BARRIER::Transition(_textures[i].Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
                    UpdateSubresources(_commandList.Get(), _textures[i].Get(), materialsUploadHeap.Get(), i * uploadBufferStep, 0, subresourceCount, &textureData);
                    _commandList->ResourceBarrier(1, &bar_);
                }
            }
        }

        // Describe and create a sampler.
        D3D12_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
        _device->CreateSampler(&samplerDesc, _samplerHeap->GetCPUDescriptorHandleForHeapStart());

        CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(_cbvSrvHeap->GetCPUDescriptorHandleForHeapStart(), 0, _cbvSrvDescriptorSize);

        // Create SRVs for each cube material.
        for (int i = 0; i < MaterialCount; ++i)
        {
            D3D12_SHADER_RESOURCE_VIEW_DESC materialSrvDesc = {};
            materialSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            materialSrvDesc.Format = textureDesc.Format;
            materialSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            materialSrvDesc.Texture2D.MipLevels = 1;
            _device->CreateShaderResourceView(_textures[i].Get(), &materialSrvDesc, srvHandle);

            srvHandle.Offset(_cbvSrvDescriptorSize);
        }

    }

    // Create the depth stencil view.
    {
        D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
        depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

        D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
        depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
        depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
        depthOptimizedClearValue.DepthStencil.Stencil = 0;

        const CD3DX12_HEAP_PROPERTIES dsvHeapProps(D3D12_HEAP_TYPE_DEFAULT);
        // Performance tip: Deny shader resource access to resources that don't need shader resource views.
        const CD3DX12_RESOURCE_DESC dsvUploadDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT,
                                                                                 1280, 720, 1, 0, 1, 0,
                                                                                 D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

        THROW_IF_FAILED(_device->CreateCommittedResource(
            &dsvHeapProps,
            D3D12_HEAP_FLAG_NONE,
            &dsvUploadDesc,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &depthOptimizedClearValue,
            IID_PPV_ARGS(&_depthStencil)
        ));

        NAME_D3D12_OBJECT(_depthStencil);

        _device->CreateDepthStencilView(_depthStencil.Get(), &depthStencilDesc, _dsvHeap->GetCPUDescriptorHandleForHeapStart());
    }

    // Close the command list and execute it to begin the initial GPU setup.
    THROW_IF_FAILED(_commandList->Close());
    ID3D12CommandList* ppCommandLists[] = { _commandList.Get() };
    _commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Create synchronization objects and wait until assets have been uploaded to the GPU.
    {
        THROW_IF_FAILED(_device->CreateFence(_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));
        _fenceValue++;

        // Create an event handle to use for frame synchronization.
        _fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (_fenceEvent == nullptr)
        {
            THROW_IF_FAILED(HRESULT_FROM_WIN32(GetLastError()));
        }

        // Wait for the command list to execute; we are reusing the same command 
        // list in our main loop but for now, we just want to wait for setup to 
        // complete before continuing.

        // Signal and increment the fence value.
        const UINT64 fenceToWaitFor = _fenceValue;
        THROW_IF_FAILED(_commandQueue->Signal(_fence.Get(), fenceToWaitFor));
        _fenceValue++;

        // Wait until the fence is completed.
        THROW_IF_FAILED(_fence->SetEventOnCompletion(fenceToWaitFor, _fenceEvent));
        WaitForSingleObject(_fenceEvent, INFINITE);
    }

    for (UINT i = 0; i < FrameCount; i++)
    {
        THROW_IF_FAILED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocators[i])));
    }
}

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
    NAME_D3D12_OBJECT(_commandQueue);

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

        // Describe and create a depth stencil view (DSV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
        dsvHeapDesc.NumDescriptors = 1;
        dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        THROW_IF_FAILED(_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&_dsvHeap)));

        // Describe and create a shader resource view (SRV) and constant 
        // buffer view (CBV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC cbvSrvHeapDesc = {};
        cbvSrvHeapDesc.NumDescriptors =
            FrameCount * RowCount * ColumnCount +    // FrameCount frames * RowCount * ColumnCount.
            MaterialCount + 1;                       // CityMaterialCount + 1 for the SRVs.
        cbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        cbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        THROW_IF_FAILED(_device->CreateDescriptorHeap(&cbvSrvHeapDesc, IID_PPV_ARGS(&_cbvSrvHeap)));
        NAME_D3D12_OBJECT(_cbvSrvHeap);

        // Describe and create a sampler descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc = {};
        samplerHeapDesc.NumDescriptors = 1;
        samplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
        samplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        THROW_IF_FAILED(_device->CreateDescriptorHeap(&samplerHeapDesc, IID_PPV_ARGS(&_samplerHeap)));

        _rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        _cbvSrvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    THROW_IF_FAILED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocator)));
}

void Graphics::LoadAssets()
{
    // Note: ComPtr's are CPU objects but these resources need to stay in scope until
    // the command list that references them has finished executing on the GPU.
    // We will flush the GPU at the end of this method to ensure the resources are not
    // prematurely destroyed.
    //ComPtr<ID3D12Resource> indexBufferUploadHeap;
    ComPtr<ID3D12Resource> textureUploadHeap;
    ComPtr<ID3D12Resource> materialsUploadHeap;

    // Create the root signature.
    {
        D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

        // This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
        featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

        if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
        {
            featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
        }

        CD3DX12_DESCRIPTOR_RANGE1 ranges[3];
        ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1 + MaterialCount, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
        ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
        ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

        CD3DX12_ROOT_PARAMETER1 rootParameters[4];
        rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
        rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
        rootParameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_VERTEX);
        rootParameters[3].InitAsConstants(1, 0, 0, D3D12_SHADER_VISIBILITY_PIXEL);

        // Allow input layout and deny unnecessary access to certain pipeline stages.
        D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;// |
            //D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
            //D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
            //D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
            //D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

        CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
        rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);

        ComPtr<ID3DBlob> signature;
        ComPtr<ID3DBlob> error;
        THROW_IF_FAILED(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));
        THROW_IF_FAILED(_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature)));
        NAME_D3D12_OBJECT(_rootSignature);
    }

    // Create the pipeline state, which includes compiling and loading shaders.
    {

#if defined(_DEBUG)
        // Enable better shader debugging with the graphics debugging tools.
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags = 0;
#endif

        UINT8* pVertexShaderData;
        UINT8* pPixelShaderData;
        UINT vertexShaderDataLength;
        UINT pixelShaderDataLength;

        // FIXME: paths
        THROW_IF_FAILED(ReadDataFromFile(L"D:\\FRT\\FRTEngine\\Binaries\\x64\\Debug\\Tetris3D\\VertexShader.cso", &pVertexShaderData, &vertexShaderDataLength));
        THROW_IF_FAILED(ReadDataFromFile(L"D:\\FRT\\FRTEngine\\Binaries\\x64\\Debug\\Tetris3D\\PixelShader.cso", &pPixelShaderData, &pixelShaderDataLength));

        // Define the vertex input layout.
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            //{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        CD3DX12_RASTERIZER_DESC rasterizerStateDesc(D3D12_DEFAULT);
        rasterizerStateDesc.CullMode = D3D12_CULL_MODE_NONE;

        // Describe and create the graphics pipeline state object (PSO).
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
        psoDesc.pRootSignature = _rootSignature.Get();
        psoDesc.VS = CD3DX12_SHADER_BYTECODE(pVertexShaderData, vertexShaderDataLength);
        psoDesc.PS = CD3DX12_SHADER_BYTECODE(pPixelShaderData, pixelShaderDataLength);
        psoDesc.RasterizerState = rasterizerStateDesc;
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
        psoDesc.SampleDesc.Count = 1;

        THROW_IF_FAILED(_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&_pipelineState)));
        NAME_D3D12_OBJECT(_pipelineState);

        delete pVertexShaderData;
        delete pPixelShaderData;
    }

    THROW_IF_FAILED(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator.Get(), nullptr, IID_PPV_ARGS(&_commandList)));
    //THROW_IF_FAILED(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator.Get(), nullptr, IID_PPV_ARGS(&_commandList)));
    NAME_D3D12_OBJECT(_commandList);

    // Create render target views (RTVs).
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < FrameCount; i++)
    {
        THROW_IF_FAILED(_swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i])));
        _device->CreateRenderTargetView(_renderTargets[i].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, _rtvDescriptorSize);

        NAME_D3D12_OBJECT_INDEXED(_renderTargets, i);
    }

    _indicesNum = Mesh::GetIndexDataSize();
}

void Graphics::PopulateCommandList()
{
    // Command list allocators can only be reset when the associated
    // command lists have finished execution on the GPU; apps should use
    // fences to determine GPU execution progress.
    //THROW_IF_FAILED(_currentFrameResource->_commandAllocator->Reset());
    THROW_IF_FAILED(_commandAllocators[_currentFrameResourceIndex]->Reset());

    // However, when ExecuteCommandList() is called on a particular command
    // list, that command list can then be reset at any time and must be before
    // re-recording.
    //THROW_IF_FAILED(_commandList->Reset(_currentFrameResource->_commandAllocator.Get(), _pipelineState.Get()));
    THROW_IF_FAILED(_commandList->Reset(_commandAllocators[_currentFrameResourceIndex].Get(), _pipelineState.Get()));

    // Set necessary state.
    _commandList->SetGraphicsRootSignature(_rootSignature.Get());

    ID3D12DescriptorHeap* ppHeaps[] = { _cbvSrvHeap.Get(), _samplerHeap.Get() };
    _commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    _commandList->RSSetViewports(1, &_viewport);
    _commandList->RSSetScissorRects(1, &_scissorRect);

    // Indicate that the back buffer will be used as a render target.
    auto bar3 = CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    _commandList->ResourceBarrier(1, &bar3);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart(), _frameIndex, _rtvDescriptorSize);
    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(_dsvHeap->GetCPUDescriptorHandleForHeapStart());
    _commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

    // Record commands.
    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    _commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    _commandList->ClearDepthStencilView(_dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    PIXBeginEvent(_commandList.Get(), 0, L"Draw");
    if (bUseBundles)
    {
        // Execute the prebuilt bundle.
        //_commandList->ExecuteBundle(pFrameResource->_bundle.Get());
    }
    else
    {
        // Populate a new command list.
        _commandList->SetGraphicsRootSignature(_rootSignature.Get());

        ID3D12DescriptorHeap* ppHeaps[] = { _cbvSrvHeap.Get(), _samplerHeap.Get() };
        _commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
        _commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        App::GetInstance()->GetWorld()->PopulateCommandList();

        _commandList->SetGraphicsRootDescriptorTable(0, _cbvSrvHeap->GetGPUDescriptorHandleForHeapStart());
        _commandList->SetGraphicsRootDescriptorTable(1, _samplerHeap->GetGPUDescriptorHandleForHeapStart());

        // Calculate the descriptor offset due to multiple frame resources.
        // (_materialCount + 1) SRVs + how many CBVs we have currently.
        UINT frameResourceDescriptorOffset = (MaterialCount + 1) + (_currentFrameResourceIndex);// *RowCount* ColumnCount);
        CD3DX12_GPU_DESCRIPTOR_HANDLE cbvSrvHandle(_cbvSrvHeap->GetGPUDescriptorHandleForHeapStart(), frameResourceDescriptorOffset, _cbvSrvDescriptorSize);

        //for (UINT i = 0; i < RowCount; i++)
        {
            //for (UINT j = 0; j < ColumnCount; j++)
            {
                // Set the cube's root constant for dynamically indexing into the material array.
                //_commandList->SetGraphicsRoot32BitConstant(3, (i * ColumnCount) + j, 0);
                _commandList->SetGraphicsRoot32BitConstant(3, 0, 0);

                // Set this cube's CBV table and move to the next descriptor.
                _commandList->SetGraphicsRootDescriptorTable(2, cbvSrvHandle);
                cbvSrvHandle.Offset(_cbvSrvDescriptorSize);

                _commandList->DrawIndexedInstanced(_indicesNum, 1, 0, 0, 0);
            }
        }
    }
    PIXEndEvent(_commandList.Get());

    // Indicate that the back buffer will now be used to present.
    auto bar4 = CD3DX12_RESOURCE_BARRIER::Transition(_renderTargets[_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    _commandList->ResourceBarrier(1, &bar4);

    THROW_IF_FAILED(_commandList->Close());
}

}

