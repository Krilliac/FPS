cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(SparkEngine LANGUAGES C CXX)

# -------------------------------------------------------------
# 1) Global compiler settings
# -------------------------------------------------------------
set(CMAKE_CXX_STANDARD        17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(MSVC)
  add_compile_options(/permissive- /Zc:__cplusplus)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "" FORCE)
endif()

# -------------------------------------------------------------
# 2) Build options + verbosity toggle
# -------------------------------------------------------------
option(ENABLE_EDITOR        "Build with in-engine editor UI"      ON)
option(ENABLE_CONSOLE       "External debug console overlay"      ON)
option(ENABLE_ANGELSCRIPT   "Embed AngelScript scripting"         ON)
option(VERBOSE_DIAGNOSTICS  "Print extra CMake diagnostics"       OFF)

# Helper for conditional diagnostic messages
function(msg_diag)
  if(VERBOSE_DIAGNOSTICS)
    message(STATUS "[diag] ${ARGN}")
  endif()
endfunction()

message(STATUS "Editor:      ${ENABLE_EDITOR}")
message(STATUS "Console:     ${ENABLE_CONSOLE}")
message(STATUS "AngelScript: ${ENABLE_ANGELSCRIPT}")
msg_diag("VERBOSE_DIAGNOSTICS = ${VERBOSE_DIAGNOSTICS}")

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
# 4) Force-include Engine.cpp / Engine.h if glob missed them
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
# 5) Debug-print the globs
# -------------------------------------------------------------
message(STATUS "---- Found engine .cpp files: ----")
foreach(src IN LISTS ENGINE_SRCS)
  msg_diag("${src}")
endforeach()
list(LENGTH ENGINE_SRCS SRCS_COUNT)
message(STATUS "Total .cpp count: ${SRCS_COUNT}")

message(STATUS "---- Found engine .h files: ----")
foreach(hdr IN LISTS ENGINE_HDRS)
  msg_diag("${hdr}")
endforeach()
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
target_sources(SparkEngine PRIVATE ${ENGINE_HDRS})

# Ensure the Shaders folder exists next to the exe in the build tree:
set(SHADERS_SRC_DIR "${CMAKE_SOURCE_DIR}/Spark Engine/Shaders/HLSL")
set(SHADERS_DST_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders/HLSL")

# Recursively copy HLSL shaders into the runtime output dir
file(GLOB_RECURSE SHADER_FILES
     "${SHADERS_SRC_DIR}/*.hlsl"
)

# Create destination directory
file(MAKE_DIRECTORY "${SHADERS_DST_DIR}")

# Copy each file preserving its filename
foreach(_shader ${SHADER_FILES})
    get_filename_component(_fname ${_shader} NAME)
    configure_file(
        "${_shader}"
        "${SHADERS_DST_DIR}/${_fname}"
        COPYONLY
    )
endforeach()

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
# 9) ThirdParty: tinyobjloader (compile implementation once)
# -------------------------------------------------------------
set(TINYOBJ_IMPL_PATH
  "${CMAKE_SOURCE_DIR}/Spark Engine/Source/Utils/TinyObjImpl.cpp"
)
msg_diag("Expecting TinyObjImpl at: ${TINYOBJ_IMPL_PATH}")
if(NOT EXISTS "${TINYOBJ_IMPL_PATH}")
  message(FATAL_ERROR "TinyObjImpl.cpp NOT found at: ${TINYOBJ_IMPL_PATH}")
endif()

add_library(tinyobjloader STATIC
  "${TINYOBJ_IMPL_PATH}"
)
target_include_directories(tinyobjloader PUBLIC
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/tinyobjloader/include"
)
target_link_libraries(SparkEngine PRIVATE tinyobjloader)

# -------------------------------------------------------------
# 10) ThirdParty: libcurl (static via submodule)
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