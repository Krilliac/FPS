cmake_minimum_required(VERSION 3.16)

# ---------------------------------------------------------------------
# SparkEditor Project Configuration
# ---------------------------------------------------------------------
project(SparkEditor VERSION 1.0.0 LANGUAGES CXX)

# Set C++20 standard (inherits from parent but ensure it's set)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------
# Third-party Dependencies
# ---------------------------------------------------------------------

# Find Dear ImGui
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/imgui")
if(NOT EXISTS "${IMGUI_DIR}")
    # Try alternative locations
    foreach(ALT_PATH IN ITEMS
        "${CMAKE_SOURCE_DIR}/ThirdParty/UI/imgui"
        "${CMAKE_SOURCE_DIR}/External/imgui"
        "${CMAKE_SOURCE_DIR}/Vendor/imgui")
        if(EXISTS "${ALT_PATH}")
            set(IMGUI_DIR "${ALT_PATH}")
            break()
        endif()
    endforeach()
endif()

if(EXISTS "${IMGUI_DIR}")
    message(STATUS "Found Dear ImGui at: ${IMGUI_DIR}")
    
    # Create ImGui library
    add_library(imgui STATIC
        "${IMGUI_DIR}/imgui.cpp"
        "${IMGUI_DIR}/imgui_demo.cpp"
        "${IMGUI_DIR}/imgui_draw.cpp"
        "${IMGUI_DIR}/imgui_tables.cpp"
        "${IMGUI_DIR}/imgui_widgets.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_win32.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_dx11.cpp"
        "${IMGUI_DIR}/imgui.h"
        "${IMGUI_DIR}/imconfig.h"
        "${IMGUI_DIR}/imgui_internal.h"
        "${IMGUI_DIR}/imstb_rectpack.h"
        "${IMGUI_DIR}/imstb_textedit.h"
        "${IMGUI_DIR}/imstb_truetype.h"
        "${IMGUI_DIR}/backends/imgui_impl_win32.h"
        "${IMGUI_DIR}/backends/imgui_impl_dx11.h"
    )
    
    target_include_directories(imgui PUBLIC 
        "${IMGUI_DIR}"
        "${IMGUI_DIR}/backends"
    )
    
    target_compile_definitions(imgui PUBLIC
        IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
        IMGUI_IMPL_WIN32_DISABLE_LINKING_XINPUT
    )
    
    # Link system libraries needed by ImGui
    target_link_libraries(imgui PUBLIC
        d3d11
        dxgi
        user32
        gdi32
        shell32
        ole32
        oleaut32
        uuid
        comdlg32
        advapi32
    )
    
    if(MSVC)
        target_compile_options(imgui PRIVATE /W2)
        target_compile_definitions(imgui PRIVATE 
            WIN32_LEAN_AND_MEAN 
            NOMINMAX
            _CRT_SECURE_NO_WARNINGS
        )
    endif()
    
    set(IMGUI_FOUND TRUE)
else()
    message(WARNING "Dear ImGui not found. Editor will not build properly.")
    set(IMGUI_FOUND FALSE)
endif()

# ---------------------------------------------------------------------
# SparkEditor Source Files
# ---------------------------------------------------------------------

# Collect all source files
file(GLOB_RECURSE SPARK_EDITOR_SOURCES
    CONFIGURE_DEPENDS
    "Source/*.cpp"
    "Source/*.h"
    "Source/*.hpp"
)

# Remove any test files
list(FILTER SPARK_EDITOR_SOURCES EXCLUDE REGEX ".*[Tt]est.*")
list(FILTER SPARK_EDITOR_SOURCES EXCLUDE REGEX ".*[Ee]xample.*")

# Create the SparkEditor executable
add_executable(SparkEditor WIN32 ${SPARK_EDITOR_SOURCES})

# ---------------------------------------------------------------------
# Include Directories
# ---------------------------------------------------------------------
target_include_directories(SparkEditor PRIVATE
    "Source"
    "${CMAKE_SOURCE_DIR}/Shared"
    "${CMAKE_SOURCE_DIR}/Spark Engine/Source"
)

