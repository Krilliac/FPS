/**
 * @file ultra_build_fix.h
 * @brief Ultra-aggressive build fix using extreme measures
 * @author Spark Engine Team
 * @date 2025
 */

#pragma once

#ifndef _ULTRA_BUILD_FIX_H_
#define _ULTRA_BUILD_FIX_H_

// NUCLEAR OPTION: Completely disable all debug macros and force clean std includes
#ifdef _DEBUG
#undef _DEBUG
#define _SPARKEDITOR_WAS_DEBUG
#endif

#ifdef _CRTDBG_MAP_ALLOC
#undef _CRTDBG_MAP_ALLOC
#define _SPARKEDITOR_HAD_CRTDBG
#endif

#ifdef _CRT_SECURE_NO_WARNINGS
#undef _CRT_SECURE_NO_WARNINGS
#endif

#ifdef _CRT_NONSTDC_NO_DEPRECATE
#undef _CRT_NONSTDC_NO_DEPRECATE
#endif

// Force clean includes
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

// Include EVERYTHING we need from standard library WITHOUT any debug pollution
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <chrono>
#include <unordered_map>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <array>
#include <tuple>
#include <utility>
#include <type_traits>
#include <iterator>
#include <numeric>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <new>
#include <typeinfo>
#include <limits>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <cassert>
#include <mutex>
#include <thread>
#include <atomic>
#include <future>

// C++20 features if available
#if __cplusplus >= 202002L
#include <ranges>
#include <concepts>
#include <span>
#include <format>
#endif

// Include Windows AFTER standard library
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>

// Restore debug settings ONLY after all includes
#ifdef _SPARKEDITOR_WAS_DEBUG
#define _DEBUG
#undef _SPARKEDITOR_WAS_DEBUG
#endif

#ifdef _SPARKEDITOR_HAD_CRTDBG
#define _CRTDBG_MAP_ALLOC
#undef _SPARKEDITOR_HAD_CRTDBG
#endif

// Ensure std is NEVER redefined
#ifdef std
#undef std
#endif

// Create ultra-safe aliases that bypass any namespace pollution
#define SE_STD ::std
#define SE_COUT ::std::cout
#define SE_CERR ::std::cerr
#define SE_STRING ::std::string
#define SE_VECTOR ::std::vector
#define SE_UNORDERED_MAP ::std::unordered_map
#define SE_UNIQUE_PTR ::std::unique_ptr
#define SE_SHARED_PTR ::std::shared_ptr
#define SE_FUNCTION ::std::function
#define SE_CHRONO ::std::chrono

// Ultra-safe type aliases
namespace SparkEditorTypes {
    using String = ::std::string;
    template<typename T> using Vector = ::std::vector<T>;
    template<typename K, typename V> using UnorderedMap = ::std::unordered_map<K, V>;
    template<typename T> using UniquePtr = ::std::unique_ptr<T>;
    template<typename T> using SharedPtr = ::std::shared_ptr<T>;
    template<typename T> using Function = ::std::function<T>;
    using HighResClock = ::std::chrono::high_resolution_clock;
    using TimePoint = ::std::chrono::system_clock::time_point;
}

#endif // _ULTRA_BUILD_FIX_H_