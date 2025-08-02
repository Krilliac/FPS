cmake_minimum_required(VERSION 3.16)

# Set the generator and toolset before project()
if(WIN32)
    set(CMAKE_GENERATOR_TOOLSET "v143")
    set(CMAKE_GENERATOR_PLATFORM "x64")
endif()

project(SparkEngine)

# Custom build options
option(ENABLE_PHYSX "Enable PhysX physics engine" ON)
option(ENABLE_EDITOR "Enable editor features" ON)
option(ENABLE_LUA "Enable Lua scripting support" ON)
option(ENABLE_COLLABORATIVE "Enable collaborative features" ON)
option(ENABLE_ADVANCED_GRAPHICS "Enable advanced graphics features" ON)
option(ENABLE_PROFILING "Enable profiling tools" ON)
option(ENABLE_NETWORKING "Enable networking features" ON)
option(ENABLE_ASSET_STREAMING "Enable asset streaming" ON)
option(ENABLE_HOT_RELOAD "Enable hot reload functionality" ON)
option(ENABLE_TERRAIN_SYSTEM "Enable terrain system" ON)
option(ENABLE_POST_PROCESSING "Enable post-processing effects" ON)
option(ENABLE_LIGHTING_SYSTEM "Enable advanced lighting" ON)
option(ENABLE_ADVANCED_INPUT "Enable advanced input features" ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Prevent in-source builds
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed. Please create a separate build directory.")
endif()

# Output directories  
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiler-specific options
if(MSVC)
    add_compile_options(/W3 /MP /bigobj)
    add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS)
    # Suppress specific warnings
    add_compile_options(
        /wd4005  # Macro redefinition
        /wd4996  # Deprecated functions
        /wd4244  # Conversion warnings
        /wd4267  # Size_t conversion
        /wd2334  # Unexpected token/function body warnings
    )
endif()

# Find and configure EnTT
set(ENTT_FOUND FALSE)
set(ENTT_SEARCH_PATHS
    "ThirdParty/entt/single_include"
    "ThirdParty/ECS/entt/single_include"
    "third-party/entt/single_include"
    "external/entt/single_include"
    "ThirdParty/entt"
    "third-party/entt"
)

foreach(ENTT_PATH ${ENTT_SEARCH_PATHS})
    if(EXISTS "${CMAKE_SOURCE_DIR}/${ENTT_PATH}/entt/entt.hpp")
        add_library(entt INTERFACE)
        target_include_directories(entt INTERFACE "${CMAKE_SOURCE_DIR}/${ENTT_PATH}")
        set(ENTT_FOUND TRUE)
        message(STATUS "Found EnTT at: ${ENTT_PATH}")
        break()
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/${ENTT_PATH}/src/entt/entt.hpp")
        add_library(entt INTERFACE)
        target_include_directories(entt INTERFACE "${CMAKE_SOURCE_DIR}/${ENTT_PATH}/src")
        set(ENTT_FOUND TRUE)
        message(STATUS "Found EnTT at: ${ENTT_PATH}/src")
        break()
    endif()
endforeach()

if(NOT ENTT_FOUND)
    message(STATUS "EnTT not found - building without ECS support")
endif()

# Find and configure ImGui
set(IMGUI_FOUND FALSE)
set(IMGUI_SEARCH_PATHS
    "ThirdParty/imgui"
    "ThirdParty/UI/imgui"
    "third-party/imgui"
    "external/imgui"
    "ThirdParty/dear-imgui"
    "third-party/dear-imgui"
)

