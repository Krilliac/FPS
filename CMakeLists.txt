cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(SparkEngine LANGUAGES C CXX)

# -------------------------------------------------------------
# 1) Global compiler settings
# -------------------------------------------------------------
set(CMAKE_CXX_STANDARD        17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  add_compile_options(/permissive- /Zc:__cplusplus)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  # strip “-d” off static libs so we always get libcurl.lib
  set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "" FORCE)
endif()

# -------------------------------------------------------------
# 2) Build options
# -------------------------------------------------------------
option(ENABLE_EDITOR      "Build with in-engine editor UI"      ON)
option(ENABLE_CONSOLE     "External debug console overlay"      ON)
option(ENABLE_ANGELSCRIPT "Embed AngelScript scripting"         ON)

message(STATUS "Editor:      ${ENABLE_EDITOR}")
message(STATUS "Console:     ${ENABLE_CONSOLE}")
message(STATUS "AngelScript: ${ENABLE_ANGELSCRIPT}")

# -------------------------------------------------------------
# 3) Glob engine .cpp / .h (exclude ThirdParty)
# -------------------------------------------------------------
file(GLOB_RECURSE ENGINE_SRCS CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/Source/*.cpp"
  "${CMAKE_SOURCE_DIR}/Spark Engine/Source/*.cpp"
  "${CMAKE_SOURCE_DIR}/Engine/Core/*.cpp"
)
file(GLOB_RECURSE ENGINE_HDRS CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/Source/*.h"
  "${CMAKE_SOURCE_DIR}/Spark Engine/Source/*.h"
  "${CMAKE_SOURCE_DIR}/Engine/Core/*.h"
)

# -------------------------------------------------------------
# 4) Force‐include Engine.cpp / Engine.h if glob missed them
# -------------------------------------------------------------
set(ENGINE_CPP_FILE "${CMAKE_SOURCE_DIR}/Engine/Core/Engine.cpp")
if(EXISTS "${ENGINE_CPP_FILE}")
  list(FIND ENGINE_SRCS "${ENGINE_CPP_FILE}" _cpp_found)
  if(_cpp_found EQUAL -1)
    message(WARNING "Appending missing: ${ENGINE_CPP_FILE}")
    list(APPEND ENGINE_SRCS "${ENGINE_CPP_FILE}")
  endif()
endif()
set(ENGINE_HDR_FILE "${CMAKE_SOURCE_DIR}/Engine/Core/Engine.h")
if(EXISTS "${ENGINE_HDR_FILE}")
  list(FIND ENGINE_HDRS "${ENGINE_HDR_FILE}" _hdr_found)
  if(_hdr_found EQUAL -1)
    message(WARNING "Appending missing: ${ENGINE_HDR_FILE}")
    list(APPEND ENGINE_HDRS "${ENGINE_HDR_FILE}")
  endif()
endif()

# -------------------------------------------------------------
# 5) Debug print the globs
# -------------------------------------------------------------
message(STATUS "---- Found engine .cpp files: ----")
foreach(src IN LISTS ENGINE_SRCS)
  message(STATUS "  ${src}")
endforeach()
message(STATUS "-----------------------------------")
list(LENGTH ENGINE_SRCS SRCS_COUNT)
message(STATUS "Total .cpp count: ${SRCS_COUNT}")

message(STATUS "---- Found engine .h files: ----")
foreach(hdr IN LISTS ENGINE_HDRS)
  message(STATUS "  ${hdr}")
endforeach()
message(STATUS "-----------------------------------")
list(LENGTH ENGINE_HDRS HDRS_COUNT)
message(STATUS "Total .h count: ${HDRS_COUNT}")

if(SRCS_COUNT EQUAL 0)
  message(FATAL_ERROR "No engine .cpp files found! Check your paths.")
endif()

# -------------------------------------------------------------
# 6) Create the executable (WIN32 subsystem uses WinMain)
# -------------------------------------------------------------
add_executable(SparkEngine WIN32
  ${ENGINE_SRCS}
)
# inject headers so they appear in the IDE
target_sources(SparkEngine PRIVATE ${ENGINE_HDRS})

# -------------------------------------------------------------
# 7) Engine include directories
# -------------------------------------------------------------
target_include_directories(SparkEngine PRIVATE
  "${CMAKE_SOURCE_DIR}/Source"
  "${CMAKE_SOURCE_DIR}/Spark Engine/Source"
  "${CMAKE_SOURCE_DIR}/Engine/Core"
)

# -------------------------------------------------------------
# 8) ThirdParty: miniz
# -------------------------------------------------------------
add_library(miniz STATIC
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/miniz/miniz.c"
)
target_include_directories(miniz PUBLIC
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/miniz"
)
target_link_libraries(SparkEngine PRIVATE miniz)

# -------------------------------------------------------------
# 9) ThirdParty: tinyobjloader (header-only)
# -------------------------------------------------------------
set(TINYOBJ_HDR
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/tinyobjloader/include/tiny_obj_loader.h"
)
set_source_files_properties(${TINYOBJ_HDR} PROPERTIES LANGUAGE CXX)
add_library(tinyobjloader STATIC ${TINYOBJ_HDR})
target_compile_definitions(tinyobjloader PUBLIC TINYOBJLOADER_IMPLEMENTATION)
target_include_directories(tinyobjloader PUBLIC
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/tinyobjloader/include"
)
target_link_libraries(SparkEngine PRIVATE tinyobjloader)

# -------------------------------------------------------------
# 10) ThirdParty: libcurl (static via submodule)
# -------------------------------------------------------------
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_CURL_EXE    OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING     OFF CACHE BOOL "" FORCE)
set(CURL_USE_SCHANNEL ON  CACHE BOOL "" FORCE)
set(CURL_DISABLE_LDAP ON  CACHE BOOL "" FORCE)
set(CURL_ZLIB         OFF CACHE BOOL "" FORCE)
set(CURL_BROTLI       OFF CACHE BOOL "" FORCE)
set(CURL_ZSTD         OFF CACHE BOOL "" FORCE)
set(CURL_HTTP2        OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL   OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBIDN2  OFF CACHE BOOL "" FORCE)

add_subdirectory(
  "${CMAKE_SOURCE_DIR}/ThirdParty/Networking/curl"
  "${CMAKE_BINARY_DIR}/curl"
)

if(TARGET libcurl_static)
  set(_CURL_TGT libcurl_static)
elseif(TARGET CURL::libcurl)
  set(_CURL_TGT CURL::libcurl)
else()
  message(FATAL_ERROR "Could not locate curl target")
endif()

target_link_directories(SparkEngine PRIVATE
  $<TARGET_FILE_DIR:${_CURL_TGT}>
)
target_link_libraries(SparkEngine PRIVATE ${_CURL_TGT})
target_compile_definitions(SparkEngine PRIVATE CURL_STATICLIB)
target_include_directories(SparkEngine PRIVATE
  "${CMAKE_SOURCE_DIR}/ThirdParty/Networking/curl/include"
)

# -------------------------------------------------------------
# 11) ThirdParty: AngelScript (optional)
# -------------------------------------------------------------
if(ENABLE_ANGELSCRIPT)
  set(AS_DIR
    "${CMAKE_SOURCE_DIR}/ThirdParty/Scripting/angelscript-mirror/sdk/angelscript/projects/cmake"
  )
  if(EXISTS "${AS_DIR}/CMakeLists.txt")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    add_subdirectory("${AS_DIR}" "${CMAKE_BINARY_DIR}/angelscript")
    target_link_libraries(SparkEngine PRIVATE angelscript)
    target_compile_definitions(SparkEngine PRIVATE WITH_ANGELSCRIPT=1)
  endif()
endif()

# -------------------------------------------------------------
# 12) Windows system libraries & defines
# -------------------------------------------------------------
if(WIN32)
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
# 13) IDE grouping: show .cpp & .h under “Source”
# -------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(
  TREE "${CMAKE_SOURCE_DIR}"
  PREFIX "Source"
  FILES
    ${ENGINE_SRCS}
    ${ENGINE_HDRS}
)