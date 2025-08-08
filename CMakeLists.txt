cmake_minimum_required(VERSION 3.16)

# -------------------------------------------------------------
# 1) Disable all third-party tests/examples globally
# -------------------------------------------------------------
set(BUILD_TESTING OFF   CACHE BOOL "Disable all testing"   FORCE)
set(BUILD_EXAMPLES OFF  CACHE BOOL "Disable all examples"  FORCE)
set(BUILD_SAMPLES OFF   CACHE BOOL "Disable all samples"   FORCE)
set(BUILD_TOOLS OFF     CACHE BOOL "Disable all tools"     FORCE)
set(BUILD_DOCS OFF      CACHE BOOL "Disable docs"          FORCE)

# -------------------------------------------------------------
# 2) Global generator and project settings
# -------------------------------------------------------------
if(WIN32)
    set(CMAKE_GENERATOR_TOOLSET "v143")
    set(CMAKE_GENERATOR_PLATFORM "x64")
endif()

project(SparkEngine)

# -------------------------------------------------------------
# 3) Custom build options
# -------------------------------------------------------------
option(ENABLE_PHYSX           "Enable PhysX physics engine"   ON)
option(ENABLE_EDITOR          "Enable editor features"        ON)
option(ENABLE_LUA             "Enable Lua scripting support"  ON)
option(ENABLE_COLLABORATIVE   "Enable collaborative features" ON)
option(ENABLE_GRAPHICS        "Enable graphics engine"        ON)
option(ENABLE_PROFILING       "Enable profiling tools"        ON)
option(ENABLE_NETWORKING      "Enable networking features"    ON)
option(ENABLE_ASSET_STREAMING "Enable asset streaming"        ON)
option(ENABLE_HOT_RELOAD      "Enable hot reload functionality" ON)
option(ENABLE_TERRAIN_SYSTEM  "Enable terrain system"          ON)
option(ENABLE_POST_PROCESSING "Enable post-processing effects" ON)
option(ENABLE_LIGHTING_SYSTEM "Enable advanced lighting"       ON)
option(ENABLE_ADVANCED_INPUT  "Enable advanced input features" ON)

# -------------------------------------------------------------
# 4) C++ standard and in-source build guard
# -------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed. Create a separate build directory.")
endif()

# -------------------------------------------------------------
# 5) Output directories
# -------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# -------------------------------------------------------------
# 6) MSVC-specific compiler options
# -------------------------------------------------------------
if(MSVC)
    add_compile_options(
        /W3 /MP /bigobj
        /wd4005  # Macro redefinition
        /wd4996  # Deprecated functions
        /wd4244  # Conversion warnings
        /wd4267  # size_t conversion warnings
    )
    add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS)
endif()

# -------------------------------------------------------------
# 7) CURL BUILD - WORKING VERSION
# -------------------------------------------------------------
set(CURL_ROOT "${CMAKE_SOURCE_DIR}/ThirdParty/Networking/curl")

# Minimal curl settings
set(BUILD_CURL_EXE          OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS       OFF CACHE BOOL "" FORCE)
set(CURL_STATICLIB          ON  CACHE BOOL "" FORCE)
set(HTTP_ONLY               ON  CACHE BOOL "" FORCE)
set(CURL_DISABLE_LDAP       ON  CACHE BOOL "" FORCE)
set(CURL_DISABLE_DICT       ON  CACHE BOOL "" FORCE)
set(CURL_DISABLE_TELNET     ON  CACHE BOOL "" FORCE)
set(CURL_DISABLE_RTSP       ON  CACHE BOOL "" FORCE)
set(CURL_DISABLE_POP3       ON  CACHE BOOL "" FORCE)
set(CURL_DISABLE_IMAP       ON  CACHE BOOL "" FORCE)
set(CURL_DISABLE_SMTP       ON  CACHE BOOL "" FORCE)
set(CURL_DISABLE_GOPHER     ON  CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL         OFF CACHE BOOL "" FORCE)
set(CURL_ZSTD               OFF CACHE BOOL "" FORCE)
set(CURL_BROTLI             OFF CACHE BOOL "" FORCE)
set(CURL_USE_SCHANNEL       ON  CACHE BOOL "" FORCE)

