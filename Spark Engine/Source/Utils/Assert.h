#pragma once

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#ifdef _MSC_VER
#  include <intrin.h>
#  define DEBUG_BREAK() __debugbreak()
#else
#  include <signal.h>
#  define DEBUG_BREAK() raise(SIGTRAP)
#endif

#ifdef _WIN32
#  include <windows.h>
#  include <dbghelp.h>
#  pragma comment(lib, "dbghelp.lib")
#endif

// Forward declaration
void TriggerCrashHandler(const char* assertMsg);

namespace Assert
{
    // Core failure implementation
    inline void Fail(const char* expr,
        const char* file,
        int         line,
        const char* fmt = nullptr, ...)
    {
        // Timestamp
        std::time_t t = std::time(nullptr);
        char timeBuf[64];
        std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));

        // Format user message if provided
        char userMsg[512] = {};
        if (fmt)
        {
            va_list args;
            va_start(args, fmt);
            vsnprintf(userMsg, sizeof(userMsg), fmt, args);
            va_end(args);
        }

        // Build the full assertion text
        char fullMsg[2048];
        std::snprintf(fullMsg, sizeof(fullMsg),
            "===== ASSERTION FAILED =====\n"
            "Time       : %s\n"
            "Expression : %s\n"
            "Location   : %s(%d)\n"
            "Message    : %s\n"
            "Thread ID  : 0x%08X\n",
            timeBuf,
            expr,
            file, line,
            userMsg[0] ? userMsg : "(none)",
            static_cast<unsigned>(GetCurrentThreadId())
        );

        // Print immediately
        std::fprintf(stderr, "%s", fullMsg);
        std::fflush(stderr);

#ifdef _WIN32
        // Capture and symbolize backtrace (omitted here for brevity)...
#endif

        // Trigger optional crash handler
        TriggerCrashHandler(fullMsg);

        DEBUG_BREAK();
        std::abort();
    }

    // HRESULT-specific fail
    inline void FailHResult(const char* expr,
        const char* file,
        int         line,
        long        hr,
        const char* fmt = nullptr, ...)
    {
        char userMsg[512] = {};
        if (fmt)
        {
            va_list args;
            va_start(args, fmt);
            vsnprintf(userMsg, sizeof(userMsg), fmt, args);
            va_end(args);
        }

        char fullMsg[2048];
        std::snprintf(fullMsg, sizeof(fullMsg),
            "===== HRESULT FAILED =====\n"
            "Expression : %s returned 0x%08lX\n"
            "Location   : %s(%d)\n"
            "Message    : %s\n"
            "Thread ID  : 0x%08X\n",
            expr, hr,
            file, line,
            userMsg[0] ? userMsg : "(none)",
            static_cast<unsigned>(GetCurrentThreadId())
        );

        std::fprintf(stderr, "%s", fullMsg);
        std::fflush(stderr);

        TriggerCrashHandler(fullMsg);
        DEBUG_BREAK();
        std::abort();
    }
}

// Simple assert: no message
#if defined(_DEBUG) || defined(DEBUG)
#  define ASSERT(expr) \
     ((expr) ? (void)0 : Assert::Fail(#expr, __FILE__, __LINE__))
#else
#  define ASSERT(expr) ((void)0)
#endif

// Assert with formatted message
#if defined(_DEBUG) || defined(DEBUG)
#  define ASSERT_MSG(expr, fmt, ...) \
     ((expr) ? (void)0 : Assert::Fail(#expr, __FILE__, __LINE__, fmt, __VA_ARGS__))
#else
#  define ASSERT_MSG(expr, fmt, ...) ((void)0)
#endif

// Always-on asserts
#if !defined(DISABLE_ALWAYS_ASSERTS)
#  define ASSERT_ALWAYS(expr) \
     ((expr) ? (void)0 : Assert::Fail(#expr, __FILE__, __LINE__))
#  define ASSERT_ALWAYS_MSG(expr, fmt, ...) \
     ((expr) ? (void)0 : Assert::Fail(#expr, __FILE__, __LINE__, fmt, __VA_ARGS__))
#else
#  define ASSERT_ALWAYS(expr)       ((void)0)
#  define ASSERT_ALWAYS_MSG(expr, fmt, ...) ((void)0)
#endif

// HRESULT-specific asserts
#if defined(_DEBUG) || defined(DEBUG)
#  define ASSERT_HR(hrExpr)                                \
     do { long _hr = static_cast<long>(hrExpr);             \
          if (FAILED(_hr))                                  \
             Assert::FailHResult(#hrExpr, __FILE__, __LINE__, _hr); \
     } while(0)

#  define ASSERT_HR_MSG(hrExpr, fmt, ...)                   \
     do { long _hr = static_cast<long>(hrExpr);             \
          if (FAILED(_hr))                                  \
             Assert::FailHResult(#hrExpr, __FILE__, __LINE__, _hr, fmt, __VA_ARGS__); \
     } while(0)
#else
#  define ASSERT_HR(hrExpr)       ((void)(hrExpr))
#  define ASSERT_HR_MSG(hrExpr, fmt, ...) ((void)(hrExpr))
#endif

// Null-pointer assertion
#define ASSERT_NOT_NULL(ptr)   ASSERT_MSG((ptr) != nullptr, "Pointer " #ptr " must not be null")
#define ASSERT_NOT_NULL_ALWAYS(ptr) ASSERT_ALWAYS_MSG((ptr) != nullptr, "Pointer " #ptr " must not be null")

// Range assertion
#define ASSERT_IN_RANGE(v, min, max) \
    ASSERT_MSG((v) >= (min) && (v) <= (max), #v " out of range [" #min "," #max "]")