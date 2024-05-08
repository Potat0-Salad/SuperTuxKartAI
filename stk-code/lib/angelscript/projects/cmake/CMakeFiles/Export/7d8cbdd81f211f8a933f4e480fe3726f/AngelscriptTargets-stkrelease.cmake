#----------------------------------------------------------------
# Generated CMake target import file for configuration "STKRelease".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Angelscript::angelscript" for configuration "STKRelease"
set_property(TARGET Angelscript::angelscript APPEND PROPERTY IMPORTED_CONFIGURATIONS STKRELEASE)
set_target_properties(Angelscript::angelscript PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_STKRELEASE "CXX"
  IMPORTED_LOCATION_STKRELEASE "${_IMPORT_PREFIX}/lib/libangelscript.a"
  )

list(APPEND _cmake_import_check_targets Angelscript::angelscript )
list(APPEND _cmake_import_check_files_for_Angelscript::angelscript "${_IMPORT_PREFIX}/lib/libangelscript.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
