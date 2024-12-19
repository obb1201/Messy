#pragma once

#if defined(PLATFORM_PC)
	#include <d3d11.h>
	#define  VertexBufferPlatform ID3D11Buffer
#else
	#error "Unknown Platform"
#endif