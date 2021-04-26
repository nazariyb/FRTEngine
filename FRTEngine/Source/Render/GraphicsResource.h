#pragma once
#include "FRTEngine.h"
#include "Render/Graphics.h"
#include "ITickable.h"


namespace frt
{
class FRTENGINE_API GraphicsResource : public ITickable
{
protected:
    GraphicsResource() = delete;
    GraphicsResource(Graphics* graphics);
    virtual ~GraphicsResource();

protected:
    Graphics* _owner;

    inline ID3D12GraphicsCommandList* GetCommandList()
    {
        return _owner->_commandList.Get();
    };

    inline ID3D12Device* GetDevice()
    {
        return _owner->_device.Get();
    };

    inline ID3D12PipelineState* GetPipelineState()
    {
        return _owner->_pipelineState.Get();
    };

    inline UINT PostIncConstBuffersNum()
    {
        return _owner->_constBuffersNum++;
    };

    inline UINT GetCbvSrvDescriptorSize()
    {
        return _owner->_cbvSrvDescriptorSize;
    };

    inline ID3D12DescriptorHeap* GetCbvSrvHeap()
    {
        return _owner->_cbvSrvHeap.Get();
    }

    inline D3D12_CPU_DESCRIPTOR_HANDLE GetCbvSrvCPUDescriptorHandleForHeapStart()
    {
        return _owner->_cbvSrvHeap->GetCPUDescriptorHandleForHeapStart();
    }
};
}