foreach(IMGUI_PATH ${IMGUI_SEARCH_PATHS})
    if(EXISTS "${CMAKE_SOURCE_DIR}/${IMGUI_PATH}/imgui.h")
        # Collect ImGui source files
        file(GLOB IMGUI_SOURCES
            "${CMAKE_SOURCE_DIR}/${IMGUI_PATH}/*.cpp"
            "${CMAKE_SOURCE_DIR}/${IMGUI_PATH}/*.h"
        )
        
        # Add backend files if they exist
        file(GLOB IMGUI_BACKEND_SOURCES
            "${CMAKE_SOURCE_DIR}/${IMGUI_PATH}/backends/imgui_impl_win32.*"
            "${CMAKE_SOURCE_DIR}/${IMGUI_PATH}/backends/imgui_impl_dx11.*"
            "${CMAKE_SOURCE_DIR}/${IMGUI_PATH}/backends/imgui_impl_dx12.*"
            "${CMAKE_SOURCE_DIR}/${IMGUI_PATH}/backends/imgui_impl_opengl3.*"
        )
        
        if(IMGUI_SOURCES)
            add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_BACKEND_SOURCES})
            target_include_directories(imgui PUBLIC 
                "${CMAKE_SOURCE_DIR}/${IMGUI_PATH}"
                "${CMAKE_SOURCE_DIR}/${IMGUI_PATH}/backends"
            )
            target_link_libraries(imgui PUBLIC d3d11 dxgi)
            set(IMGUI_FOUND TRUE)
            message(STATUS "Found ImGui at: ${IMGUI_PATH}")
            break()
        endif()
    endif()
endforeach()

if(NOT IMGUI_FOUND)
    message(STATUS "ImGui not found - building without GUI support")
endif()

# Find and configure GLFW
set(GLFW_FOUND FALSE)
set(GLFW_SEARCH_PATHS
    "ThirdParty/glfw"
    "third-party/glfw"
    "external/glfw"
    "ThirdParty/GLFW"
)

foreach(GLFW_PATH ${GLFW_SEARCH_PATHS})
    if(EXISTS "${CMAKE_SOURCE_DIR}/${GLFW_PATH}/include/GLFW/glfw3.h")
        set(GLFW_FOUND TRUE)
        add_subdirectory("${CMAKE_SOURCE_DIR}/${GLFW_PATH}" glfw EXCLUDE_FROM_ALL)
        message(STATUS "Found GLFW at: ${GLFW_PATH}")
        break()
    endif()
endforeach()

if(NOT GLFW_FOUND)
    message(STATUS "GLFW not found - building without GLFW support")
endif()

# Find and configure PhysX
set(PHYSX_FOUND FALSE)
set(PHYSX_SEARCH_PATHS
    "ThirdParty/PhysX"
    "third-party/PhysX"
    "external/PhysX"
    "ThirdParty/physx"
    "third-party/physx"
)

foreach(PHYSX_PATH ${PHYSX_SEARCH_PATHS})
    if(EXISTS "${CMAKE_SOURCE_DIR}/${PHYSX_PATH}/Include/PxPhysicsAPI.h")
        set(PHYSX_FOUND TRUE)
        message(STATUS "Found PhysX at: ${PHYSX_PATH}")
        break()
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/${PHYSX_PATH}/physx/include/PxPhysicsAPI.h")
        set(PHYSX_FOUND TRUE)
        set(PHYSX_PATH "${PHYSX_PATH}/physx")
        message(STATUS "Found PhysX at: ${PHYSX_PATH}")
        break()
    endif()
endforeach()

if(NOT PHYSX_FOUND)
    message(STATUS "PhysX not found - building without physics support")
endif()

# Find and configure Assimp
set(ASSIMP_FOUND FALSE)
set(ASSIMP_SEARCH_PATHS
    "ThirdParty/assimp"
    "third-party/assimp"
    "external/assimp"
    "ThirdParty/Assimp"
)

foreach(ASSIMP_PATH ${ASSIMP_SEARCH_PATHS})
    if(EXISTS "${CMAKE_SOURCE_DIR}/${ASSIMP_PATH}/include/assimp/Importer.hpp")
        set(ASSIMP_FOUND TRUE)
        add_subdirectory("${CMAKE_SOURCE_DIR}/${ASSIMP_PATH}" assimp EXCLUDE_FROM_ALL)
        message(STATUS "Found Assimp at: ${ASSIMP_PATH}")
        break()
    endif()
endforeach()

if(NOT ASSIMP_FOUND)
    message(STATUS "Assimp not found - building without model loading support")
endif()

# Find and configure stb (header-only library)
set(STB_FOUND FALSE)
set(STB_SEARCH_PATHS
    "ThirdParty/stb"
    "third-party/stb"
    "external/stb"
    "ThirdParty/STB"
)