# Add ImGui include directories if found
if(IMGUI_FOUND)
    target_include_directories(SparkEditor PRIVATE
        "${IMGUI_DIR}"
        "${IMGUI_DIR}/backends"
    )
endif()

# ---------------------------------------------------------------------
# Preprocessor Definitions
# ---------------------------------------------------------------------
target_compile_definitions(SparkEditor PRIVATE
    # Editor features
    SPARK_EDITOR_BUILD
    EDITOR_VERSION_MAJOR=1
    EDITOR_VERSION_MINOR=0
    EDITOR_VERSION_PATCH=0
    
    # Phase completions
    PHASE_1_COMPLETE      # Foundation systems
    PHASE_2_COMPLETE      # Advanced editing systems  
    PHASE_3_COMPLETE      # Professional tools
    
    # Feature flags for all Phase 3 systems
    VISUAL_SCRIPTING_ENABLED
    ADVANCED_ASSET_PIPELINE_ENABLED
    LEVEL_STREAMING_ENABLED
    PERFORMANCE_PROFILER_ENABLED
    VERSION_CONTROL_ENABLED
    BUILD_DEPLOYMENT_ENABLED
    
    # DirectX and graphics
    GRAPHICS_API_DX11
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    _CRT_SECURE_NO_WARNINGS
    
    # Engine integration
    ENGINE_INTEGRATION_ENABLED
    REAL_TIME_PREVIEW_ENABLED
    HOT_RELOAD_ENABLED
    
    # Professional features
    COLLABORATIVE_EDITING_ENABLED
    AI_OPTIMIZATION_ENABLED
    CLOUD_SERVICES_ENABLED
    MULTI_PLATFORM_BUILD_ENABLED
)

# Debug-specific definitions
target_compile_definitions(SparkEditor PRIVATE
    $<$<CONFIG:Debug>:
        SPARK_DEBUG
        ENABLE_ASSERTIONS
        ENABLE_LOGGING
        ENABLE_PROFILING
        IMGUI_DEBUG
    >
)

# Release-specific definitions  
target_compile_definitions(SparkEditor PRIVATE
    $<$<CONFIG:Release>:
        SPARK_RELEASE
        NDEBUG
    >
)

# ---------------------------------------------------------------------
# Compiler Options
# ---------------------------------------------------------------------
if(MSVC)
    target_compile_options(SparkEditor PRIVATE
        /W3                 # Warning level 3
        /MP                 # Multi-processor compilation
        /bigobj             # Large object files
        /std:c++20          # C++20 standard
        
        # Disable specific warnings
        /wd4005             # Macro redefinition
        /wd4996             # Deprecated functions
        /wd4244             # Conversion warnings
        /wd4267             # size_t conversion warnings
        /wd4251             # DLL interface warnings
        /wd4275             # DLL interface warnings for base classes
        
        # Performance optimizations for Release
        $<$<CONFIG:Release>:/O2 /Oi /Ot /Oy /GL>
        
        # Debug information for Debug
        $<$<CONFIG:Debug>:/Od /RTC1 /MDd>
        $<$<CONFIG:Release>:/MD>
    )
    
    # Link-time optimizations for Release
    target_link_options(SparkEditor PRIVATE
        $<$<CONFIG:Release>:/LTCG /OPT:REF /OPT:ICF>
    )
endif()

# ---------------------------------------------------------------------
# Link Libraries
# ---------------------------------------------------------------------

# Link ImGui library
if(IMGUI_FOUND)
    target_link_libraries(SparkEditor PRIVATE imgui)
endif()

# System libraries for Windows
target_link_libraries(SparkEditor PRIVATE
    # DirectX 11
    d3d11
    dxgi  
    d3dcompiler
    dxguid
    
    # Windows system libraries
    kernel32
    user32
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32
    
    # Network libraries (for version control and collaboration)
    ws2_32
    wsock32
    winmm
    crypt32
    wldap32
    normaliz
    
    # File system and COM
    shlwapi
    comctl32
    version
)

# ---------------------------------------------------------------------
# Post-Build Steps
# ---------------------------------------------------------------------

