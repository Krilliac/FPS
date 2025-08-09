/**
 * @file namespace_fix.h
 * @brief Fixes namespace collision issues with standard library
 * @author Spark Engine Team
 * @date 2025
 */

#pragma once

// This header must be included before any SparkEditor headers that might
// cause namespace pollution

#ifndef _SPARKEDITOR_NAMESPACE_FIX_
#define _SPARKEDITOR_NAMESPACE_FIX_

// Include critical standard library headers first to establish proper namespaces
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <chrono>
#include <unordered_map>

// Prevent any macro pollution of std namespace
#ifdef std
#undef std
#endif

// Make sure we never accidentally create a nested std namespace
// by ensuring all std references are fully qualified
#define SPARKEDITOR_STD ::std

#endif // _SPARKEDITOR_NAMESPACE_FIX_