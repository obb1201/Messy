#pragma once

#if defined(PLATFORM_PC)
	#include <d3d11.h>
	#define VertexInputLayoutPlatform ID3D11InputLayout
#else
	#error "Unknown Platform"
#endif
