/**
 * @file comprehensive_build_fix.h
 * @brief Comprehensive fix for all namespace pollution issues
 * @author Spark Engine Team
 * @date 2025
 */

#pragma once

#ifndef _COMPREHENSIVE_BUILD_FIX_H_
#define _COMPREHENSIVE_BUILD_FIX_H_

// STEP 1: Disable ALL problematic debug macros before any includes
#ifdef _CRTDBG_MAP_ALLOC
#undef _CRTDBG_MAP_ALLOC
#endif

#ifdef _DEBUG
#undef _DEBUG
#define _WAS_DEBUG_DEFINED
#endif

// STEP 2: Include standard library headers without debug mappings
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

// STEP 3: Include C++20 features if available
#if __cplusplus >= 202002L
#include <ranges>
#include <concepts>
#include <span>
#include <format>
#endif

// STEP 4: Re-enable debug mode if it was originally defined
#ifdef _WAS_DEBUG_DEFINED
#undef _WAS_DEBUG_DEFINED
#define _DEBUG
// Re-enable CRTDBG_MAP_ALLOC but only after std includes
#define _CRTDBG_MAP_ALLOC
#endif

// STEP 5: Ensure std is never redefined
#ifdef std
#undef std
#endif

// STEP 6: Provide safe macros for common operations
#define SPARKEDITOR_COUT ::std::cout
#define SPARKEDITOR_CERR ::std::cerr
#define SPARKEDITOR_STRING ::std::string
#define SPARKEDITOR_VECTOR ::std::vector
#define SPARKEDITOR_MEMORY ::std::memory

#endif // _COMPREHENSIVE_BUILD_FIX_H_