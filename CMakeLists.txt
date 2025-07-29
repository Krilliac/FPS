cmake_minimum_required(VERSION 3.20)
project(SparkEngine LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static CRT on MSVC
option(USE_STATIC_RUNTIME "Link static CRT" ON)
if (MSVC AND USE_STATIC_RUNTIME)
  foreach(flag CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_DEBUG)
    string(REPLACE "/MD" "/MT" ${flag} "${${flag}}")
  endforeach()
endif()

# Toggles
option(ENABLE_EDITOR      "Editor UI" ON)
option(ENABLE_CONSOLE     "Console overlay" ON)
option(ENABLE_ANGELSCRIPT "AngelScript scripting" ON)
option(ENABLE_VULKAN      "Vulkan renderer" OFF)
option(ENABLE_BULLET      "Bullet physics" OFF)

# ------------  project sources  ------------
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/*.cpp" "${CMAKE_SOURCE_DIR}/*.c")
file(GLOB_RECURSE INC CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/*.h"   "${CMAKE_SOURCE_DIR}/*.hpp")
list(FILTER SRC EXCLUDE REGEX "build/|ThirdParty/")
list(FILTER INC EXCLUDE REGEX "build/|ThirdParty/")
add_executable(SparkEngine ${SRC} ${INC})

# ------------  include paths  ------------
target_include_directories(SparkEngine PRIVATE
  "${CMAKE_SOURCE_DIR}"                   # <---- project root (Engine/Core/Engine.h)
  "${CMAKE_SOURCE_DIR}/Source"
  "${CMAKE_SOURCE_DIR}/ThirdParty"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/tinyobjloader"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/glm"
  "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/stb"
)

# ------------  AngelScript  ------------
if (ENABLE_ANGELSCRIPT)
  add_subdirectory(ThirdParty/Scripting/angelscript-mirror/sdk/angelscript/projects/cmake
                   ${CMAKE_BINARY_DIR}/as)
  target_link_libraries(SparkEngine PRIVATE angelscript_static)
  target_compile_definitions(SparkEngine PRIVATE WITH_ANGELSCRIPT)
endif()

# ------------  platform libs  ------------
if (WIN32)
  target_link_libraries(SparkEngine PRIVATE d3d11 dxgi dxguid Shlwapi)
  target_compile_definitions(SparkEngine PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
elseif(APPLE)
  find_library(COCOA Cocoa REQUIRED)
  target_link_libraries(SparkEngine PRIVATE ${COCOA})
else()
  find_package(X11 REQUIRED)
  target_link_libraries(SparkEngine PRIVATE X11::X11 pthread dl)
endif()

# ------------  ide tidy  ------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(TREE "${CMAKE_SOURCE_DIR}" PREFIX "Src" FILES ${SRC} ${INC})
