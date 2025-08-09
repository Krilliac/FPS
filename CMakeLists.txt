cmake_minimum_required(VERSION 3.16)

# ================================================================
# Modern CMake Configuration
# ================================================================

# Set CMake policies for modern behavior
cmake_policy(SET CMP0091 NEW)  # Enable MSVC_RUNTIME_LIBRARY support

# Set consistent runtime library for all targets
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# ---------------------------------------------------------------------
# 1) Disable all third-party tests/examples globally
# ---------------------------------------------------------------------
set(BUILD_TESTING OFF CACHE BOOL "Disable all testing" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable all examples" FORCE)
set(BUILD_SAMPLES OFF CACHE BOOL "Disable all samples" FORCE)
set(BUILD_TOOLS OFF CACHE BOOL "Disable all tools" FORCE)
set(BUILD_DOCS OFF CACHE BOOL "Disable docs" FORCE)

# ---------------------------------------------------------------------
# 2) Global generator and project settings
# ---------------------------------------------------------------------
# Platform-specific generator settings (only if not already set)
if(WIN32 AND NOT CMAKE_GENERATOR_TOOLSET)
    set(CMAKE_GENERATOR_TOOLSET "v143")
endif()
if(WIN32 AND NOT CMAKE_GENERATOR_PLATFORM)
    set(CMAKE_GENERATOR_PLATFORM "x64")
endif()

project(SparkEngine)

# ---------------------------------------------------------------------
# 3) Custom build options
# ---------------------------------------------------------------------
option(ENABLE_PHYSX "Enable PhysX physics engine" ON)
option(ENABLE_EDITOR "Enable editor features" ON)
option(ENABLE_LUA "Enable Lua scripting support" ON)
option(ENABLE_COLLABORATIVE "Enable collaborative features" ON)
option(ENABLE_GRAPHICS "Enable graphics engine" ON)
option(ENABLE_PROFILING "Enable profiling tools" ON)
option(ENABLE_NETWORKING "Enable networking features" ON)
option(ENABLE_ASSET_STREAMING "Enable asset streaming" ON)
option(ENABLE_HOT_RELOAD "Enable hot reload functionality" ON)
option(ENABLE_TERRAIN_SYSTEM "Enable terrain system" ON)
option(ENABLE_POST_PROCESSING "Enable post-processing effects" ON)
option(ENABLE_LIGHTING_SYSTEM "Enable advanced lighting" ON)
option(ENABLE_ADVANCED_INPUT "Enable advanced input features" ON)

# ---------------------------------------------------------------------
# 4) C++ standard and in-source build guard
# ---------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed. Create a separate build directory.")
endif()

# ---------------------------------------------------------------------
# 5) Output directories - ENSURE BOTH EXECUTABLES GO TO SAME DIRECTORY
# ---------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Make sure all configurations use the same output directory
foreach(config Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${config} CONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR}/lib)
endforeach()

# ---------------------------------------------------------------------
# 6) MSVC-specific compiler options
# ---------------------------------------------------------------------
if(MSVC)
    add_compile_options(
        /W3 /MP /bigobj
        /wd4005  # Macro redefinition
        /wd4996  # Deprecated functions
        /wd4244  # Conversion warnings
        /wd4267  # size_t conversion warnings
        /wd26495 # Uninitialized member variable warnings (for TinyObjLoader)
    )
    add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS)
endif()

# ---------------------------------------------------------------------
# 7) Third-party library sources (curl, miniz, tinyobjloader)
# ---------------------------------------------------------------------

# CURL
set(CURL_ROOT "${CMAKE_SOURCE_DIR}/ThirdParty/Networking/curl")
if(EXISTS "${CURL_ROOT}")
    file(GLOB_RECURSE CURL_SOURCES
        "${CURL_ROOT}/lib/*.c"
        "${CURL_ROOT}/lib/*.h"
    )
    list(FILTER CURL_SOURCES EXCLUDE REGEX ".*test.*|.*example.*|.*tool.*")
endif()

# MINIZ
set(MINIZ_ROOT "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/miniz")
if(EXISTS "${MINIZ_ROOT}")
    file(GLOB_RECURSE MINIZ_SOURCES
        "${MINIZ_ROOT}/*.c"
        "${MINIZ_ROOT}/*.h"
    )
    list(FILTER MINIZ_SOURCES EXCLUDE REGEX ".*test.*|.*example.*|.*tool.*")
endif()

# TinyObjLoader - Proper Implementation
find_path(TINYOBJLOADER_INCLUDE_DIR 
    NAMES tiny_obj_loader.h
    PATHS 
        "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/tinyobjloader"
        "${CMAKE_SOURCE_DIR}/ThirdParty/FileFormats/tinyobjloader"
        "${CMAKE_SOURCE_DIR}/ThirdParty/Graphics/tinyobjloader"
        "${CMAKE_SOURCE_DIR}/ThirdParty/tinyobjloader"
        "${CMAKE_SOURCE_DIR}/ThirdParty/Utils/tinyobjloader/include"
        "${CMAKE_SOURCE_DIR}/ThirdParty/FileFormats/tinyobjloader/include"
    DOC "TinyObjLoader include directory"
)

