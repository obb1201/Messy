#pragma once

#if defined(PLATFORM_PC)
	#include <d3d11.h>
	#define  PixelShaderPlatform ID3D11PixelShader
#else
	#error "Unknown Platform"
#endif