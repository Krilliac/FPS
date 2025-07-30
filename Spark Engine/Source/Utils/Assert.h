#pragma once

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

// Forward declaration to avoid circular dependency
void TriggerCrashHandler(const char* assertMsg);

namespace Assert
{
    inline void Fail(const char* expr,
        const char* file,
        int         line,
        const char* msg = nullptr)
    {
        // Timestamp
        std::time_t t = std::time(nullptr);
        char timeBuf[64];
        std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));

        // Build detailed assertion message
        char fullMsg[2048];
        snprintf(fullMsg, sizeof(fullMsg),
            "===== ASSERTION FAILED =====\n"
            "Time       : %s\n"
            "Expression : %s\n"
            "Location   : %s(%d)\n"
            "Message    : %s\n"
            "Thread ID  : 0x%08X\n",
            timeBuf,
            expr,
            file, line,
            msg ? msg : "",
            static_cast<unsigned>(GetCurrentThreadId())
        );

        // Print to stderr for immediate visibility
        std::fprintf(stderr, "%s", fullMsg);

#ifdef _WIN32
        // Capture and symbolize backtrace
        void* stack[62];
        USHORT frames = CaptureStackBackTrace(1, 62, stack, nullptr);
        SYMBOL_INFO* symbol = (SYMBOL_INFO*)std::malloc(sizeof(SYMBOL_INFO) + 256);
        if (symbol)
        {
            symbol->MaxNameLen = 255;
            symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
            HANDLE process = GetCurrentProcess();
            SymInitialize(process, nullptr, TRUE);
            std::fprintf(stderr, "Call stack:\n");
            for (USHORT i = 0; i < frames; ++i)
            {
                DWORD64 disp = 0;
                if (SymFromAddr(process, (DWORD64)stack[i], &disp, symbol))
                {
                    std::fprintf(stderr,
                        "  [%02u] %s + 0x%0llX\n",
                        frames - i - 1,
                        symbol->Name,
                        (unsigned long long)disp);
                }
                else
                {
                    std::fprintf(stderr,
                        "  [%02u] <unknown symbol>\n",
                        frames - i - 1);
                }
            }
            SymCleanup(process);
            std::free(symbol);
        }
#endif

        std::fflush(stderr);

        // Trigger crash handler to generate dumps, screenshots, etc.
        TriggerCrashHandler(fullMsg);

        DEBUG_BREAK();
        std::abort();
    }

    inline void FailHResult(const char* expr,
        const char* file,
        int         line,
        long        hr,
        const char* msg = nullptr)
    {
        char fullMsg[1024];
        snprintf(fullMsg, sizeof(fullMsg),
            "===== HRESULT FAILED =====\n"
            "Expression : %s returned 0x%08lX\n"
            "Location   : %s(%d)\n"
            "Message    : %s\n"
            "Thread ID  : 0x%08X\n",
            expr, hr,
            file, line,
            msg ? msg : "",
            static_cast<unsigned>(GetCurrentThreadId())
        );

        std::fprintf(stderr, "%s", fullMsg);
        std::fflush(stderr);

        // Trigger crash handler for HRESULT failures too
        TriggerCrashHandler(fullMsg);

        DEBUG_BREAK();
        std::abort();
    }
}

// Debug-only asserts
#if defined(_DEBUG) || defined(DEBUG)
#  define ASSERT(expr)           ((expr) ? (void)0 : Assert::Fail(#expr, __FILE__, __LINE__, nullptr))
#  define ASSERT_MSG(expr,msg)   ((expr) ? (void)0 : Assert::Fail(#expr, __FILE__, __LINE__, msg))
#else
#  define ASSERT(expr)           ((void)0)
#  define ASSERT_MSG(expr,msg)   ((void)0)
#endif

// Always-on asserts
#if !defined(DISABLE_ALWAYS_ASSERTS)
#  define ASSERT_ALWAYS(expr)         ((expr) ? (void)0 : Assert::Fail(#expr, __FILE__, __LINE__, nullptr))
#  define ASSERT_ALWAYS_MSG(expr,msg) ((expr) ? (void)0 : Assert::Fail(#expr, __FILE__, __LINE__, msg))
#else
#  define ASSERT_ALWAYS(expr)         ((void)0)
#  define ASSERT_ALWAYS_MSG(expr,msg) ((void)0)
#endif

// HRESULT-specific asserts
#if defined(_DEBUG) || defined(DEBUG)
#  define ASSERT_HR(hrExpr)            \
     do { long _hr = static_cast<long>(hrExpr); \
          if (FAILED(_hr)) Assert::FailHResult(#hrExpr, __FILE__, __LINE__, _hr, nullptr); \
     } while(0)
#  define ASSERT_HR_MSG(hrExpr, message) \
     do { long _hr = static_cast<long>(hrExpr); \
          if (FAILED(_hr)) Assert::FailHResult(#hrExpr, __FILE__, __LINE__, _hr, message); \
     } while(0)
#else
#  define ASSERT_HR(hrExpr)            ((void)(hrExpr))
#  define ASSERT_HR_MSG(hrExpr,message) ((void)(hrExpr))
#endif

// Null-pointer assertion
#define ASSERT_NOT_NULL(ptr)      ASSERT_MSG((ptr) != nullptr, "Pointer " #ptr " must not be null")
#define ASSERT_NOT_NULL_ALWAYS(p) ASSERT_ALWAYS_MSG((p) != nullptr, "Pointer " #p " must not be null")

// Range assertion
#define ASSERT_IN_RANGE(v,min,max) \
    ASSERT_MSG((v) >= (min) && (v) <= (max), #v " out of range [" #min "," #max "]")