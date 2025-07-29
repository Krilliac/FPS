cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(SparkEngine LANGUAGES C CXX)

# -------------------------------------------------------------
# Global compiler setup
# -------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
  add_compile_options(/permissive- /Zc:__cplusplus)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# -------------------------------------------------------------
# Engine build options
# -------------------------------------------------------------
option(ENABLE_EDITOR      "Build with in-engine editor UI" ON)
option(ENABLE_CONSOLE     "External debug console overlay" ON)
option(ENABLE_ANGELSCRIPT "Embed AngelScript scripting" ON)

message(STATUS "Editor..............: ${ENABLE_EDITOR}")
message(STATUS "Console.............: ${ENABLE_CONSOLE}")
message(STATUS "AngelScript.........: ${ENABLE_ANGELSCRIPT}")

# -------------------------------------------------------------
# Gather engine sources & headers
# -------------------------------------------------------------
file(GLOB_RECURSE ENGINE_SRC
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/*.cpp"
  "${CMAKE_SOURCE_DIR}/*.c"
)
file(GLOB_RECURSE ENGINE_INC
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/*.h"
  "${CMAKE_SOURCE_DIR}/*.hpp"
)

list(FILTER ENGINE_SRC EXCLUDE REGEX "build/|ThirdParty/")
list(FILTER ENGINE_INC EXCLUDE REGEX "build/|ThirdParty/")
message(STATUS "Found ${ENGINE_SRC_COUNT} source files")

if (NOT ENGINE_SRC)
  message(FATAL_ERROR "No source files found!")
endif()

# -------------------------------------------------------------
# Primary executable target
# -------------------------------------------------------------
add_executable(SparkEngine
  ${ENGINE_SRC}
  ${ENGINE_INC}
)

# -------------------------------------------------------------
# ThirdParty integration (Miniz, tinyobjloader, libcurl)
# -------------------------------------------------------------
# <<< ThirdParty Integration >>>

# Miniz
if (NOT TARGET miniz)
  add_library(miniz
    "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/miniz/miniz.c"
  )
  target_include_directories(miniz PUBLIC
    "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/miniz"
  )
endif()
target_link_libraries(SparkEngine PRIVATE miniz)

# tinyobjloader
if (NOT TARGET tinyobjloader)
  add_library(tinyobjloader INTERFACE)
  target_include_directories(tinyobjloader INTERFACE
    "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/tinyobjloader/include"
  )
endif()
target_link_libraries(SparkEngine PRIVATE tinyobjloader)

# libcurl build from submodule
set(BUILD_SHARED_LIBS      OFF CACHE BOOL "" FORCE)
set(BUILD_CURL_EXE         OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING          OFF CACHE BOOL "" FORCE)
set(CURL_USE_SCHANNEL      ON  CACHE BOOL "" FORCE)
set(CURL_DISABLE_LDAP      ON  CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL        OFF CACHE BOOL "" FORCE)    # <— disable libpsl [1]
set(CURL_ZLIB              OFF CACHE BOOL "" FORCE)    # <— disable zlib
set(CURL_BROTLI            OFF CACHE BOOL "" FORCE)    # <— disable brotli
set(CURL_ZSTD              OFF CACHE BOOL "" FORCE)    # <— disable zstd
set(CURL_HTTP2             OFF CACHE BOOL "" FORCE)    # <— disable nghttp2
set(CURL_USE_LIBIDN2       OFF CACHE BOOL "" FORCE)    # <— disable libidn2

add_subdirectory(
  "${CMAKE_SOURCE_DIR}/ThirdParty/Networking/curl"
  "${CMAKE_BINARY_DIR}/curl"
)

target_link_libraries(SparkEngine PRIVATE libcurl)     # libcurl target from curl subdir
target_include_directories(SparkEngine PRIVATE
  "${CMAKE_SOURCE_DIR}/ThirdParty/Networking/curl/install/include"
)
target_compile_definitions(SparkEngine PRIVATE CURL_STATICLIB)

# <<< End ThirdParty Integration >>>

# -------------------------------------------------------------
# Engine include dirs
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
  "${CMAKE_SOURCE_DIR}/ThirdParty/Networking/curl/install/include"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/glm"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/stb"
)

# -------------------------------------------------------------
# AngelScript integration
# -------------------------------------------------------------
if (ENABLE_ANGELSCRIPT)
  set(AS_CMAKE_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/Scripting/angelscript-mirror/sdk/angelscript/projects/cmake")
  if (EXISTS "${AS_CMAKE_DIR}/CMakeLists.txt")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    add_subdirectory("${AS_CMAKE_DIR}" "${CMAKE_BINARY_DIR}/angelscript")
    target_link_libraries(SparkEngine PRIVATE angelscript_static)
    target_compile_definitions(SparkEngine PRIVATE WITH_ANGELSCRIPT=1)
  endif()
endif()

# -------------------------------------------------------------
# Platform system libs
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
    CURL_STATICLIB
  )
endif()

# -------------------------------------------------------------
# IDE organization
# -------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(
  TREE "${CMAKE_SOURCE_DIR}"
  PREFIX "Source"
  FILES ${ENGINE_SRC} ${ENGINE_INC}
)