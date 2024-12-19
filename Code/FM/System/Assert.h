#pragma once

#include <cstddef>
#include "GlobalInclude.h"
//#include <System/Logger.h>

#if defined( DEBUG )
#define ASSERT_SYSTEM	ON
#else
#define ASSERT_SYSTEM	OFF
#endif

#if USING( ASSERT_SYSTEM )
#define Assume(expr)	__assume(expr)

#define Assert(expr, ...) (void)( (!!(expr)) || (__debugbreak(),0)); __assume(expr)
#define Verify(expr, ...) (void)( (!!(expr)) || (__debugbreak(),0));
#else
#define Assert(expr, ...) __assume(expr)
#define Verify(expr, ...) (expr)
#endif