foreach(STB_PATH ${STB_SEARCH_PATHS})
    if(EXISTS "${CMAKE_SOURCE_DIR}/${STB_PATH}/stb_image.h")
        add_library(stb INTERFACE)
        target_include_directories(stb INTERFACE "${CMAKE_SOURCE_DIR}/${STB_PATH}")
        set(STB_FOUND TRUE)
        message(STATUS "Found STB at: ${STB_PATH}")
        break()
    endif()
endforeach()

if(NOT STB_FOUND)
    message(STATUS "STB not found - building without STB image support")
endif()

# Find and configure OpenAL
set(OPENAL_FOUND FALSE)
set(OPENAL_SEARCH_PATHS
    "ThirdParty/openal"
    "third-party/openal"
    "external/openal"
    "ThirdParty/OpenAL"
    "ThirdParty/openal-soft"
)

foreach(OPENAL_PATH ${OPENAL_SEARCH_PATHS})
    if(EXISTS "${CMAKE_SOURCE_DIR}/${OPENAL_PATH}/include/AL/al.h")
        set(OPENAL_FOUND TRUE)
        message(STATUS "Found OpenAL at: ${OPENAL_PATH}")
        break()
    endif()
endforeach()

if(NOT OPENAL_FOUND)
    message(STATUS "OpenAL not found - building without OpenAL audio support")
endif()

# Find and configure Bullet Physics
set(BULLET_FOUND FALSE)
set(BULLET_SEARCH_PATHS
    "ThirdParty/bullet3"
    "third-party/bullet3"
    "external/bullet3"
    "ThirdParty/Bullet"
    "third-party/bullet"
)

foreach(BULLET_PATH ${BULLET_SEARCH_PATHS})
    if(EXISTS "${CMAKE_SOURCE_DIR}/${BULLET_PATH}/src/btBulletDynamicsCommon.h")
        set(BULLET_FOUND TRUE)
        add_subdirectory("${CMAKE_SOURCE_DIR}/${BULLET_PATH}" bullet EXCLUDE_FROM_ALL)
        message(STATUS "Found Bullet Physics at: ${BULLET_PATH}")
        break()
    endif()
endforeach()

if(NOT BULLET_FOUND)
    message(STATUS "Bullet Physics not found - building without Bullet support")
endif()

# Find and configure Sol2 (Lua binding)
set(SOL2_FOUND FALSE)
set(SOL2_SEARCH_PATHS
    "ThirdParty/sol2"
    "third-party/sol2"
    "external/sol2"
    "ThirdParty/Sol2"
)

foreach(SOL2_PATH ${SOL2_SEARCH_PATHS})
    if(EXISTS "${CMAKE_SOURCE_DIR}/${SOL2_PATH}/include/sol/sol.hpp")
        add_library(sol2 INTERFACE)
        target_include_directories(sol2 INTERFACE "${CMAKE_SOURCE_DIR}/${SOL2_PATH}/include")
        set(SOL2_FOUND TRUE)
        message(STATUS "Found Sol2 at: ${SOL2_PATH}")
        break()
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/${SOL2_PATH}/single/include/sol/sol.hpp")
        add_library(sol2 INTERFACE)
        target_include_directories(sol2 INTERFACE "${CMAKE_SOURCE_DIR}/${SOL2_PATH}/single/include")
        set(SOL2_FOUND TRUE)
        message(STATUS "Found Sol2 at: ${SOL2_PATH}/single/include")
        break()
    endif()
endforeach()

if(NOT SOL2_FOUND)
    message(STATUS "Sol2 not found - building without Lua scripting support")
endif()

# Find and configure Lua
set(LUA_FOUND FALSE)
set(LUA_SEARCH_PATHS
    "ThirdParty/lua"
    "third-party/lua"
    "external/lua"
    "ThirdParty/Lua"
)

foreach(LUA_PATH ${LUA_SEARCH_PATHS})
    if(EXISTS "${CMAKE_SOURCE_DIR}/${LUA_PATH}/lua.h")
        set(LUA_FOUND TRUE)
        message(STATUS "Found Lua at: ${LUA_PATH}")
        break()
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/${LUA_PATH}/src/lua.h")
        set(LUA_FOUND TRUE)
        set(LUA_PATH "${LUA_PATH}/src")
        message(STATUS "Found Lua at: ${LUA_PATH}")
        break()
    endif()
