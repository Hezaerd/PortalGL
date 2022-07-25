// ReSharper disable CppDeprecatedEntity

#include "Core/Debug/Log.h"

using namespace Core::Debug;

bool Log::Openfile(std::filesystem::path const& filename)
{
	logFile = fopen((char const*)filename.string().c_str(), "w");  // NOLINT(clang-diagnostic-deprecated-declarations)
	return (logFile != nullptr);
}

Log::~Log()
{
	if (logFile)
	{
		fclose(logFile);
	}
}

void Log::Print(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	if (logFile != nullptr)
	{
		va_list copy;
		va_copy(copy, args);
		vfprintf(logFile, format, args);
		va_end(copy);
	}
	vfprintf(stdout, format, args);
	va_end(args);
}