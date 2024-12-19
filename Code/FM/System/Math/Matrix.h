#pragma once

#include <DirectXMath.h>
#if defined(PLATFORM_PC)
	#define PLATFORM_INLINE_MATRIX "pc/matrix_pc.h"
	typedef DirectX::XMMATRIX MatrixData;
#endif


// combine two headers to make platform specific vector classes
#include "matrix_common.h"
#include PLATFORM_INLINE_MATRIX

#undef PLATFORM_INLINE_MATRIX


