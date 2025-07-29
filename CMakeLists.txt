cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(SparkEngine LANGUAGES C CXX)

# -------------------------------------------------------------
# Global compiler setup
# -------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/permissive- /Zc:__cplusplus)
    # Static runtime linking
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# -------------------------------------------------------------
# Engine Options
# -------------------------------------------------------------
option(ENABLE_EDITOR      "Build with in-engine editor UI"      ON)
option(ENABLE_CONSOLE     "External debug console overlay"      ON)
option(ENABLE_ANGELSCRIPT "Embed AngelScript scripting"         ON)

message(STATUS "Editor..............: ${ENABLE_EDITOR}")
message(STATUS "Console.............: ${ENABLE_CONSOLE}")
message(STATUS "AngelScript.........: ${ENABLE_ANGELSCRIPT}")

# -------------------------------------------------------------
# Find all source files
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

# Filter out build and third-party
list(FILTER ENGINE_SRC EXCLUDE REGEX "build/|ThirdParty/")
list(FILTER ENGINE_INC EXCLUDE REGEX "build/|ThirdParty/")

list(LENGTH ENGINE_SRC SRC_COUNT)
message(STATUS "Found ${SRC_COUNT} source files")

if(NOT ENGINE_SRC)
    message(FATAL_ERROR "No source files found!")
endif()

add_executable(SparkEngine ${ENGINE_SRC} ${ENGINE_INC})
# <<< Miniz Integration >>>
#--- Begin Miniz Integration ---
if (NOT TARGET miniz)
    add_library(miniz
        "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/miniz/miniz.c"
    )
    target_include_directories(miniz PUBLIC
        "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/miniz"
    )
endif()
target_link_libraries(SparkEngine PRIVATE miniz)
#--- End Miniz Integration ---
# <<< End Miniz Integration >>>

#--- Begin tinyobjloader Integration ---
if (NOT TARGET tinyobjloader)
  add_library(tinyobjloader INTERFACE)
  target_include_directories(tinyobjloader INTERFACE
    "\/ThirdParty/Utils/tinyobjloader/include"
  )
endif()
target_link_libraries(SparkEngine PRIVATE miniz tinyobjloader)
#--- End tinyobjloader Integration ---
# <<< ThirdParty Integration >>>

# -------------------------------------------------------------
# Comprehensive include directories
# -------------------------------------------------------------
target_include_directories(SparkEngine PRIVATE
    # Project roots
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/Source"
    "${CMAKE_SOURCE_DIR}/Spark Engine/Source"
    
    # Engine directories
    "${CMAKE_SOURCE_DIR}/Engine"
    "${CMAKE_SOURCE_DIR}/Engine/Core"
    "${CMAKE_SOURCE_DIR}/Source/Core"
    "${CMAKE_SOURCE_DIR}/Spark Engine/Source/Core"
    
    # Third-party
    "${CMAKE_SOURCE_DIR}/ThirdParty"
    "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/tinyobjloader"
    "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/glm"
    "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/stb"
)

# Compiler definitions
target_compile_definitions(SparkEngine PRIVATE
    _CRT_SECURE_NO_WARNINGS
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
)

# -------------------------------------------------------------
# AngelScript
# -------------------------------------------------------------
if (ENABLE_ANGELSCRIPT)
    set(AS_CMAKE_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/Scripting/angelscript-mirror/sdk/angelscript/projects/cmake")
    if (EXISTS "${AS_CMAKE_DIR}/CMakeLists.txt")
        add_subdirectory("${AS_CMAKE_DIR}" "${CMAKE_BINARY_DIR}/angelscript")
        target_link_libraries(SparkEngine PRIVATE angelscript_static)
        target_compile_definitions(SparkEngine PRIVATE WITH_ANGELSCRIPT=1)
    endif()
endif()

# -------------------------------------------------------------
# Platform libraries
# -------------------------------------------------------------
if (WIN32)
    target_link_libraries(SparkEngine PRIVATE
        d3d11 dxgi dxguid
        Shlwapi user32 gdi32 winmm
        dbghelp
    )
endif()

# -------------------------------------------------------------
# IDE organization
# -------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(TREE "${CMAKE_SOURCE_DIR}" PREFIX "Source" FILES ${ENGINE_SRC} ${ENGINE_INC})
# ---- auto-fix include paths ----
target_include_directories(SparkEngine PRIVATE
  "${CMAKE_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/Source"
  "${CMAKE_SOURCE_DIR}/Spark Engine/Source"
  "${CMAKE_SOURCE_DIR}/Engine"
  "${CMAKE_SOURCE_DIR}/Engine/Core"
  "${CMAKE_SOURCE_DIR}/ThirdParty"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/miniz"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/tinyobjloader"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/tinyobjloader/include"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Networking/curl/include"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/glm"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/stb"
)

find_library(CURL_LIB
  NAMES libcurl_a libcurl
  PATHS "${CMAKE_SOURCE_DIR}/ThirdParty/Networking/curl/lib"
  NO_DEFAULT_PATH
)
if (CURL_LIB)
  target_link_libraries(SparkEngine PRIVATE ${CURL_LIB})
endif()

