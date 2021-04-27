#pragma once
#include "Render/GraphicsResource.h"
#include "Tools/DXHelper.h"

#include "Utils/Logger/Logger.h"
#include <utility>


namespace frt
{
class FRTENGINE_API IndexBuffer : public GraphicsResource
{
public:
    IndexBuffer() = delete;
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer(IndexBuffer&&) = delete;

    IndexBuffer(Graphics* owner, UINT8* indices, UINT indicesNum);

    virtual void Update() override;

    inline virtual void PopulateCommandList() override
    {
        GetCommandList()->IASetIndexBuffer(&_indexBufferView);
    }

protected:
    ComPtr<ID3D12Resource> _indexBuffer;
    D3D12_INDEX_BUFFER_VIEW _indexBufferView;
    ComPtr<ID3D12Resource> _indexBufferUploadHeap;
};
}