# Build curl
add_subdirectory("${CURL_ROOT}" EXCLUDE_FROM_ALL)

# -------------------------------------------------------------
# 8) SparkEngine source files
# -------------------------------------------------------------
file(GLOB_RECURSE SPARK_ENGINE_SOURCES
    CONFIGURE_DEPENDS
    "Spark Engine/Source/*.cpp"
    "Spark Engine/Source/*.h"
    "Spark Engine/Source/*.hpp"
)
list(FILTER SPARK_ENGINE_SOURCES EXCLUDE REGEX ".*[Tt]est.*")
list(FILTER SPARK_ENGINE_SOURCES EXCLUDE REGEX ".*[Ee]xample.*")

add_executable(SparkEngine WIN32 ${SPARK_ENGINE_SOURCES})

# -------------------------------------------------------------
# 8.5) COMPLETE ASSET MANAGEMENT - COPY ALL ASSETS TO BUILD OUTPUT
# -------------------------------------------------------------

# Function to copy assets with proper structure preservation
function(copy_assets_to_build)
    set(ASSETS_COPIED FALSE)
    
    # Copy Shaders (highest priority)
    foreach(SHADER_PATH IN ITEMS
        "${CMAKE_SOURCE_DIR}/Spark Engine/Shaders"
        "${CMAKE_SOURCE_DIR}/Shaders" 
        "${CMAKE_SOURCE_DIR}/Assets/Shaders")
        
        if(EXISTS "${SHADER_PATH}")
            add_custom_command(TARGET SparkEngine POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${SHADER_PATH}"
                "$<TARGET_FILE_DIR:SparkEngine>/Shaders"
                COMMENT "Copying shaders from ${SHADER_PATH}")
            
            # Also copy to working directory for immediate access
            add_custom_command(TARGET SparkEngine POST_BUILD  
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${SHADER_PATH}"
                "${CMAKE_SOURCE_DIR}/Shaders"
                COMMENT "Copying shaders to working directory")
                
            message(STATUS "Shader source found: ${SHADER_PATH}")
            set(ASSETS_COPIED TRUE)
            break()
        endif()
    endforeach()
    
    # Copy main Assets directory if it exists
    foreach(ASSET_PATH IN ITEMS
        "${CMAKE_SOURCE_DIR}/Spark Engine/Assets"
        "${CMAKE_SOURCE_DIR}/Assets")
        
        if(EXISTS "${ASSET_PATH}")
            add_custom_command(TARGET SparkEngine POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${ASSET_PATH}"
                "$<TARGET_FILE_DIR:SparkEngine>/Assets"
                COMMENT "Copying assets from ${ASSET_PATH}")
                
            message(STATUS "Asset source found: ${ASSET_PATH}")
            set(ASSETS_COPIED TRUE)
            break()
        endif()
    endforeach()
    
    # Copy Models, Textures, Audio, Scripts, Config (similar pattern)
    foreach(TYPE_PATH IN ITEMS "Models;Models" "Textures;Textures" "Audio;Audio" "Scripts;Scripts" "Config;Config")
        string(REPLACE ";" "=" TYPE_PAIR "${TYPE_PATH}")
        string(REPLACE "=" ";" TYPE_PAIR "${TYPE_PAIR}")
        list(GET TYPE_PAIR 0 TYPE_NAME)
        list(GET TYPE_PAIR 1 TYPE_DIR)
        
        foreach(SEARCH_PATH IN ITEMS
            "${CMAKE_SOURCE_DIR}/Spark Engine/${TYPE_DIR}"
            "${CMAKE_SOURCE_DIR}/${TYPE_DIR}"
            "${CMAKE_SOURCE_DIR}/Assets/${TYPE_DIR}")
            
            if(EXISTS "${SEARCH_PATH}")
                add_custom_command(TARGET SparkEngine POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${SEARCH_PATH}"
                    "$<TARGET_FILE_DIR:SparkEngine>/${TYPE_DIR}"
                    COMMENT "Copying ${TYPE_NAME} from ${SEARCH_PATH}")
                    
                message(STATUS "${TYPE_NAME} source found: ${SEARCH_PATH}")
                set(ASSETS_COPIED TRUE)
            endif()
        endforeach()
    endforeach()
    
    # Copy individual important files
    set(IMPORTANT_FILES "settings.ini" "config.json" "engine.cfg" "game.properties" "version.txt")
    
    foreach(FILE_NAME ${IMPORTANT_FILES})
        foreach(SEARCH_PATH IN ITEMS 
            "${CMAKE_SOURCE_DIR}/${FILE_NAME}"
            "${CMAKE_SOURCE_DIR}/Spark Engine/${FILE_NAME}")
            
            if(EXISTS "${SEARCH_PATH}")
                add_custom_command(TARGET SparkEngine POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${SEARCH_PATH}"
                    "$<TARGET_FILE_DIR:SparkEngine>/${FILE_NAME}"
                    COMMENT "Copying ${FILE_NAME}")
                    
                message(STATUS "Important file found: ${SEARCH_PATH}")
                set(ASSETS_COPIED TRUE)
            endif()
        endforeach()
    endforeach()
    
    # Create asset directories in build output even if sources don't exist
    add_custom_command(TARGET SparkEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Shaders"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Shaders/HLSL"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Assets" 
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Models"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Textures"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Audio"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Scripts"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:SparkEngine>/Config"
        COMMENT "Creating asset directory structure")
        
    # Status reporting
    if(ASSETS_COPIED)
        message(STATUS "Asset copying configured successfully")
    else()
        message(WARNING "No asset directories found - assets will need to be manually copied")
    endif()
