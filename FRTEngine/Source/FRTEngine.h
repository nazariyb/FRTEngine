#pragma once

#ifdef FRTENGINE_EXPORTS
#define FRTENGINE_API __declspec(dllexport)
#else
#define FRTENGINE_API __declspec(dllimport)
#include "Utils/Logger/Logger.h"
#include "Input/Input.h"
#endif


#define D3DCOMPILE_DEBUG 1

