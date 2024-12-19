#pragma once

#include <DirectXMath.h>
#if defined(PLATFORM_PC)
	#define PLATFORM_INLINE_QUATERNION "PC/Quaternion_pc.h"
	typedef DirectX::XMVECTOR QuaternionData;
#endif

// combine two headers to make platform specific Quaterion classes
#include "Quaternion_Common.h"
#include PLATFORM_INLINE_QUATERNION

#undef PLATFORM_INLINE_QUATERNION