# Create necessary directories
add_custom_command(TARGET SparkEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/EditorAssets"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/EditorAssets/Icons"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/EditorAssets/Themes"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/EditorAssets/Layouts"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/EditorAssets/Templates"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/Projects"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/Projects/DefaultProject"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/Projects/DefaultProject/Scenes"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/Projects/DefaultProject/Assets"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/Projects/DefaultProject/Scripts"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/Temp"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/Library"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/Cache"
    COMMENT "Creating SparkEditor directory structure"
)

# Copy editor-specific assets if they exist
foreach(ASSET_TYPE IN ITEMS "Icons" "Themes" "Layouts" "Templates")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Resources/${ASSET_TYPE}")
        add_custom_command(TARGET SparkEditor POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/Resources/${ASSET_TYPE}"
            "$<TARGET_FILE_DIR:SparkEditor>/EditorAssets/${ASSET_TYPE}"
            COMMENT "Copying ${ASSET_TYPE} to editor assets"
        )
    endif()
endforeach()

# Copy editor configuration files
foreach(CONFIG_FILE IN ITEMS "EditorConfig.json" "KeyBindings.json" "DefaultLayout.json")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_FILE}")
        add_custom_command(TARGET SparkEditor POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_FILE}"
            "$<TARGET_FILE_DIR:SparkEditor>/${CONFIG_FILE}"
            COMMENT "Copying ${CONFIG_FILE}"
        )
    endif()
endforeach()

# ---------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------

# Ensure SparkEditor is built after SparkEngine if it exists
if(TARGET SparkEngine)
    add_dependencies(SparkEditor SparkEngine)
    message(STATUS "SparkEditor will be built after SparkEngine")
    
    # Copy SparkEngine.exe to editor directory for engine integration
    add_custom_command(TARGET SparkEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SparkEngine>"
            "$<TARGET_FILE_DIR:SparkEditor>/SparkEngine.exe"
        COMMENT "Copying SparkEngine.exe to editor directory for integration"
    )
endif()

# Copy SparkConsole if it exists (for debugging and communication)
if(TARGET SparkConsole)
    add_custom_command(TARGET SparkEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SparkConsole>"
            "$<TARGET_FILE_DIR:SparkEditor>/SparkConsole.exe"
        COMMENT "Copying SparkConsole.exe to editor directory"
    )
endif()

# ---------------------------------------------------------------------
# Visual Studio Integration
# ---------------------------------------------------------------------
if(MSVC)
    # Set working directory for debugging
    set_property(TARGET SparkEditor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    
    # Set as startup project if this is the main project
    if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SparkEditor)
    endif()
    
    # Organize source files in Solution Explorer
    foreach(source ${SPARK_EDITOR_SOURCES})
        get_filename_component(source_path "${source}" DIRECTORY)
        file(RELATIVE_PATH source_relative "${CMAKE_CURRENT_SOURCE_DIR}/Source" "${source_path}")
        string(REPLACE "/" "\\\\" group_path "${source_relative}")
        
        if(group_path STREQUAL "")
            source_group("Source Files" FILES "${source}")
        else()
            source_group("Source Files\\\\${group_path}" FILES "${source}")
        endif()
    endforeach()
    
    # Create virtual folders for better organization
    source_group("Core Systems" REGULAR_EXPRESSION "Source/Core/.*")
    source_group("UI Panels" REGULAR_EXPRESSION "Source/Panels/.*")
    source_group("Scene Management" REGULAR_EXPRESSION "Source/SceneSystem/.*")
    source_group("Asset Management" REGULAR_EXPRESSION "Source/AssetBrowser/.*")
    source_group("Communication" REGULAR_EXPRESSION "Source/Communication/.*")
    source_group("Viewport" REGULAR_EXPRESSION "Source/Viewport/.*")
    
    # Phase 2 systems
    source_group("Advanced Systems\\\\Gizmos" REGULAR_EXPRESSION "Source/Gizmos/.*")
    source_group("Advanced Systems\\\\Reflection" REGULAR_EXPRESSION "Source/Reflection/.*")
    source_group("Advanced Systems\\\\Material Editor" REGULAR_EXPRESSION "Source/MaterialEditor/.*")
    source_group("Advanced Systems\\\\Animation" REGULAR_EXPRESSION "Source/Animation/.*")
    source_group("Advanced Systems\\\\Terrain" REGULAR_EXPRESSION "Source/Terrain/.*")
    source_group("Advanced Systems\\\\Lighting" REGULAR_EXPRESSION "Source/Lighting/.*")
    
    # Phase 3 systems
    source_group("Professional Tools\\\\Visual Scripting" REGULAR_EXPRESSION "Source/VisualScripting/.*")
    source_group("Professional Tools\\\\Asset Pipeline" REGULAR_EXPRESSION "Source/AssetPipeline/.*")
    source_group("Professional Tools\\\\Level Streaming" REGULAR_EXPRESSION "Source/LevelStreaming/.*")
    source_group("Professional Tools\\\\Profiler" REGULAR_EXPRESSION "Source/Profiler/.*")
    source_group("Professional Tools\\\\Version Control" REGULAR_EXPRESSION "Source/VersionControl/.*")
    source_group("Professional Tools\\\\Build System" REGULAR_EXPRESSION "Source/BuildSystem/.*")
    
    # Add custom build configurations
    set_property(TARGET SparkEditor PROPERTY VS_GLOBAL_KEYWORD "Win32Proj")
    set_property(TARGET SparkEditor PROPERTY VS_GLOBAL_ROOTNAMESPACE "SparkEditor")
    set_property(TARGET SparkEditor PROPERTY VS_GLOBAL_WindowsTargetPlatformVersion "10.0")