if(TINYOBJLOADER_INCLUDE_DIR)
    # Create a proper library target for TinyObjLoader
    add_library(tinyobjloader INTERFACE)
    target_include_directories(tinyobjloader INTERFACE ${TINYOBJLOADER_INCLUDE_DIR})
    target_compile_definitions(tinyobjloader INTERFACE TINYOBJLOADER_IMPLEMENTATION)
    
    set(TINYOBJ_FOUND TRUE)
    message(STATUS "Found TinyObjLoader at: ${TINYOBJLOADER_INCLUDE_DIR}")
else()
    set(TINYOBJ_FOUND FALSE)
    message(WARNING "TinyObjLoader not found")
endif()

# ---------------------------------------------------------------------
# 8) Build SparkConsole FIRST to ensure it's available for SparkEngine
# ---------------------------------------------------------------------
if(EXISTS "${CMAKE_SOURCE_DIR}/SparkConsole/CMakeLists.txt")
    message(STATUS "Building SparkConsole first...")
    add_subdirectory(SparkConsole)
endif()

# ---------------------------------------------------------------------
# 9) SparkEngine source files
# ---------------------------------------------------------------------
file(GLOB_RECURSE SPARK_ENGINE_SOURCES
    CONFIGURE_DEPENDS
    "Spark Engine/Source/*.cpp"
    "Spark Engine/Source/*.h"
    "Spark Engine/Source/*.hpp"
)
list(FILTER SPARK_ENGINE_SOURCES EXCLUDE REGEX ".*[Tt]est.*")
list(FILTER SPARK_ENGINE_SOURCES EXCLUDE REGEX ".*[Ee]xample.*")

# Create the main executable
add_executable(SparkEngine WIN32 ${SPARK_ENGINE_SOURCES})

# Add third-party sources if they exist
if(CURL_SOURCES)
    target_sources(SparkEngine PRIVATE ${CURL_SOURCES})
    target_compile_definitions(SparkEngine PRIVATE NETWORKING_ENABLED CURL_STATICLIB)
    message(STATUS "Added CURL sources to build")
endif()

if(MINIZ_SOURCES)
    target_sources(SparkEngine PRIVATE ${MINIZ_SOURCES})
    message(STATUS "Added MINIZ sources to build")
endif()

if(TINYOBJ_FOUND)
    target_link_libraries(SparkEngine PRIVATE tinyobjloader)
    message(STATUS "Added TinyObjLoader to build")
endif()

# Ensure SparkEngine depends on SparkConsole
if(TARGET SparkConsole)
    add_dependencies(SparkEngine SparkConsole)
    message(STATUS "SparkEngine will be built after SparkConsole")
    
    # Add a post-build step to copy SparkConsole.exe to the same directory as SparkEngine.exe
    add_custom_command(TARGET SparkEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SparkConsole>
            $<TARGET_FILE_DIR:SparkEngine>/$<TARGET_FILE_NAME:SparkConsole>
        COMMENT "Copying SparkConsole to SparkEngine output directory"
    )
endif()

# ---------------------------------------------------------------------
# 9.5) Build SparkEditor if enabled and exists
# ---------------------------------------------------------------------
if(ENABLE_EDITOR AND EXISTS "${CMAKE_SOURCE_DIR}/SparkEditor/CMakeLists.txt")
    message(STATUS "Adding SparkEditor to build...")
    add_subdirectory(SparkEditor)
    
    # Ensure SparkEditor is built after SparkEngine
    if(TARGET SparkEngine AND TARGET SparkEditor)
        add_dependencies(SparkEditor SparkEngine)
        message(STATUS "SparkEditor will be built after SparkEngine")
    endif()
endif()

# ---------------------------------------------------------------------
# 10) Asset Management
# ---------------------------------------------------------------------
# Create necessary directories
add_custom_command(TARGET SparkEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Shaders"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Assets"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Models"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Textures"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Audio"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Scripts"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Config"
    COMMENT "Creating asset directory structure"
)

# ---------------------------------------------------------------------
# 11) Include directories
# ---------------------------------------------------------------------
target_include_directories(SparkEngine PRIVATE
    "Spark Engine/Source"
)

# Add CURL headers if available
if(EXISTS "${CURL_ROOT}/include")
    target_include_directories(SparkEngine PRIVATE "${CURL_ROOT}/include")
endif()

# Add MINIZ headers if available
if(EXISTS "${MINIZ_ROOT}")
    target_include_directories(SparkEngine PRIVATE "${MINIZ_ROOT}")
endif()

# ---------------------------------------------------------------------
# 12) System libraries
# ---------------------------------------------------------------------
target_link_libraries(SparkEngine PRIVATE
    d3d11 dxgi d3dcompiler dxguid
    winmm ws2_32 wsock32
    ole32 oleaut32 uuid
    kernel32 user32 gdi32 winspool
    shell32 comdlg32 advapi32
    crypt32 wldap32 normaliz
)

