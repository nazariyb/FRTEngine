#pragma once
#include "FRTEngine.h"
#include "Render/Graphics.h"

namespace frt
{
class FRTENGINE_API GraphicsResource
{
protected:
    GraphicsResource() = delete;
    GraphicsResource(Graphics* graphics);
    virtual ~GraphicsResource();

public:
    virtual void Update() = 0;
    virtual void PopulateCommandList() = 0;

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
};
}

