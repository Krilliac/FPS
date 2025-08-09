/**
 * @file build_fix.h
 * @brief Comprehensive build fix for namespace issues
 * @author Spark Engine Team
 * @date 2025
 */

#pragma once

// CRITICAL: This header must be included FIRST in any file that uses SparkEditor namespace
// It fixes the namespace collision issues that prevent compilation

#ifndef _SPARKEDITOR_BUILD_FIX_H_
#define _SPARKEDITOR_BUILD_FIX_H_

// STEP 1: Prevent problematic macros that cause namespace pollution
#ifdef _CRTDBG_MAP_ALLOC
#undef _CRTDBG_MAP_ALLOC
#endif

// STEP 2: Include ALL standard library headers FIRST to establish proper namespaces
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

// STEP 3: Include ranges and concepts (C++20 features)
#if __cplusplus >= 202002L
#include <ranges>
#include <concepts>
#endif

// STEP 4: Prevent any accidental std namespace pollution
#ifdef std
#undef std
#endif

// STEP 5: Re-enable _CRTDBG_MAP_ALLOC ONLY for debug builds after includes
#ifdef _DEBUG
#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#endif
#endif

#endif // _SPARKEDITOR_BUILD_FIX_H_