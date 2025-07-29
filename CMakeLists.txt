cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(SparkEngine LANGUAGES C CXX)

# -------------------------------------------------------------
# Global compiler setup
# -------------------------------------------------------------
set(CMAKE_CXX_STANDARD        17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
  add_compile_options(/permissive- /Zc:__cplusplus)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# -------------------------------------------------------------
# Remove any "-d" postfix from static libs
# -------------------------------------------------------------
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "No -d on static libraries" FORCE)

# -------------------------------------------------------------
# Build options
# -------------------------------------------------------------
option(ENABLE_EDITOR      "Build with in-engine editor UI" ON)
option(ENABLE_CONSOLE     "External debug console overlay" ON)
option(ENABLE_ANGELSCRIPT "Embed AngelScript scripting"    ON)

message(STATUS "Editor:      ${ENABLE_EDITOR}")
message(STATUS "Console:     ${ENABLE_CONSOLE}")
message(STATUS "AngelScript: ${ENABLE_ANGELSCRIPT}")

# -------------------------------------------------------------
# Gather engine sources & headers
# -------------------------------------------------------------
file(GLOB_RECURSE ENGINE_SRC
  "${CMAKE_SOURCE_DIR}/Source/*.cpp"
  "${CMAKE_SOURCE_DIR}/Spark Engine/Source/*.cpp"
  "${CMAKE_SOURCE_DIR}/Engine/*.cpp"
  "${CMAKE_SOURCE_DIR}/Engine/Core/*.cpp"
)

file(GLOB_RECURSE ENGINE_HDR
  "${CMAKE_SOURCE_DIR}/Source/*.h"
  "${CMAKE_SOURCE_DIR}/Spark Engine/Source/*.h"
  "${CMAKE_SOURCE_DIR}/Engine/*.h"
  "${CMAKE_SOURCE_DIR}/Engine/Core/*.h"
)

if (NOT ENGINE_SRC)
  message(FATAL_ERROR "No engine .cpp files found!")
endif()

# -------------------------------------------------------------
# Executable target (WIN32 subsystem to avoid requiring main())
# -------------------------------------------------------------
if (WIN32)
  add_executable(SparkEngine WIN32
    ${ENGINE_SRC}
    ${ENGINE_HDR}
  )
else()
  add_executable(SparkEngine
    ${ENGINE_SRC}
    ${ENGINE_HDR}
  )
endif()

# -------------------------------------------------------------
# ThirdParty: miniz
# -------------------------------------------------------------
add_library(miniz
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/miniz/miniz.c"
)
target_include_directories(miniz PUBLIC
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/miniz"
)
target_link_libraries(SparkEngine PRIVATE miniz)

# -------------------------------------------------------------
# ThirdParty: tinyobjloader (single-header implementation)
# -------------------------------------------------------------
set(TINYOBJ_HDR
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/tinyobjloader/include/tiny_obj_loader.h"
)

set_source_files_properties(${TINYOBJ_HDR}
  PROPERTIES LANGUAGE CXX
)

add_library(tinyobjloader STATIC
  ${TINYOBJ_HDR}
)
target_compile_definitions(tinyobjloader PUBLIC
  TINYOBJLOADER_IMPLEMENTATION
)
target_include_directories(tinyobjloader PUBLIC
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/tinyobjloader/include"
)
target_link_libraries(SparkEngine PRIVATE tinyobjloader)

# -------------------------------------------------------------
# ThirdParty: libcurl (static via submodule)
# -------------------------------------------------------------
set(BUILD_SHARED_LIBS    OFF CACHE BOOL "" FORCE)
set(BUILD_CURL_EXE       OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING        OFF CACHE BOOL "" FORCE)
set(CURL_USE_SCHANNEL    ON  CACHE BOOL "" FORCE)
set(CURL_DISABLE_LDAP    ON  CACHE BOOL "" FORCE)
set(CURL_ZLIB            OFF CACHE BOOL "" FORCE)
set(CURL_BROTLI          OFF CACHE BOOL "" FORCE)
set(CURL_ZSTD            OFF CACHE BOOL "" FORCE)
set(CURL_HTTP2           OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL      OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBIDN2     OFF CACHE BOOL "" FORCE)

add_subdirectory(
  "${CMAKE_SOURCE_DIR}/ThirdParty/Networking/curl"
  "${CMAKE_BINARY_DIR}/curl" EXCLUDE_FROM_ALL
)

# detect which curl target was created
if (TARGET curl)
  set(_CURL_TGT curl)
elseif (TARGET libcurl)
  set(_CURL_TGT libcurl)
elseif (TARGET CURL::libcurl)
  set(_CURL_TGT CURL::libcurl)
else()
  message(FATAL_ERROR "Could not find a curl target after add_subdirectory()")
endif()

# link by target name so CMake wires up dependencies & paths
target_link_libraries(SparkEngine PRIVATE
  ${_CURL_TGT}
)
target_compile_definitions(SparkEngine PRIVATE
  CURL_STATICLIB
)
target_include_directories(SparkEngine PRIVATE
  "${CMAKE_SOURCE_DIR}/ThirdParty/Networking/curl/include"
)

# -------------------------------------------------------------
# Engine include directories
# -------------------------------------------------------------
target_include_directories(SparkEngine PRIVATE
  "${CMAKE_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/Source"
  "${CMAKE_SOURCE_DIR}/Spark Engine/Source"
  "${CMAKE_SOURCE_DIR}/Engine"
  "${CMAKE_SOURCE_DIR}/Engine/Core"
  "${CMAKE_SOURCE_DIR}/ThirdParty"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/miniz"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/tinyobjloader/include"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/glm"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/stb"
)

# -------------------------------------------------------------
# AngelScript integration (optional)
# -------------------------------------------------------------
if (ENABLE_ANGELSCRIPT)
  set(AS_DIR
    "${CMAKE_SOURCE_DIR}/ThirdParty/Scripting/angelscript-mirror/sdk/angelscript/projects/cmake"
  )
  if (EXISTS "${AS_DIR}/CMakeLists.txt")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    add_subdirectory("${AS_DIR}" "${CMAKE_BINARY_DIR}/angelscript" EXCLUDE_FROM_ALL)
    target_link_libraries(SparkEngine PRIVATE angelscript)
    target_compile_definitions(SparkEngine PRIVATE WITH_ANGELSCRIPT=1)
  endif()
endif()

# -------------------------------------------------------------
# Link Windows system libs
# -------------------------------------------------------------
if (WIN32)
  target_link_libraries(SparkEngine PRIVATE
    d3d11 dxgi dxguid
    Shlwapi user32 gdi32 winmm
    dbghelp windowscodecs
  )
  target_compile_definitions(SparkEngine PRIVATE
    NOMINMAX
    _CRT_SECURE_NO_WARNINGS
    WIN32_LEAN_AND_MEAN
  )
endif()

# -------------------------------------------------------------
# IDE grouping
# -------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(
  TREE "${CMAKE_SOURCE_DIR}"
  PREFIX "Source"
  FILES ${ENGINE_SRC} ${ENGINE_HDR}
)