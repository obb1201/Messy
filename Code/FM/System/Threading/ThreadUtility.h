#pragma once

#include <thread>

#include "System/GlobalInclude.h"

namespace fm
{
	void SetThreadName(const std::thread::id& id, const char* threadname);
	void SetThreadPriority(void* nativeHandle, const int32 priority);
	int32 GetThreadPriority(void* nativeHandle);
}
