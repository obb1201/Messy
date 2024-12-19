#pragma once
#include "GlobalInclude.h"

#if defined( DEBUG )
	#define MEMORY_TRACKING	ON
#else
	#define MEMORY_TRACKING	OFF
#endif

#if USING( MEMORY_TRACKING )
	#define fmNew( mempool, description )			new(mempool, description)
	#define fmDelete( ptr )							TrackedDelete(ptr); delete ptr
	#define fmDeleteArray( ptr )					TrackedDelete(ptr); delete[] ptr
#else
	#define fmNew( mempool, description )			new
	#define fmDelete( ptr )							delete ptr
	#define fmDeleteArray( ptr )					delete[] ptr
#endif

// TODO: aligned new
#define SafeDelete( ptr )						fmDelete(ptr);		ptr = nullptr
#define SafeDeleteArray( ptr )					fmDeleteArray(ptr);	ptr = nullptr
#define SafeRelease( ptr )						if (ptr) { ptr->Release(); ptr = nullptr; }

#define MemSet( ptr, value, numBytes )			memset( ptr, value, numBytes )
#define MemMove( dstPtr, srcPtr, size )			memmove( dstPtr, srcPtr, size )
#define MemCpy(dstPtr, dstSize, srcPtr, srcSize)	\
	Assert(dstPtr != nullptr, "dstPtr is null");	\
	Assert(srcPtr != nullptr, "srcPtr is null");	\
	Assert(dstSize > 0, "dstSize is zero");			\
	Assert(srcSize > 0, "srcSize is zero");			\
	Assert(dstSize >= srcSize, "srcSize is bigger than dstSize");	\
\
	memcpy(dstPtr, srcPtr, srcSize);				\

// (enum, string)
#define MEMORYTRACK_TUPLE								\
	MEMORYTRACK_TUPLEENTRY(Temp,		"Temp"),		\
	MEMORYTRACK_TUPLEENTRY(Rendering,	"Rendering"),	\
	MEMORYTRACK_TUPLEENTRY(Gameplay,	"Gameplay"),	\
	MEMORYTRACK_TUPLEENTRY(Audio,		"Audio"),		\
	MEMORYTRACK_TUPLEENTRY(Resource,	"Resource"),	\
	MEMORYTRACK_TUPLEENTRY(Threading,	"Threading"),	\
	MEMORYTRACK_TUPLEENTRY(System,		"System"),		\
	MEMORYTRACK_TUPLEENTRY(Count,		"Count"),		\
	// Add more categories as you want

namespace fm
{
#define MEMORYTRACK_TUPLEENTRY(e, str) e
	enum class eMemPool
	{
		MEMORYTRACK_TUPLE
	};
#undef MEMORYTRACK_TUPLEENTRY

	// Basically, it would be proper to use overloading operator new and delete together.
	// but when overloading operator delete is invoked with a couple of parameters including void*, a problem that destructor isn't called happens.
	// (refer that only including void*, calling destructor works).
	// we do not use overloading operator delete for the above reason so that function to track memory is separated from below operator delete.
	void TrackedDelete(void* ptr);
}

// Do not put them into namespace fm. Compile errors will happen.
void* operator new(const size_t size, const fm::eMemPool pooltype, const char* description);
// This is only declared to get rid of waring 4291. It doesn't work ever.
void operator delete(void* ptr, const fm::eMemPool pooltype, const char* description);