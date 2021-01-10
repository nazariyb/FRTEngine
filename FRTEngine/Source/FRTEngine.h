#pragma once

#ifdef FRTENGINE_EXPORTS
#define FRTENGINE_API __declspec(dllexport)
#else
#define FRTENGINE_API __declspec(dllimport)
#endif


#define D3DCOMPILE_DEBUG 1

