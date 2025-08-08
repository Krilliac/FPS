cmake_minimum_required(VERSION 3.16)
project(SparkConsole)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# All source files in src directory
file(GLOB CONSOLE_SOURCES "src/*.cpp")
file(GLOB CONSOLE_HEADERS "src/*.h")

# Create executable
add_executable(SparkConsole ${CONSOLE_SOURCES} ${CONSOLE_HEADERS})

# Output settings
set_target_properties(SparkConsole PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Compiler settings
if(MSVC)
    target_compile_options(SparkConsole PRIVATE /W3 /MP /EHsc)
    target_compile_definitions(SparkConsole PRIVATE _CRT_SECURE_NO_WARNINGS WIN32_LEAN_AND_MEAN)
    set_property(TARGET SparkConsole PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()

# Source groups
source_group("Source Files" FILES ${CONSOLE_SOURCES})
source_group("Header Files" FILES ${CONSOLE_HEADERS})

message(STATUS "SparkConsole configured")message(STATUS "SparkConsole configured successfully")