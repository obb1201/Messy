//-------------------------------------------------------------------------------------------------
// GlobalInclude.h
//
// The one file that will be included by everything in the engine.
//-------------------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
//------------------------------
// Easy preprocessor defines from http://www.codersnotes.com/notes/easy-preprocessor-defines
//------------------------------
#define ON		2 -
#define OFF		1 -
#define USING( x )			( (x 0) == 2 )
#define NOTUSING( x )		( (x 0) == 1 )


//------------------------------------------------
// inline related Macros
//------------------------------------------------
#define FORCEINLINE	__forceinline

//------------------------------------------------
// attributes
//------------------------------------------------
#define FALLTHROUGH __fallthrough

//------------------------------------------------
//
//------------------------------------------------
#define COPY_CONSTRUCTOR( T )			T( const T& a )
// TODO: use C++11 feature for this
#define ASSIGN_OPERATOR( T ) void operator=( const T& )

// TODO: move this to array
//-----------------------------------------------
// ARRAY
//-----------------------------------------------
#define StaticArraySize( a ) ( sizeof(a) / sizeof( a[0] ) )

namespace fm
{
	//--------------------------
	// Integer Types
	//--------------------------
	typedef int8_t			int8;
	typedef uint8_t			uint8;
	typedef int16_t			int16;
	typedef uint16_t		uint16;
	typedef int32_t			int32;
	typedef uint32_t		uint32;
	typedef int64_t			int64;
	typedef uint64_t		uint64;
	typedef uintptr_t		uintptr;
	typedef ptrdiff_t		ptrdiff;

#define INVALID_INDEX (-1)

	typedef uint32 Handle;
	const uint32 INVALID_HANDLE = (uint32)-1;

	// safe sign/unsigned conversion
	FORCEINLINE int8		ToSigned(const uint8 x)  { return (int8)x; }
	FORCEINLINE int16		ToSigned(const uint16 x) { return (int16)x; }
	FORCEINLINE int32		ToSigned(const uint32 x) { return (int32)x; }
	FORCEINLINE int64		ToSigned(const uint64 x) { return (int64)x; }

	FORCEINLINE uint8		ToUnsigned(const int8 x)  { return (uint8)x; }
	FORCEINLINE uint16		ToUnsigned(const int16 x) { return (uint16)x; }
	FORCEINLINE uint32		ToUnsigned(const int32 x) { return (uint32)x; }
	FORCEINLINE uint64		ToUnsigned(const int64 x) { return (uint64)x; }
};

