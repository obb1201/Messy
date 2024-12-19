#pragma once

#if defined(PLATFORM_PC)
	#include <d3d11.h>
	#define ShaderResourceViewPlatform ID3D11ShaderResourceView	
	#define SamplerStatePlatform ID3D11SamplerState
#else
	#error "Unknown Platform"
#endif