endforeach()

if(NOT LUA_FOUND)
    message(STATUS "Lua not found - building without Lua support")
endif()

# Collect all SparkEngine source files
file(GLOB_RECURSE SPARK_ENGINE_SOURCES
    "Spark Engine/Source/*.cpp"
    "Spark Engine/Source/*.h"
    "Spark Engine/Source/*.hpp"
)

# Collect all ThirdParty source files to add to project
file(GLOB_RECURSE THIRDPARTY_SOURCES
    "ThirdParty/*.h"
    "ThirdParty/*.hpp"
    "third-party/*.h"
    "third-party/*.hpp"
)

# Filter out test files and problematic files
list(FILTER SPARK_ENGINE_SOURCES EXCLUDE REGEX ".*[Tt]est.*")
list(FILTER SPARK_ENGINE_SOURCES EXCLUDE REGEX ".*[Bb]ench.*")
list(FILTER SPARK_ENGINE_SOURCES EXCLUDE REGEX ".*[Ee]xample.*")

# Remove any empty or problematic files
foreach(SOURCE_FILE ${SPARK_ENGINE_SOURCES})
    if(NOT EXISTS "${SOURCE_FILE}")
        list(REMOVE_ITEM SPARK_ENGINE_SOURCES "${SOURCE_FILE}")
    endif()
endforeach()

