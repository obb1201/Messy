#pragma once

#if defined(PLATFORM_PC)
	#include <d3d11.h>
	#define Texture2DPlatform ID3D11Texture2D
#else
	#error "Unknown Platform"
#endif