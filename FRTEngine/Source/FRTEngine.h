#pragma once

#ifdef FRTENGINE_EXPORTS
#define FRTENGINE_API __declspec(dllexport)
#else
#define FRTENGINE_API __declspec(dllimport)
#include "Utils/Logger/Logger.h"
#include "Input/Input.h"
#endif


#if defined(_DEBUG)
#define IS_DEBUG 1
#define D3DCOMPILE_DEBUG 1
#else
#define IS_DEBUG 0
#define D3DCOMPILE_DEBUG 0
#endif

