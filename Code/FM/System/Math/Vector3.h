#pragma once

#include <DirectXMath.h>

#if defined(PLATFORM_PC)
	#define PLATFORM_INLINE "pc/vector3_pc.h"
	typedef DirectX::XMVECTOR Vector3Data;
#endif


// combine two headers to make platform specific vector classes
#include "vector3_common.h"

#include PLATFORM_INLINE

#undef PLATFORM_INLINE