# Create the main executable
if(SPARK_ENGINE_SOURCES)
    add_executable(SparkEngine WIN32 ${SPARK_ENGINE_SOURCES})
    
    # Add third-party headers to project for visibility
    if(THIRDPARTY_SOURCES)
        target_sources(SparkEngine PRIVATE ${THIRDPARTY_SOURCES})
    endif()

    # Create Visual Studio folder structure that matches source directory structure
    foreach(FILE ${SPARK_ENGINE_SOURCES})
        # Get the directory of the file relative to the source root
        get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

        # Get relative path from "Spark Engine/Source" directory
        string(REPLACE "${CMAKE_SOURCE_DIR}/Spark Engine/Source/" "" GROUP_NAME "${PARENT_DIR}")
        string(REPLACE "${CMAKE_SOURCE_DIR}/Spark Engine/Source" "" GROUP_NAME "${GROUP_NAME}")

        # Replace forward slashes with backslashes for Windows
        string(REPLACE "/" "\\" GROUP_NAME "${GROUP_NAME}")

        # Remove leading slashes/backslashes
        string(REGEX REPLACE "^[\\/]+" "" GROUP_NAME "${GROUP_NAME}")

        # If group name is empty, put in root Source Files folder
        if("${GROUP_NAME}" STREQUAL "")
            source_group("Source Files" FILES "${FILE}")
        else()
            source_group("Source Files\\${GROUP_NAME}" FILES "${FILE}")
        endif()
    endforeach()
    
    # Create folder structure for third-party files
    foreach(FILE ${THIRDPARTY_SOURCES})
        get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
        string(REPLACE "${CMAKE_SOURCE_DIR}/" "" GROUP_NAME "${PARENT_DIR}")
        string(REPLACE "/" "\\" GROUP_NAME "${GROUP_NAME}")
        source_group("${GROUP_NAME}" FILES "${FILE}")
    endforeach()

    # Include directories
    target_include_directories(SparkEngine PRIVATE 
        "Spark Engine/Source"
        "Spark Engine/Source/Core"
    )

    # System libraries
    target_link_libraries(SparkEngine PRIVATE
        # DirectX libraries
        d3d11 dxgi d3dcompiler dxguid
        
        # Windows system libraries
        winmm ws2_32 wsock32
        ole32 oleaut32 uuid
        kernel32 user32 gdi32 winspool
        shell32 comdlg32 advapi32
    )

    # Conditional library linking with options
    if(ENTT_FOUND)
        target_link_libraries(SparkEngine PRIVATE entt)
        target_compile_definitions(SparkEngine PRIVATE ENTT_AVAILABLE)
        message(STATUS "Enabled EnTT integration")
    endif()

    if(IMGUI_FOUND AND ENABLE_EDITOR)
        target_link_libraries(SparkEngine PRIVATE imgui)
        target_compile_definitions(SparkEngine PRIVATE IMGUI_AVAILABLE EDITOR_ENABLED)
        message(STATUS "Enabled ImGui integration with Editor")
    elseif(IMGUI_FOUND)
        target_link_libraries(SparkEngine PRIVATE imgui)
        target_compile_definitions(SparkEngine PRIVATE IMGUI_AVAILABLE)
        message(STATUS "Enabled ImGui integration")
    endif()
    
    if(GLFW_FOUND)
        target_link_libraries(SparkEngine PRIVATE glfw)
        target_compile_definitions(SparkEngine PRIVATE GLFW_AVAILABLE)
        message(STATUS "Enabled GLFW integration")
    endif()

    if(PHYSX_FOUND AND ENABLE_PHYSX)
        target_compile_definitions(SparkEngine PRIVATE PHYSX_AVAILABLE)
        message(STATUS "Enabled PhysX integration")
    elseif(ENABLE_PHYSX)
        message(STATUS "PhysX requested but not found")
    endif()
    
    if(ASSIMP_FOUND)
        target_link_libraries(SparkEngine PRIVATE assimp)
        target_compile_definitions(SparkEngine PRIVATE ASSIMP_AVAILABLE)
        message(STATUS "Enabled Assimp integration")
    endif()
    
    if(STB_FOUND)
        target_link_libraries(SparkEngine PRIVATE stb)
        target_compile_definitions(SparkEngine PRIVATE STB_AVAILABLE)
        message(STATUS "Enabled STB integration")
    endif()
    
    if(OPENAL_FOUND)
        target_compile_definitions(SparkEngine PRIVATE OPENAL_AVAILABLE)
        message(STATUS "Enabled OpenAL integration")
    endif()
    
    if(BULLET_FOUND)
        target_link_libraries(SparkEngine PRIVATE BulletDynamics BulletCollision LinearMath)
        target_compile_definitions(SparkEngine PRIVATE BULLET_AVAILABLE)
        message(STATUS "Enabled Bullet Physics integration")
    endif()
    
    if(SOL2_FOUND AND ENABLE_LUA)
        target_link_libraries(SparkEngine PRIVATE sol2)
        target_compile_definitions(SparkEngine PRIVATE SOL2_AVAILABLE LUA_SCRIPTING_ENABLED)
        message(STATUS "Enabled Sol2 integration with Lua scripting")
    elseif(ENABLE_LUA)
        message(STATUS "Lua scripting requested but Sol2 not found")
    endif()
    
    if(LUA_FOUND AND ENABLE_LUA)
        target_compile_definitions(SparkEngine PRIVATE LUA_AVAILABLE LUA_SCRIPTING_ENABLED)
        message(STATUS "Enabled Lua integration")
    elseif(ENABLE_LUA)
        message(STATUS "Lua scripting requested but Lua not found")
    endif()

    # Feature-specific compile definitions
    if(ENABLE_COLLABORATIVE)
        target_compile_definitions(SparkEngine PRIVATE COLLABORATIVE_ENABLED)
        message(STATUS "Enabled collaborative features")
    endif()

    if(ENABLE_ADVANCED_GRAPHICS)
        target_compile_definitions(SparkEngine PRIVATE ADVANCED_GRAPHICS_ENABLED)
        message(STATUS "Enabled advanced graphics features")
    endif()

    if(ENABLE_PROFILING)
        target_compile_definitions(SparkEngine PRIVATE PROFILING_ENABLED)
        message(STATUS "Enabled profiling tools")
    endif()

    if(ENABLE_NETWORKING)
        target_compile_definitions(SparkEngine PRIVATE NETWORKING_ENABLED)
        message(STATUS "Enabled networking features")
    endif()

    if(ENABLE_ASSET_STREAMING)
        target_compile_definitions(SparkEngine PRIVATE ASSET_STREAMING_ENABLED)
        message(STATUS "Enabled asset streaming")
    endif()

    if(ENABLE_HOT_RELOAD)
        target_compile_definitions(SparkEngine PRIVATE HOT_RELOAD_ENABLED)
        message(STATUS "Enabled hot reload functionality")
    endif()

    if(ENABLE_TERRAIN_SYSTEM)
        target_compile_definitions(SparkEngine PRIVATE TERRAIN_SYSTEM_ENABLED)
        message(STATUS "Enabled terrain system")
    endif()

    if(ENABLE_POST_PROCESSING)
        target_compile_definitions(SparkEngine PRIVATE POST_PROCESSING_ENABLED)
        message(STATUS "Enabled post-processing effects")
    endif()

    if(ENABLE_LIGHTING_SYSTEM)
        target_compile_definitions(SparkEngine PRIVATE LIGHTING_SYSTEM_ENABLED)
        message(STATUS "Enabled advanced lighting system")
    endif()

    if(ENABLE_ADVANCED_INPUT)
        target_compile_definitions(SparkEngine PRIVATE ADVANCED_INPUT_ENABLED)
        message(STATUS "Enabled advanced input features")
    endif()

    # Configuration-specific definitions
    target_compile_definitions(SparkEngine PRIVATE
        $<$<CONFIG:Debug>:_DEBUG SPARK_DEBUG>
        $<$<CONFIG:Release>:NDEBUG SPARK_RELEASE>
        $<$<CONFIG:RelWithDebInfo>:SPARK_RELEASE_WITH_DEBUG>
    )

    # Set Visual Studio properties
    if(MSVC)
        set_property(TARGET SparkEngine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SparkEngine)
    endif()

    message(STATUS "SparkEngine executable configured successfully")
