//#pragma once
//
//#include "FRTEngine.h"
//
//#include <initguid.h>
//#include <d3dcompiler.h>
//#include <d3d12.h>
//#include <dxgi1_4.h>
//#include <wrl.h>
//
//#include "Tools/d3dx12.h"
//
//
//namespace frt
//{
//class FRTENGINE_API DeviceResources
//{
//    DeviceResources();
//    ~DeviceResources();
//
//    HRESULT CreateDeviceResources(HWND hWnd);
//    HRESULT CreateDeviceResources();
//    HRESULT CreateWindowResources(HWND hWnd);
//
//    HRESULT ConfigureBackBuffer();
//    HRESULT ReleaseBackBuffer();
//    HRESULT GoFullScreen();
//    HRESULT GoWindowed();
//
//    float GetAspectRatio();
//
//    ID3D12Device* GetDevice() { return m_pd3dDevice.Get(); };
//    ID3D12DeviceContext* GetDeviceContext() { return m_pd3dDeviceContext.Get(); };
//    ID3D12RenderTargetView* GetRenderTarget() { return m_pRenderTarget.Get(); }
//    ID3D12DepthStencilView* GetDepthStencil() { return m_pDepthStencilView.Get(); }
//
//    void Present();
//
//private:
//
//    //-----------------------------------------------------------------------------
//    // Direct3D device
//    //-----------------------------------------------------------------------------
//    Microsoft::WRL::ComPtr<ID3D12Device>        m_pd3dDevice;
//    Microsoft::WRL::ComPtr<ID3D12DeviceContext> m_pd3dDeviceContext; // immediate context
//    Microsoft::WRL::ComPtr<IDXGISwapChain>      m_pDXGISwapChain;
//
//
//    //-----------------------------------------------------------------------------
//    // DXGI swap chain device resources
//    //-----------------------------------------------------------------------------
//    Microsoft::WRL::ComPtr < ID3D12Texture2D>        m_pBackBuffer;
//    Microsoft::WRL::ComPtr < ID3D12RenderTargetView> m_pRenderTarget;
//
//
//    //-----------------------------------------------------------------------------
//    // Direct3D device resources for the depth stencil
//    //-----------------------------------------------------------------------------
//    Microsoft::WRL::ComPtr<ID3D12Texture2D>         m_pDepthStencil;
//    Microsoft::WRL::ComPtr<ID3D12DepthStencilView>  m_pDepthStencilView;
//
//
//    //-----------------------------------------------------------------------------
//    // Direct3D device metadata and device resource metadata
//    //-----------------------------------------------------------------------------
//    D3D_FEATURE_LEVEL       m_featureLevel;
//    D3D12_TEXTURE2D_DESC    m_bbDesc;
//    D3D12_VIEWPORT          m_viewport;
//};
//}
//
