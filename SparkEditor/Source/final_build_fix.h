/**
 * @file final_build_fix.h
 * @brief Final comprehensive solution for namespace pollution
 * @author Spark Engine Team
 * @date 2025
 */

#pragma once

#ifndef _FINAL_BUILD_FIX_H_
#define _FINAL_BUILD_FIX_H_

// The core issue: _CRTDBG_MAP_ALLOC causes Windows headers to create
// a nested 'std' namespace inside any currently open namespace.
// We must disable it completely during our includes.

// Save the original debug setting
#ifdef _CRTDBG_MAP_ALLOC
#define _SPARKEDITOR_HAD_CRTDBG_MAP_ALLOC
#undef _CRTDBG_MAP_ALLOC
#endif

// Include all standard headers WITHOUT debug mapping
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <chrono>
#include <unordered_map>
#include <type_traits>
#include <ranges>
#include <concepts>
#include <Windows.h>

// Restore debug mapping only if it was originally set
#ifdef _SPARKEDITOR_HAD_CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#undef _SPARKEDITOR_HAD_CRTDBG_MAP_ALLOC
#endif

// Ensure std is never accidentally defined as something else
#ifdef std
#undef std
#endif

// Define safe aliases to prevent any issues
namespace SparkEditorSafe {
    using cout_type = decltype(::std::cout);
    using cerr_type = decltype(::std::cerr);
    using string_type = ::std::string;
    template<typename T> using vector_type = ::std::vector<T>;
    template<typename T> using unique_ptr_type = ::std::unique_ptr<T>;
}

#endif // _FINAL_BUILD_FIX_H_