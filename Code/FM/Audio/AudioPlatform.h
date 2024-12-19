#pragma once

#if defined(PLATFORM_PC)
	#include "audio/PC/AudioSystem_PC.h"

	#define AudioPlatform AudioSystemPC
#endif