# ---------------------------------------------------------------------
# 13) Lua, PhysX, and other third-party includes
# ---------------------------------------------------------------------
if(ENABLE_LUA)
    foreach(PATH IN ITEMS
        "ThirdParty/Scripting/lua"
        "ThirdParty/Scripting/sol2/single/include"
        "ThirdParty/sol2/single/include"
    )
        if(EXISTS "${CMAKE_SOURCE_DIR}/${PATH}")
            if(PATH MATCHES "sol2")
                add_library(sol2 INTERFACE)
                target_include_directories(sol2 INTERFACE "${CMAKE_SOURCE_DIR}/${PATH}")
                target_link_libraries(SparkEngine PRIVATE sol2)
                target_compile_definitions(SparkEngine PRIVATE LUA_SCRIPTING_ENABLED)
                message(STATUS "Found Sol2 at: ${PATH}")
            else()
                target_include_directories(SparkEngine PRIVATE "${CMAKE_SOURCE_DIR}/${PATH}")
                message(STATUS "Found Lua headers at: ${PATH}")
            endif()
        endif()
    endforeach()
endif()

if(ENABLE_PHYSX)
    if(EXISTS "${CMAKE_SOURCE_DIR}/ThirdParty/Physics/PhysX/include")
        target_include_directories(SparkEngine PRIVATE "ThirdParty/Physics/PhysX/include")
        target_compile_definitions(SparkEngine PRIVATE PHYSX_AVAILABLE)
        message(STATUS "PhysX headers found")
    else()
        message(WARNING "PhysX not found")
    endif()
endif()

# ---------------------------------------------------------------------
# 14) Feature definitions
# ---------------------------------------------------------------------
set(FEATURE_DEFINITIONS "")

if(ENABLE_EDITOR)
    list(APPEND FEATURE_DEFINITIONS "EDITOR_ENABLED")
endif()

if(ENABLE_GRAPHICS)
    list(APPEND FEATURE_DEFINITIONS "GRAPHICS_ENABLED")
endif()

if(ENABLE_COLLABORATIVE)
    list(APPEND FEATURE_DEFINITIONS "COLLABORATIVE_ENABLED")
endif()

if(ENABLE_PROFILING)
    list(APPEND FEATURE_DEFINITIONS "PROFILING_ENABLED")
endif()

if(ENABLE_ASSET_STREAMING)
    list(APPEND FEATURE_DEFINITIONS "ASSET_STREAMING_ENABLED")
endif()

if(ENABLE_HOT_RELOAD)
    list(APPEND FEATURE_DEFINITIONS "HOT_RELOAD_ENABLED")
endif()

if(ENABLE_TERRAIN_SYSTEM)
    list(APPEND FEATURE_DEFINITIONS "TERRAIN_SYSTEM_ENABLED")
endif()

if(ENABLE_POST_PROCESSING)
    list(APPEND FEATURE_DEFINITIONS "POST_PROCESSING_ENABLED")
endif()

if(ENABLE_LIGHTING_SYSTEM)
    list(APPEND FEATURE_DEFINITIONS "LIGHTING_SYSTEM_ENABLED")
endif()

if(ENABLE_ADVANCED_INPUT)
    list(APPEND FEATURE_DEFINITIONS "ADVANCED_INPUT_ENABLED")
endif()

# Apply feature definitions to target
if(FEATURE_DEFINITIONS)
    target_compile_definitions(SparkEngine PRIVATE ${FEATURE_DEFINITIONS})
endif()

# ---------------------------------------------------------------------
# 15) Visual Studio settings
# ---------------------------------------------------------------------
foreach(src ${SPARK_ENGINE_SOURCES})
    get_filename_component(dir "${src}" DIRECTORY)
    file(RELATIVE_PATH grp "${CMAKE_SOURCE_DIR}/Spark Engine/Source" "${dir}")
    string(REPLACE "/" "\\\\" grp "${grp}")
    if(grp STREQUAL "")
        source_group("Source Files" FILES "${src}")
    else()
        source_group("Source Files\\\\${grp}" FILES "${src}")
    endif()
endforeach()

if(MSVC)
    set_property(TARGET SparkEngine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SparkEngine)
    source_group("Assets\\\\Shaders" FILES "*.hlsl" "*.fx" "*.glsl")
    source_group("Assets\\\\Models" FILES "*.obj" "*.fbx" "*.dae" "*.3ds" "*.ply")
    source_group("Assets\\\\Textures" FILES "*.png" "*.jpg" "*.jpeg" "*.tga" "*.dds" "*.bmp")
    source_group("Assets\\\\Audio" FILES "*.wav" "*.mp3" "*.ogg" "*.flac")
    source_group("Assets\\\\Scripts" FILES "*.lua" "*.js" "*.py")
    source_group("Assets\\\\Config" FILES "*.ini" "*.json" "*.cfg" "*.xml" "*.properties")
endif()

message(STATUS "SPARKENGINE CONFIGURED SUCCESSFULLY")
message(STATUS "SparkEngine and SparkConsole will be built to: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
