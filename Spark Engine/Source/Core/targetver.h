/**
 * @file targetver.h
 * @brief Windows platform version targeting
 * @author Spark Engine Team
 * @date 2025
 * 
 * This header defines the Windows platform version that the application targets.
 * Including SDKDDKVer.h defines the highest available Windows platform. To target
 * a specific Windows version, include WinSDKVer.h and set _WIN32_WINNT before
 * including SDKDDKVer.h.
 */

#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
#include <SDKDDKVer.h>

