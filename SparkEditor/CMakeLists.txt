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

# Proper ImGui dependency management
find_path(IMGUI_INCLUDE_DIR 
    NAMES imgui.h
    PATHS 
        "${CMAKE_SOURCE_DIR}/ThirdParty/imgui"
        "${CMAKE_SOURCE_DIR}/ThirdParty/UI/imgui"
        "${CMAKE_SOURCE_DIR}/External/imgui"
        "${CMAKE_SOURCE_DIR}/Vendor/imgui"
    DOC "Dear ImGui include directory"
)

if(IMGUI_INCLUDE_DIR)
    message(STATUS "Found Dear ImGui at: ${IMGUI_INCLUDE_DIR}")
    
    # Create proper ImGui library target
    add_library(imgui STATIC
        "${IMGUI_INCLUDE_DIR}/imgui.cpp"
        "${IMGUI_INCLUDE_DIR}/imgui_demo.cpp"
        "${IMGUI_INCLUDE_DIR}/imgui_draw.cpp"
        "${IMGUI_INCLUDE_DIR}/imgui_tables.cpp"
        "${IMGUI_INCLUDE_DIR}/imgui_widgets.cpp"
        "${IMGUI_INCLUDE_DIR}/backends/imgui_impl_win32.cpp"
        "${IMGUI_INCLUDE_DIR}/backends/imgui_impl_dx11.cpp"
    )
    
    target_include_directories(imgui PUBLIC 
        "${IMGUI_INCLUDE_DIR}"
        "${IMGUI_INCLUDE_DIR}/backends"
    )
    
    target_compile_definitions(imgui PUBLIC
        IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
        IMGUI_IMPL_WIN32_DISABLE_LINKING_XINPUT
    )
    
    # Link required system libraries
    target_link_libraries(imgui PUBLIC
        d3d11 dxgi user32 gdi32 shell32 ole32 oleaut32 uuid comdlg32 advapi32
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
    message(FATAL_ERROR "Dear ImGui is required for SparkEditor but was not found!")
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
        "${IMGUI_INCLUDE_DIR}"
        "${IMGUI_INCLUDE_DIR}/backends"
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
    
    # DirectX and graphics
    GRAPHICS_API_DX11
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    _CRT_SECURE_NO_WARNINGS
    
    # Engine integration
    ENGINE_INTEGRATION_ENABLED
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
        /W3  # Warning level 3
        /MP  # Multi-processor compilation
        /bigobj  # Large object files
        /std:c++20  # C++20 standard
        
        # Disable specific warnings
        /wd4005  # Macro redefinition
        /wd4996  # Deprecated functions
        /wd4244  # Conversion warnings
        /wd4267  # size_t conversion warnings
        /wd4251  # DLL interface warnings
        /wd4275  # DLL interface warnings for base classes
        
        # Performance optimizations for Release
        $<$<CONFIG:Release>:/O2 /Oi /Ot /Oy /GL>
        
        # Debug information for Debug
        $<$<CONFIG:Debug>:/Od /RTC1>
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
    
    # Network libraries
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
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/Projects"
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEditor>/Temp"
    COMMENT "Creating SparkEditor directory structure"
)

# Copy editor configuration files if they exist
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
    
    # Copy SparkEngine executable for integration
    add_custom_command(TARGET SparkEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SparkEngine>
            $<TARGET_FILE_DIR:SparkEditor>/$<TARGET_FILE_NAME:SparkEngine>
        COMMENT "Installing SparkEngine for editor integration"
    )
endif()

if(TARGET SparkConsole)
    add_custom_command(TARGET SparkEditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SparkConsole>
            $<TARGET_FILE_DIR:SparkEditor>/$<TARGET_FILE_NAME:SparkConsole>
        COMMENT "Installing SparkConsole for editor debugging"
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