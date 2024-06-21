#ifdef __OHOS__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "../Interfaces/IOperatingSystem.h"

// interfaces

#include "../Interfaces/ILog.h"
//解决命名空间冲突问题
#define LogLevel  OHOS_LogLevel  
#define LOG_DEBUG OHOS_LOG_DEBUG 
#define LOG_INFO  OHOS_LOG_INFO  
#define LOG_WARN  OHOS_LOG_WARN  
#define LOG_ERROR OHOS_LOG_ERROR 
#define LOG_FATAL OHOS_LOG_FATAL 

#include <hilog/log.h>

#undef LogLevel  
#undef LOG_DEBUG 
#undef LOG_INFO  
#undef LOG_WARN  
#undef LOG_ERROR 
#undef LOG_FATAL 

#include <assert.h>
#include "../Interfaces/IMemory.h"
#include <signal.h>

void _OutputDebugStringV(const char *str, va_list args)
{
#if defined(FORGE_DEBUG)
	OH_LOG_Print(LOG_APP, OHOS_LOG_INFO, LOG_DOMAIN, "The-Forge", str, args);
#endif
}

void _OutputDebugString(const char *str, ...)
{
#if defined(FORGE_DEBUG)
	va_list arglist;
	va_start(arglist, str);
	OH_LOG_Print(LOG_APP, OHOS_LOG_INFO, LOG_DOMAIN, "The-Forge", str, arglist);
	va_end(arglist);
#endif
}

void _FailedAssert(const char *file, int line, const char *statement)
{
#ifdef FORGE_DEBUG
	OH_LOG_Print(LOG_APP, OHOS_LOG_ERROR, LOG_DOMAIN, "The-Forge", "Assertion failed: (%s)\n\nFile: %s\nLine: %d\n\n", statement, file, line);
	raise(SIGTRAP);
#endif
}

void _PrintUnicode(const char *str, bool error)
{
	OH_LOG_Print(LOG_APP, error ? OHOS_LOG_ERROR : OHOS_LOG_INFO, LOG_DOMAIN, "The-Forge","%s", str);
}
#endif // ifdef __OHOS__