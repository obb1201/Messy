#pragma once

#include <DirectXMath.h>
#if defined(PLATFORM_PC)
	#define PLATFORM_INLINE "pc/vector2_pc.h"
	typedef DirectX::XMVECTOR Vector2Data;
#endif


// combine two headers to make platform specific vector classes
#include "Vector2_common.h"

#include PLATFORM_INLINE

#undef PLATFORM_INLINE


