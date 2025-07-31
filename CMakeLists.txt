cmake_minimum_required(VERSION 3.20)
project(SparkEngine VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find packages
find_package(directx REQUIRED)

# ===============================
# THIRD PARTY DEPENDENCIES
# ===============================

# EnTT - Header only ECS library
add_subdirectory(ThirdParty/entt)

# ImGui
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/imgui")
add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
    ${IMGUI_DIR}/backends/imgui_impl_dx11.cpp
)
target_include_directories(imgui PUBLIC 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)
target_link_libraries(imgui PUBLIC d3d11 dxgi)

# Sol2 - Lua binding
add_subdirectory(ThirdParty/sol2)

# PhysX (assuming precompiled libraries)
set(PHYSX_ROOT_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/PhysX")
if(EXISTS "${PHYSX_ROOT_DIR}")
    add_library(PhysX INTERFACE)
    target_include_directories(PhysX INTERFACE "${PHYSX_ROOT_DIR}/include")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(PHYSX_LIB_DIR "${PHYSX_ROOT_DIR}/lib/debug")
    else()
        set(PHYSX_LIB_DIR "${PHYSX_ROOT_DIR}/lib/release")
    endif()

    target_link_libraries(PhysX INTERFACE
        "${PHYSX_LIB_DIR}/PhysX_64.lib"
        "${PHYSX_LIB_DIR}/PhysXCommon_64.lib"
        "${PHYSX_LIB_DIR}/PhysXFoundation_64.lib"
        "${PHYSX_LIB_DIR}/PhysXPvdSDK_64.lib"
    )
endif()

# STB Image - Header only image loader
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE "ThirdParty/stb")

# ===============================
# SPARKENGINE LIBRARY
# ===============================

# Collect all source files
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "Spark Engine/Source/**/*.cpp" "Spark Engine/Source/**/*.h")

# Create the main executable
add_executable(SparkEngine WIN32
    ${ENGINE_SOURCES}
    "${CMAKE_SOURCE_DIR}/Spark Engine/SparkEngine.rc"
)

# Include directories
target_include_directories(SparkEngine PRIVATE
    "${CMAKE_SOURCE_DIR}/Spark Engine/Source"
    "${CMAKE_SOURCE_DIR}/ThirdParty"
)

# Link libraries
target_link_libraries(SparkEngine PRIVATE
    d3d11
    dxgi
    d3dcompiler
    winmm
    EnTT::EnTT
    imgui
    sol2::sol2
    stb_image
)

# Link PhysX if available
if(TARGET PhysX)
    target_link_libraries(SparkEngine PRIVATE PhysX)
    target_compile_definitions(SparkEngine PRIVATE SPARKENGINE_PHYSICS_ENABLED)
endif()

# Compiler-specific options
if(MSVC)
    target_compile_options(SparkEngine PRIVATE
        /W4          # Warning level 4
        /permissive- # Strict C++ conformance
        /Zc:__cplusplus # Enable correct __cplusplus macro
    )

    # Set runtime library
    set_property(TARGET SparkEngine PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # Enable multiprocessor compilation
    target_compile_options(SparkEngine PRIVATE /MP)
endif()

# Platform definitions
target_compile_definitions(SparkEngine PRIVATE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    UNICODE
    _UNICODE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Debug>:_DEBUG>
)

# Copy DLLs to output directory (if they exist)
if(EXISTS "${CMAKE_SOURCE_DIR}/ThirdParty/PhysX/bin")
    add_custom_command(TARGET SparkEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/ThirdParty/PhysX/bin"
        $<TARGET_FILE_DIR:SparkEngine>
        COMMENT "Copying PhysX DLLs"
    )
endif()

# Create folders in IDE
source_group(TREE "${CMAKE_SOURCE_DIR}/Spark Engine/Source" PREFIX "Source" FILES ${ENGINE_SOURCES})

# Set startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SparkEngine)

message(STATUS "SparkEngine configuration complete")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

# Include new systems"
add_subdirectory("Spark Engine/Source/Graphics/Systems")