endfunction()

# Execute the asset copying
copy_assets_to_build()

# -------------------------------------------------------------
# 9) Include directories WITH CURL HEADERS
# -------------------------------------------------------------
target_include_directories(SparkEngine PRIVATE 
    "Spark Engine/Source"
    "${CURL_ROOT}/include"
    "${CMAKE_BINARY_DIR}/ThirdParty/Networking/curl/include"
)

# -------------------------------------------------------------
# 10) LINK CURL WITH ACTUAL TARGET NAME
# -------------------------------------------------------------
target_link_libraries(SparkEngine PRIVATE libcurl_static)
target_compile_definitions(SparkEngine PRIVATE NETWORKING_ENABLED CURL_STATICLIB)

# -------------------------------------------------------------
# 11) System libraries
# -------------------------------------------------------------
target_link_libraries(SparkEngine PRIVATE
    d3d11 dxgi d3dcompiler dxguid
    winmm ws2_32 wsock32
    ole32 oleaut32 uuid
    kernel32 user32 gdi32 winspool
    shell32 comdlg32 advapi32
    crypt32 wldap32 normaliz
)

# -------------------------------------------------------------
# 12) Third-party libraries - FIXED tinyobjloader
# -------------------------------------------------------------
set(MINIZ_FOUND FALSE)
foreach(PATH IN ITEMS
    "ThirdParty/Utils/miniz"
    "ThirdParty/Compression/miniz"
    "ThirdParty/miniz"
)
    if(EXISTS "${CMAKE_SOURCE_DIR}/${PATH}/CMakeLists.txt")
        add_subdirectory(${PATH} EXCLUDE_FROM_ALL)
        target_link_libraries(SparkEngine PRIVATE miniz)
        set(MINIZ_FOUND TRUE)
        message(STATUS "Found miniz at: ${PATH}")
        break()
    endif()
endforeach()

