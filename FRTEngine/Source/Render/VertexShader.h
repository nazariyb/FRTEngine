#pragma once
#include "Render/GraphicsResource.h"


namespace frt
{
class FRTENGINE_API VertexShader : public GraphicsResource
{
public:
    VertexShader() = delete;
    VertexShader(const VertexShader&) = delete;
    VertexShader(VertexShader&&) = delete;

    VertexShader(Graphics* owner, LPCWSTR filename, unsigned inputSlot);

};
}
