#pragma once

#if defined(PLATFORM_PC)
	#include <d3d11.h>
	#define  VertexShaderPlatform ID3D11VertexShader
#else
	#error "Unknown Platform"
#endif