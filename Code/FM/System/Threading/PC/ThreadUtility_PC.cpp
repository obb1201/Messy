#if defined(PLATFORM_PC)

#include <windows.h>

#include "System/Threading/ThreadUtility.h"

namespace fm
{
	//https://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
	//SetThreadName() function refers to the above link. 
	const uint32 MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)
	struct ThreadNameInfo
	{
		uint32		Type;		// Must be 0x1000.
		const char* Name;		// Pointer to name (in user addr space).
		uint32		ThreadID;	// Thread ID (-1=caller thread).
		uint32		Flags;		// Reserved for future use, must be zero.
	} ;
#pragma pack(pop)

	void SetThreadName(const std::thread::id& id, const char* threadName)
	{
		uint32* uId = (uint32*)&id;

		ThreadNameInfo info;
		info.Type = 0x1000;
		info.Name = threadName;
		info.ThreadID = *uId;
		info.Flags = 0;

#pragma warning(push)
#pragma warning(disable: 6320 6322)
		__try 
		{
			RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) 
		{
		}
#pragma warning(pop)
	}

	void SetThreadPriority(void* nativeHandle, const int32 priority)
	{
		::SetThreadPriority(nativeHandle, priority);
	}

	int32 GetThreadPriority(void* nativeHandle)
	{
		return ::GetThreadPriority(nativeHandle);
	}
}

#endif