else()
    message(FATAL_ERROR "No source files found for SparkEngine")
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "========================================")
message(STATUS "SparkEngine Build Configuration Summary")
message(STATUS "========================================")
message(STATUS "Build Type:           ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard:         ${CMAKE_CXX_STANDARD}")
message(STATUS "")
message(STATUS "Third-Party Libraries:")
message(STATUS "  EnTT Support:       ${ENTT_FOUND}")
message(STATUS "  ImGui Support:      ${IMGUI_FOUND}")
message(STATUS "  GLFW Support:       ${GLFW_FOUND}")
message(STATUS "  PhysX Support:      ${PHYSX_FOUND}")
message(STATUS "  Assimp Support:     ${ASSIMP_FOUND}")
message(STATUS "  STB Support:        ${STB_FOUND}")
message(STATUS "  OpenAL Support:     ${OPENAL_FOUND}")
message(STATUS "  Bullet Support:     ${BULLET_FOUND}")
message(STATUS "  Sol2 Support:       ${SOL2_FOUND}")
message(STATUS "  Lua Support:        ${LUA_FOUND}")
message(STATUS "")
message(STATUS "Feature Options:")
message(STATUS "  PhysX:              ${ENABLE_PHYSX}")
message(STATUS "  Editor:             ${ENABLE_EDITOR}")
message(STATUS "  Lua Scripting:      ${ENABLE_LUA}")
message(STATUS "  Collaborative:      ${ENABLE_COLLABORATIVE}")
message(STATUS "  Advanced Graphics:  ${ENABLE_ADVANCED_GRAPHICS}")
message(STATUS "  Profiling:          ${ENABLE_PROFILING}")
message(STATUS "  Networking:         ${ENABLE_NETWORKING}")
message(STATUS "  Asset Streaming:    ${ENABLE_ASSET_STREAMING}")
message(STATUS "  Hot Reload:         ${ENABLE_HOT_RELOAD}")
message(STATUS "  Terrain System:     ${ENABLE_TERRAIN_SYSTEM}")
message(STATUS "  Post Processing:    ${ENABLE_POST_PROCESSING}")
message(STATUS "  Lighting System:    ${ENABLE_LIGHTING_SYSTEM}")
message(STATUS "  Advanced Input:     ${ENABLE_ADVANCED_INPUT}")
message(STATUS "")
message(STATUS "Compiler:             ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Toolset:              ${CMAKE_GENERATOR_TOOLSET}")  
message(STATUS "Platform:             ${CMAKE_GENERATOR_PLATFORM}")
message(STATUS "Source Files Found:   ${SPARK_ENGINE_SOURCES}")
message(STATUS "ThirdParty Files:     ${THIRDPARTY_SOURCES}")
message(STATUS "========================================")
message(STATUS "")