#include "GraphicsResource.h"

namespace frt
{
GraphicsResource::GraphicsResource(Graphics* graphics)
    : _owner(graphics)
{}

GraphicsResource::~GraphicsResource()
{
    delete _owner;
}
}
