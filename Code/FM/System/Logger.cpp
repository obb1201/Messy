#include <limits>

// TODO: make platform specific file? 
#if defined (PLATFORM_PC)
#include <Windows.h>
#endif

#include "Logger.h"
#include "String.h"

namespace fm
{
	UniquePtr<Logger> Logger::mInstance;

	void Logger::CreateInstance()
	{
		Assert(mInstance == nullptr, "don't create this twice");
		mInstance = UniquePtr<Logger>(fmNew(eMemPool::System, "Logger instance") Logger());
	}

	void Logger::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated Logger");
		mInstance.reset();
	}

	Logger::Logger()
	{
		// TODO: disable logging for release build
		mEnabledLevels.SetFlags(std::numeric_limits<uint64>::max());
	}

	void Logger::EnableLevel(const LogLevel level, const bool enabled)
	{
		const uint8 bit = static_cast<uint8>(level);
		if (enabled)
		{
			mEnabledLevels.SetBit(bit);
		}
		else
		{
			mEnabledLevels.ClearBit(bit);
		}
	}

#define LOGLEVEL_TUPLEENTRY(e, str) str
	static char* LogLevelStrings[]
	{
		LOGLEVEL_TUPLE
	};
#undef LOGLEVEL_TUPLEENTRY

	void Logger::Log(const LogLevel level, const char* const format, ...) const
	{
		const uint8 intLevel = (uint8)level;
		// TODO: file log maybe?
		if (!mEnabledLevels.IsSetBit(intLevel))
		{
			return;
		}

		String<2048> buffer;
		buffer.Append("[%s] ", LogLevelStrings[intLevel]);
		va_list arglist;
		va_start(arglist, format);
		buffer.AppendVariableArgs(format, arglist);
		va_end(arglist);
		buffer.Append("\n");
#if defined(PLATFORM_PC)
		OutputDebugStringA(buffer.GetCString());
#endif
	}
}