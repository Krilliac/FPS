cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(SparkEngine LANGUAGES C CXX)

# â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ Global compiler setup â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional static CRT on MSVC
option(USE_STATIC_RUNTIME "Link the MSVC static runtime (/MT)" ON)
if (MSVC AND USE_STATIC_RUNTIME)
  foreach(flag CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_DEBUG)
    string (REPLACE "/MD" "/MT" ${flag} "${${flag}}")
  endforeach()
endif()

# â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ Configurable Engine Options â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
option(ENABLE_EDITOR      "Build with in-engine editor UI"                  ON)
option(ENABLE_CONSOLE     "External debug console overlay"                  ON)
option(ENABLE_ANGELSCRIPT "Embed AngelScript scripting (requires submodule)" ON)
option(ENABLE_VULKAN      "Enable Vulkan renderer in addition to D3D11"      OFF)

# Convenience for command-line generators
message(STATUS "Editor..............: ${ENABLE_EDITOR}")
message(STATUS "Console.............: ${ENABLE_CONSOLE}")
message(STATUS "AngelScript.........: ${ENABLE_ANGELSCRIPT}")
message(STATUS "Static Runtime (MT).: ${USE_STATIC_RUNTIME}")
message(STATUS "Vulkan backend......: ${ENABLE_VULKAN}")

# â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ Auto-discover engine sources â”€â”€â”€â”€â”€â”€â”€â”€â”€
file(GLOB_RECURSE ENGINE_SRC  CONFIGURE_DEPENDS  Source/*.cpp  Source/*.c)
file(GLOB_RECURSE ENGINE_INC  CONFIGURE_DEPENDS  Source/*.h    Source/*.hpp)

add_executable(SparkEngine  ${ENGINE_SRC}  ${ENGINE_INC})

target_include_directories(SparkEngine
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty
)

target_compile_definitions(SparkEngine PRIVATE _CRT_SECURE_NO_WARNINGS)

# â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ Third-party headers (header-only) â”€â”€â”€â”€
target_include_directories(SparkEngine PRIVATE
    ThirdParty/Utils/glm
    ThirdParty/Utils/stb
)

# â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ AngelScript (optional) â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
if (ENABLE_ANGELSCRIPT)
  add_subdirectory(ThirdParty/Scripting/angelscript
                   EXCLUDE_FROM_ALL)           # builds libangelscript
  target_link_libraries(SparkEngine PRIVATE angelscript_static)
  target_compile_definitions(SparkEngine PRIVATE WITH_ANGELSCRIPT=1)
endif()

# â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ Bullet Physics (toggle as example) â”€â”€â”€
option(ENABLE_BULLET "Bullet physics support" OFF)
if (ENABLE_BULLET)
  add_subdirectory(ThirdParty/Physics/bullet3 EXCLUDE_FROM_ALL)
  target_link_libraries(SparkEngine PRIVATE BulletDynamics BulletCollision LinearMath)
endif()

# â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ Platform-specific libs â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
if (WIN32)
  target_link_libraries(SparkEngine PRIVATE d3d11 dxgi dxguid Shlwapi)
  target_compile_definitions(SparkEngine PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
elseif(APPLE)
  find_library(COCOA Cocoa REQUIRED)
  target_link_libraries(SparkEngine PRIVATE ${COCOA})
else() # Linux
  find_package(X11 REQUIRED)
  target_link_libraries(SparkEngine PRIVATE X11::X11 pthread dl)
endif()

# â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ IDE niceties â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source FILES ${ENGINE_SRC} ${ENGINE_INC})

