#pragma once

#include "System/UniquePtr.h"
#include "System/Flag64.h"

// (enum, string)
#define LOGLEVEL_TUPLE							\
	LOGLEVEL_TUPLEENTRY(Error,		"Error"),	\
	LOGLEVEL_TUPLEENTRY(Warning,	"Warning"),	\
	LOGLEVEL_TUPLEENTRY(Info,		"Info"),	\
	LOGLEVEL_TUPLEENTRY(Debug,		"Debug"),	\

namespace fm
{
#define LOGLEVEL_TUPLEENTRY(e, str) e
	enum class LogLevel
	{
		LOGLEVEL_TUPLE
	};
#undef LOGLEVEL_TUPLEENTRY

#define LOG(loglevel, ...) Logger::GetInstance().Log(loglevel, __VA_ARGS__)

	class Logger final
	{
		FRIEND_UNIQUEPTR(Logger);
	public:
		COPY_CONSTRUCTOR(Logger) = delete;
		ASSIGN_OPERATOR(Logger) = delete;

		// --- singleton-related
		static void CreateInstance();
		static void DeleteInstance();
		static Logger& GetInstance() { Assert(mInstance.get() != nullptr); return *mInstance; }

		void EnableLevel(const LogLevel level, const bool enabled);
		void Log(const LogLevel level, const char* const format, ...) const;

	private:
		static UniquePtr<Logger> mInstance;
		
		Flag64 mEnabledLevels;

	private:
		Logger();
		~Logger() = default;
	};
}