endif()

# ---------------------------------------------------------------------
# Installation (Optional)
# ---------------------------------------------------------------------
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation directory" FORCE)
endif()

install(TARGETS SparkEditor
    RUNTIME DESTINATION bin
    COMPONENT SparkEditor
)

# Install editor assets
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/EditorAssets"
    DESTINATION bin
    COMPONENT SparkEditor
    OPTIONAL
)

# Install required DLLs and dependencies
if(WIN32)
    # Install Visual C++ Redistributable dependencies (if needed)
    include(InstallRequiredSystemLibraries)
endif()

# ---------------------------------------------------------------------
# Package Configuration
# ---------------------------------------------------------------------
set(CPACK_PACKAGE_NAME "SparkEditor")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Spark Engine Editor - Professional Game Development Environment")
set(CPACK_PACKAGE_VENDOR "Spark Engine Team")
set(CPACK_PACKAGE_CONTACT "support@sparkengine.dev")

if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "Spark Engine Editor")
    set(CPACK_NSIS_PACKAGE_NAME "SparkEditor")
    set(CPACK_NSIS_CONTACT "support@sparkengine.dev")
    set(CPACK_NSIS_HELP_LINK "https://sparkengine.dev/docs")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://sparkengine.dev")
    set(CPACK_NSIS_MODIFY_PATH ON)
endif()

include(CPack)

# ---------------------------------------------------------------------
# Status Messages
# ---------------------------------------------------------------------
message(STATUS "")
message(STATUS "=== SparkEditor Configuration Summary ===")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "Dependencies:")
message(STATUS "  Dear ImGui: ${IMGUI_FOUND}")
message(STATUS "")
message(STATUS "Features Enabled:")
message(STATUS "  ? Phase 1 - Foundation Systems (Complete)")
message(STATUS "  ? Phase 2 - Advanced Editing (Complete)")  
message(STATUS "  ? Phase 3 - Professional Tools (Complete)")
message(STATUS "")
message(STATUS "Professional Tools:")
message(STATUS "  ? Visual Scripting System")
message(STATUS "  ? Advanced Asset Pipeline") 
message(STATUS "  ? Level Streaming & World Composition")
message(STATUS "  ? Performance Profiler & Optimization")
message(STATUS "  ? Version Control Integration")
message(STATUS "  ? Build & Deployment System")
message(STATUS "")
message(STATUS "Output Directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "SparkEditor will be built to: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "==========================================")
message(STATUS "")

# Final validation
if(NOT IMGUI_FOUND)
    message(WARNING "Dear ImGui not found - Editor UI will not function properly!")
    message(WARNING "Please ensure Dear ImGui is available in ThirdParty/imgui or update the path")
endif()

message(STATUS "SparkEditor CMake configuration completed successfully!")