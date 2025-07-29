#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Angelscript::angelscript" for configuration "Release"
set_property(TARGET Angelscript::angelscript APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Angelscript::angelscript PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "ASM_MASM;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/angelscript.lib"
  )

list(APPEND _cmake_import_check_targets Angelscript::angelscript )
list(APPEND _cmake_import_check_files_for_Angelscript::angelscript "${_IMPORT_PREFIX}/lib/angelscript.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