# FIXED: tinyobjloader is header-only - create implementation file
set(TINYOBJ_FOUND FALSE)
foreach(PATH IN ITEMS
    "ThirdParty/Utils/tinyobjloader"
    "ThirdParty/FileFormats/tinyobjloader"
    "ThirdParty/Graphics/tinyobjloader"
    "ThirdParty/tinyobjloader"
)
    if(EXISTS "${CMAKE_SOURCE_DIR}/${PATH}/tiny_obj_loader.h")
        # Create implementation file for header-only library
        set(TINYOBJ_IMPL_FILE "${CMAKE_BINARY_DIR}/tinyobj_impl.cpp")
        file(WRITE ${TINYOBJ_IMPL_FILE} 
             "#define TINYOBJLOADER_IMPLEMENTATION\n"
             "#include \"tiny_obj_loader.h\"\n")
        
        target_sources(SparkEngine PRIVATE ${TINYOBJ_IMPL_FILE})
        target_include_directories(SparkEngine PRIVATE "${CMAKE_SOURCE_DIR}/${PATH}")
        set(TINYOBJ_FOUND TRUE)
        message(STATUS "Found tinyobjloader at: ${PATH}")
        break()
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/${PATH}/include/tiny_obj_loader.h")
        # Create implementation file for header-only library
        set(TINYOBJ_IMPL_FILE "${CMAKE_BINARY_DIR}/tinyobj_impl.cpp")
        file(WRITE ${TINYOBJ_IMPL_FILE} 
             "#define TINYOBJLOADER_IMPLEMENTATION\n"
             "#include \"tiny_obj_loader.h\"\n")
        
        target_sources(SparkEngine PRIVATE ${TINYOBJ_IMPL_FILE})
        target_include_directories(SparkEngine PRIVATE "${CMAKE_SOURCE_DIR}/${PATH}/include")
        set(TINYOBJ_FOUND TRUE)
        message(STATUS "Found tinyobjloader at: ${PATH}/include")
        break()
    endif()
endforeach()

# -------------------------------------------------------------
# 13) Lua
# -------------------------------------------------------------
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

# -------------------------------------------------------------
# 14) PhysX
# -------------------------------------------------------------
if(ENABLE_PHYSX)
    if(EXISTS "${CMAKE_SOURCE_DIR}/ThirdParty/Physics/PhysX/include")
        target_include_directories(SparkEngine PRIVATE "ThirdParty/Physics/PhysX/include")
        target_compile_definitions(SparkEngine PRIVATE PHYSX_AVAILABLE)
        message(STATUS "PhysX headers found")
    else()
        message(WARNING "PhysX not found")
    endif()
endif()

# -------------------------------------------------------------
# 15) Feature definitions
# -------------------------------------------------------------
target_compile_definitions(SparkEngine PRIVATE 
    EDITOR_ENABLED
    GRAPHICS_ENABLED
    COLLABORATIVE_ENABLED
    PROFILING_ENABLED
    ASSET_STREAMING_ENABLED
    HOT_RELOAD_ENABLED
    TERRAIN_SYSTEM_ENABLED
    POST_PROCESSING_ENABLED
    LIGHTING_SYSTEM_ENABLED
    ADVANCED_INPUT_ENABLED
)

# -------------------------------------------------------------
# 16) Visual Studio settings
# -------------------------------------------------------------
foreach(src ${SPARK_ENGINE_SOURCES})
    get_filename_component(dir "${src}" DIRECTORY)
    file(RELATIVE_PATH grp "${CMAKE_SOURCE_DIR}/Spark Engine/Source" "${dir}")
    string(REPLACE "/" "\\" grp "${grp}")
    if(grp STREQUAL "")
        source_group("Source Files" FILES "${src}")
    else()
        source_group("Source Files\\${grp}" FILES "${src}")
    endif()
endforeach()

if(MSVC)
    set_property(TARGET SparkEngine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SparkEngine)
    
    # Add asset file extensions to project filters
    source_group("Assets\\Shaders" FILES "*.hlsl" "*.fx" "*.glsl")
    source_group("Assets\\Models" FILES "*.obj" "*.fbx" "*.dae" "*.3ds" "*.ply")
    source_group("Assets\\Textures" FILES "*.png" "*.jpg" "*.jpeg" "*.tga" "*.dds" "*.bmp")
    source_group("Assets\\Audio" FILES "*.wav" "*.mp3" "*.ogg" "*.flac")
    source_group("Assets\\Scripts" FILES "*.lua" "*.js" "*.py")
    source_group("Assets\\Config" FILES "*.ini" "*.json" "*.cfg" "*.xml" "*.properties")
endif()

message(STATUS "SPARKENGINE CONFIGURED SUCCESSFULLY")