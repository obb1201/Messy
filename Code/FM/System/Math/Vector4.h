#pragma once

#include <DirectXMath.h>
#if defined(PLATFORM_PC)
	#define PLATFORM_INLINE "pc/vector4_pc.h"
	typedef DirectX::XMVECTOR Vector4Data;
#endif


// combine two headers to make platform specific vector classes
#include "vector4_common.h"

#include PLATFORM_INLINE

#undef PLATFORM_INLINE


