# Install script for directory: /Users/marcel/desktop/SuperTuxKartAI/stk-code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "STKRelease")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE DIRECTORY FILES "/Users/marcel/desktop/SuperTuxKartAI/stk-code/cmake_build/bin/supertuxkart.app" USE_SOURCE_PERMISSIONS)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./supertuxkart.app/Contents/MacOS/supertuxkart" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./supertuxkart.app/Contents/MacOS/supertuxkart")
    execute_process(COMMAND /Users/marcel/anaconda3/bin/install_name_tool
      -delete_rpath "/Users/marcel/desktop/pytorch/torch/lib"
      -delete_rpath "/Users/marcel/desktop/supertuxkartai/stk-code/cmake_build/../dependencies-macosx/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./supertuxkart.app/Contents/MacOS/supertuxkart")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/supertuxkart" TYPE DIRECTORY FILES "/Users/marcel/desktop/SuperTuxKartAI/stk-code/data" REGEX "/\\.svn$" EXCLUDE REGEX "/\\.git$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/supertuxkart/data" TYPE DIRECTORY FILES "/Users/marcel/desktop/SuperTuxKartAI/stk-code/../stk-assets/" REGEX "/\\.svn$" EXCLUDE REGEX "/\\.git$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications" TYPE FILE FILES "/Users/marcel/desktop/SuperTuxKartAI/stk-code/data/supertuxkart.desktop")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/16x16/apps" TYPE FILE RENAME "supertuxkart.png" FILES "/Users/marcel/desktop/SuperTuxKartAI/stk-code/data/supertuxkart_16.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/32x32/apps" TYPE FILE RENAME "supertuxkart.png" FILES "/Users/marcel/desktop/SuperTuxKartAI/stk-code/data/supertuxkart_32.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/48x48/apps" TYPE FILE RENAME "supertuxkart.png" FILES "/Users/marcel/desktop/SuperTuxKartAI/stk-code/data/supertuxkart_48.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/64x64/apps" TYPE FILE RENAME "supertuxkart.png" FILES "/Users/marcel/desktop/SuperTuxKartAI/stk-code/data/supertuxkart_64.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/128x128/apps" TYPE FILE RENAME "supertuxkart.png" FILES "/Users/marcel/desktop/SuperTuxKartAI/stk-code/data/supertuxkart_128.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/256x256/apps" TYPE FILE RENAME "supertuxkart.png" FILES "/Users/marcel/desktop/SuperTuxKartAI/stk-code/data/supertuxkart_256.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/512x512/apps" TYPE FILE RENAME "supertuxkart.png" FILES "/Users/marcel/desktop/SuperTuxKartAI/stk-code/data/supertuxkart_512.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/1024x1024/apps" TYPE FILE RENAME "supertuxkart.png" FILES "/Users/marcel/desktop/SuperTuxKartAI/stk-code/data/supertuxkart_1024.png")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/metainfo" TYPE FILE FILES "/Users/marcel/desktop/SuperTuxKartAI/stk-code/data/supertuxkart.appdata.xml")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/marcel/desktop/SuperTuxKartAI/stk-code/cmake_build/lib/bullet/cmake_install.cmake")
  include("/Users/marcel/desktop/SuperTuxKartAI/stk-code/cmake_build/lib/enet/cmake_install.cmake")
  include("/Users/marcel/desktop/SuperTuxKartAI/stk-code/cmake_build/lib/libsquish/cmake_install.cmake")
  include("/Users/marcel/desktop/SuperTuxKartAI/stk-code/cmake_build/lib/graphics_engine/cmake_install.cmake")
  include("/Users/marcel/desktop/SuperTuxKartAI/stk-code/cmake_build/lib/graphics_utils/cmake_install.cmake")
  include("/Users/marcel/desktop/SuperTuxKartAI/stk-code/cmake_build/lib/tinygettext/cmake_install.cmake")
  include("/Users/marcel/desktop/SuperTuxKartAI/stk-code/cmake_build/lib/mcpp/cmake_install.cmake")
  include("/Users/marcel/desktop/SuperTuxKartAI/stk-code/cmake_build/lib/irrlicht/cmake_install.cmake")
  include("/Users/marcel/desktop/SuperTuxKartAI/stk-code/cmake_build/lib/wiiuse/cmake_install.cmake")
  include("/Users/marcel/desktop/SuperTuxKartAI/stk-code/cmake_build/lib/angelscript/projects/cmake/cmake_install.cmake")
  include("/Users/marcel/desktop/SuperTuxKartAI/stk-code/cmake_build/lib/mojoal/cmake_install.cmake")
  include("/Users/marcel/desktop/SuperTuxKartAI/stk-code/cmake_build/lib/sheenbidi/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/marcel/desktop/SuperTuxKartAI/stk-code/cmake_build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
