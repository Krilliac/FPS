#pragma once

#include <cstdio>
#include <cstdlib>

#ifdef _MSC_VER
#  define DEBUG_BREAK() __debugbreak()
#else
#  include <signal.h>
#  define DEBUG_BREAK() raise(SIGTRAP)
#endif

namespace Assert
{
    inline void Fail(const char* expr, const char* file, int line, const char* msg = nullptr)
    {
        std::fprintf(stderr,
            "Assertion failed: (%s)\nFile: %s, Line: %d\n%s\n",
            expr, file, line,
            msg ? msg : ""
        );
        DEBUG_BREAK();
        std::abort();
    }

    inline void FailHResult(const char* expr, const char* file, int line, long hr, const char* msg = nullptr)
    {
        std::fprintf(stderr,
            "HRESULT check failed: (%s) returned 0x%08lX\nFile: %s, Line: %d\n%s\n",
            expr, hr, file, line,
            msg ? msg : ""
        );
        DEBUG_BREAK();
        std::abort();
    }
}

// Debug-only asserts
#if defined(_DEBUG) || defined(DEBUG)
#  define ASSERT(expr) \
     ((expr) ? (void)0 : Assert::Fail(#expr, __FILE__, __LINE__, nullptr))
#  define ASSERT_MSG(expr, message) \
     ((expr) ? (void)0 : Assert::Fail(#expr, __FILE__, __LINE__, message))
#else
#  define ASSERT(expr) ((void)0)
#  define ASSERT_MSG(expr, message) ((void)0)
#endif

// Always-on asserts (disable with DISABLE_ALWAYS_ASSERTS)
#if !defined(DISABLE_ALWAYS_ASSERTS)
#  define ASSERT_ALWAYS(expr) \
     ((expr) ? (void)0 : Assert::Fail(#expr, __FILE__, __LINE__, nullptr))
#  define ASSERT_ALWAYS_MSG(expr, message) \
     ((expr) ? (void)0 : Assert::Fail(#expr, __FILE__, __LINE__, message))
#else
#  define ASSERT_ALWAYS(expr) ((void)0)
#  define ASSERT_ALWAYS_MSG(expr, message) ((void)0)
#endif

// HRESULT-specific asserts
#if defined(_DEBUG) || defined(DEBUG)
#  define ASSERT_HR(hrExpr) \
     do { long _hr = static_cast<long>(hrExpr); \
          if (FAILED(_hr)) Assert::FailHResult(#hrExpr, __FILE__, __LINE__, _hr, nullptr); \
     } while(0)
#  define ASSERT_HR_MSG(hrExpr, message) \
     do { long _hr = static_cast<long>(hrExpr); \
          if (FAILED(_hr)) Assert::FailHResult(#hrExpr, __FILE__, __LINE__, _hr, message); \
     } while(0)
#else
#  define ASSERT_HR(hrExpr) ((void)(hrExpr))
#  define ASSERT_HR_MSG(hrExpr, message) ((void)(hrExpr))
#endif

// Null-pointer assertions
#define ASSERT_NOT_NULL(ptr) \
    ASSERT_MSG((ptr) != nullptr, "Pointer " #ptr " must not be null")

#define ASSERT_NOT_NULL_ALWAYS(ptr) \
    ASSERT_ALWAYS_MSG((ptr) != nullptr, "Pointer " #ptr " must not be null")

// Range assertions
#define ASSERT_IN_RANGE(val, min, max) \
    ASSERT_MSG((val) >= (min) && (val) <= (max), #val " out of range [" #min "," #max "]")