# Copyright Institute of Sound and Vibration Research - All rights reserved

# Top-level build configuration for project VISceRal

# Arbitrarily set minimum version.
# This specific version is used to avoid complaints about the
# cygwin platform not defining WIN32 anymore.
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.4)

# Avoid warnings about the CXX_VISIBILITY_PRESET set for all target types.
if( POLICY CMP0063 ) # (only if the CMake version supports this policy
  CMAKE_POLICY(SET CMP0063 NEW)
endif( POLICY CMP0063 )
# Policy for supporting <package>_ROOT CMake or environment variables in find_package() (CMake >= 3.12 only)
if( POLICY CMP0074 )
  CMAKE_POLICY(SET CMP0074 NEW)
endif( POLICY CMP0074 )

# The name of our top-level project.
PROJECT( VISR_PRODUCTION_SUITE CXX C )

# Define central version numbers for the project
# TODO: Decide whether to move that to an external CMake file .
set( VISR_PRODUCTION_SUITE_MAJOR_VERSION 0 )
set( VISR_PRODUCTION_SUITE_MINOR_VERSION 12 )
set( VISR_PRODUCTION_SUITE_PATCH_VERSION 0 )

ADD_DEFINITIONS( -DVISR_PRODUCTION_SUITE_MAJOR_VERSION=${VISR_PRODUCTION_SUITE_MAJOR_VERSION} )
ADD_DEFINITIONS( -DVISR_PRODUCTION_SUITE_MINOR_VERSION=${VISR_PRODUCTION_SUITE_MINOR_VERSION} )
ADD_DEFINITIONS( -DVISR_PRODUCTION_SUITE_PATCH_VERSION=${VISR_PRODUCTION_SUITE_PATCH_VERSION} )

SET( VISR_PRODUCTION_SUITE_VERSION_STRING ${VISR_PRODUCTION_SUITE_MAJOR_VERSION}.${VISR_PRODUCTION_SUITE_MINOR_VERSION}.${VISR_PRODUCTION_SUITE_PATCH_VERSION})
SET( VISR_PRODUCTION_SUITE_TOPLEVEL_NAME VISR-Production-Suite-${VISR_PRODUCTION_SUITE_VERSION_STRING})

add_compile_definitions( NDEBUG )

# Add support for unit tests
INCLUDE( CTest )

# Enable C++11 features on Linux platforms
# See http://www.cmake.org/Wiki/CMake/Tutorials/C%2B%2B11Flags
# Maybe there is a more general way to configure the different platforms and toolchains
if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")
#message( STATUS "output "${CMAKE_MODULE_PATH} )
# At the moment, all include directories are relative to the source root directory.
# Therefore it suffices to set this root as the include path.
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/src )

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake_modules)

# Default path for static libraries
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/staticlib )

# Copy the executables to the binary directory
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

# Default destination for shared libraries (not DLLs)
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib )

# Default path for DLLs on windows
SET( RUNTIME_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib )

#############################################################
# Create a system string.
SET( VISR_PRODUCTION_SUITE_SYSTEM_NAME ${CMAKE_SYSTEM_NAME} )
IF( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "Darwin" )

SET(CMAKE_INSTALL_PREFIX /Applications/VISR-Production-Suite)

SET( CMAKE_MACOSX_RPATH ON)
# use, i.e. don't skip the full RPATH for the build tree
# TRUE effect: with CMAKE_BUILD_WITH_INSTALL_RPATH=FALSE VISR-Production-Suite target_linked libraries are no longer prepended with @rpath during build
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
# when building, don't use the install RPATH already
# (but later on when installing)

# <af> Temporarily disabled this feature to enable building without installing.
# <af> This probably breaks the package building mechanism.
# TRUE effect: with CMAKE_BUILD_WITH_INSTALL_RPATH=TRUE VISR-Production-Suite target_linked libraries are prepended with @rpath during build
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
#SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib;${CMAKE_INSTALL_PREFIX}/3rd")
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/3rd")
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)


   SET( VISR_PRODUCTION_SUITE_SYSTEM_NAME MacOS )
#  SET(CMAKE_INSTALL_PREFIX /Applications )
ENDIF( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "Darwin" )

SET( VISR_PRODUCTION_SUITE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} )
IF( VISR_PRODUCTION_SUITE_SYSTEM_PROCESSOR MATCHES "AMD64" )
  SET( VISR_PRODUCTION_SUITE_SYSTEM_PROCESSOR x86_64 )
ENDIF( VISR_PRODUCTION_SUITE_SYSTEM_PROCESSOR MATCHES "AMD64" )
STRING( TOLOWER ${VISR_PRODUCTION_SUITE_SYSTEM_NAME}-${VISR_PRODUCTION_SUITE_SYSTEM_PROCESSOR} VISR_PRODUCTION_SUITE_PLATFORM_NAME )
# For use as a C/C++ preprocessor definition, we must not use hyphens.
STRING( TOLOWER ${VISR_PRODUCTION_SUITE_SYSTEM_NAME}_${VISR_PRODUCTION_SUITE_SYSTEM_PROCESSOR} VISR_PRODUCTION_SUITE_PLATFORM_NAME_MACRO )


# MESSAGE( STATUS "VISR-Production-Suite system processor: " ${VISR_PRODUCTION_SUITE_SYSTEM_PROCESSOR} )
# MESSAGE( STATUS "VISR-Production-Suite system name:      " ${VISR_PRODUCTION_SUITE_SYSTEM_NAME} )
# MESSAGE( STATUS "VISR-Production-Suite platform name:    " ${VISR_PRODUCTION_SUITE_PLATFORM_NAME} )

# Make the platform strings available to the source code.
ADD_DEFINITIONS("-DVISR_PRODUCTION_SUITE_SYSTEM_PROCESSOR_${VISR_PRODUCTION_SUITE_SYSTEM_PROCESSOR}")
ADD_DEFINITIONS("-DVISR_PRODUCTION_SUITE_SYSTEM_NAME_${VISR_PRODUCTION_SUITE_SYSTEM_NAME}")
ADD_DEFINITIONS("-DVISR_PRODUCTION_SUITE_PLATFORM_NAME_${VISR_PRODUCTION_SUITE_PLATFORM_NAME_MACRO}")

############################################################
# Set platform-specific compiler flags

# This covers Linux and MacOS X
if(UNIX)
  # Support for the Raspberry Pi: Enable hardware floating-point arithmetic
  if( ${VISR_PRODUCTION_SUITE_SYSTEM_PROCESSOR} MATCHES "armv7l" )
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon -march=armv7-a -mfloat-abi=hard" )
  endif( ${VISR_PRODUCTION_SUITE_SYSTEM_PROCESSOR} MATCHES "armv7l" )
  # TODO: Add more elaborate configuration for -march and -mtune
  # At the moment, the code is invariably built for the architecture
  # features of the machine it is compiled on.
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall --pedantic" )
  # Avoid super-picky 'nested anon types' warning
  if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang") # Matches regular Clang and AppleClang (XCode)
    # message( STATUS "Detected LLVM C++ compiler" )
    # Remove nonsensical warnings due to unknown pragmas introduced by -pedantic
    # (as pragmas are compiler-specific, unknown ones should be ignored silently anyway)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-nested-anon-types -Wno-unknown-pragmas" )
  endif (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -std=c++14")
endif(UNIX)

if( WIN32 ) # Covers 64-bit Windows as well
  # Macro to determine the Windows version, stolen from
  # http://stackoverflow.com/questions/9742003/platform-detection-in-cmake
  macro(get_win32_winnt_version version)
    if (WIN32 AND CMAKE_SYSTEM_VERSION)
        set(ver ${CMAKE_SYSTEM_VERSION})
        string(REPLACE "." "" ver ${ver})
        string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})
        set(${version} "0x${ver}")
    endif()
  endmacro()
  get_win32_winnt_version( WIN_VERSION_ID )
  message( STATUS "Window version: " ${WIN_VERSION_ID} )
  add_definitions( -D_WINN32_WINNT=${WIN_VERSION_ID} )
  # Avoid specific compiler warnings of the Visual Studio  compiler.
  add_definitions( -D_SCL_SECURE_NO_WARNINGS ) # STL algorithms with pointers instead of 'real' iterators are perfectly legal.
  # Avoid complaints about STL members in classes with VISR_CORE_LIBRARY_SYMBOL not having DLL interfaces.
  add_definitions( "/wd4251" )
  #
#   ADD_DEFINITIONS( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4005 /wd4996 /wd4251 /nologo" )
   add_definitions( "-D_WIN32_WINNT=0x0601" )


  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3" ) # Increase warning level (/W4 and /Wall generate lots of output not related to coding flaws.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")

 FIND_PACKAGE( SndFile REQUIRED )
 SET(CMAKE_INSTALL_PREFIX "C:/Program Files/VISR-Production-Suite")
endif( WIN32 )


############################################################
# Set the minimum Mac OS version.
if( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "MacOS" )
#  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version")
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9")
#  message( STATUS PRODTOOLS "Macos version: " ${CMAKE_OSX_DEPLOYMENT_TARGET} )
endif( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "MacOS" )

############################################################
# Set default visibility of shared library symbols to hidden (as on Windows platforms)
#SET( CMAKE_C_VISIBILITY_PRESET hidden )
#SET( CMAKE_CXX_VISIBILITY_PRESET hidden )

SET( CMAKE_C_VISIBILITY_PRESET default )
SET( CMAKE_CXX_VISIBILITY_PRESET default )
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


###########################################################

FIND_PACKAGE( Juce REQUIRED)
SET(JUCE_SOURCES_FOLDER ${CMAKE_BINARY_DIR}/resources/juce/sources)
foreach(dir ${JUCE_MODULES})
	get_filename_component(module_name ${dir} NAME)
#	message( STATUS "module " ${JUCE_SOURCES_${dir}} )
	get_filename_component(file_name ${JUCE_SOURCES_${dir}} NAME)
	file(WRITE ${JUCE_SOURCES_FOLDER}/include_${file_name} "#include \"AppConfig.h\"\n#include <${module_name}/${file_name}>")
#	list(APPEND ${CurrLib}_SOURCES ${JUCE_SOURCES_FOLDER}/include_${file_name})
	#list(APPEND ${CurrLib}_HEADERS ${JUCE_HEADERS_${dir}})
endforeach(dir ${JUCE_MODULES})
get_filename_component(wrapper_module_name ${JUCE_PLUGIN_WRAPPER_DIR} NAME)


file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_utils.cpp "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_utils.cpp>")
file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_VST2.cpp "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_VST2.cpp>")
file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_VST3.cpp "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_VST3.cpp>")
file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_Standalone.cpp "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_Standalone.cpp>")

if (APPLE)
	file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_VST_utils.mm "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_VST_utils.mm>")
	file(WRITE ${JUCE_SOURCES_FOLDER}/juce_audio_plugin_client_AAX.mm "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_AAX.mm>")
	file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_AU_1.mm "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_AU_1.mm>")
    file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_AU_2.mm "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_AU_2.mm>")
else (APPLE)
	file(WRITE ${JUCE_SOURCES_FOLDER}/juce_audio_plugin_client_AAX.cpp "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_AAX.cpp>")
	list(APPEND AAX_SOURCES ${JUCE_SOURCES_FOLDER}/juce_audio_plugin_client_AAX.cpp)
endif (APPLE)


OPTION( BUILD_ECOSYSTEM_ONLY "enable only a certain subset of the available plugins, which belong to the S3A project's object-based ecosystem" ON )
OPTION( BUILD_MDO OFF )
OPTION( BUILD_REVERB OFF)
OPTION( BUILD_CAP OFF)
OPTION( BUILD_DAWPLUGINS_VST2 "enable VST2 plugins, including Juce library building" OFF )
OPTION( BUILD_DAWPLUGINS_VST3 "enable VST3 plugins, including Juce library building" ON )
OPTION( BUILD_DAWPLUGINS_AAX "enable AAX plugins, including Juce library building" OFF )
OPTION( BUILD_PLUGINS_IN_BUILD_FOLDER OFF )


if(BUILD_DAWPLUGINS_VST2)
    FIND_PACKAGE( VST REQUIRED)
endif (BUILD_DAWPLUGINS_VST2)
if(BUILD_DAWPLUGINS_AAX)
    FIND_PACKAGE( AAX REQUIRED)
endif (BUILD_DAWPLUGINS_AAX)


if (APPLE)
OPTION( BUILD_DAWPLUGINS_AU "enable AU plugins, including Juce library building" OFF)
endif (APPLE)

OPTION( BUILD_DAWPLUGINS_STANDALONE "enable standalone version of the plugin, including Juce library building" OFF )

include (oslibs)

###########################################################
# Support for 3rd-party libraries.

find_package( VISR 0.12.2 REQUIRED)

## Boost support
# On Linux and Windows, the boost libraries provided by the OS image (or brew on MacOS)
# should be found automatically.
# Otherwise (in particular for Windows platforms), an environment variable
# BOOST_ROOT should hold the path to the boost root directory.

OPTION( Boost_USE_STATIC_LIBS OFF )

# For the moment, we support only dynamic linking of boost.
# In this case, this line is required on Windows to prevent undefined symbols from boost::program_options

IF( NOT Boost_USE_STATIC_LIBS )
  ADD_DEFINITIONS("-DBOOST_ALL_DYN_LINK")
ENDIF( NOT Boost_USE_STATIC_LIBS )

# Avoid problems with the auto-link feature on Windows platforms
ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")

# Define the boost libraries that must be present.
# In boost 1.59 on Windows, chrono is an implicit dependency of boost_thread. So in order to locate the runtime DLL for packaging, we mus add this library here.
SET( VISR_PRODUCTION_SUITE_BOOST_LIBRARIES filesystem system)
# Note that boost regex is used only with GCC <= 4.8 on Linux because of the faulty std::regex implementation in these GCC versions.
# However, we do not exclude boost::regex here because of the number of places it is handled in the build structure (especially packaging)

# Note: The minimum boost requirement is arbitrarily chosen at the moment.
# Todo: Replace by a substantiated version requirement.
FIND_PACKAGE( Boost 1.50 REQUIRED COMPONENTS ${VISR_PRODUCTION_SUITE_BOOST_LIBRARIES} )
# TODO: Switch to imported targets and remove global include directory ASAP
#INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )




################################################################################
# Set the install directories of special output types.
# For Linux this adjusts the path to put the items under /usr/share/VISR-Production-Suite

if( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "Linux" )
  set( VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY "share/VISR-Production-Suite" ) # Directory under CMAKE_INSTALL_PREFIX where VISR-Production-Suite-specific content is placed.
else( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "Linux" )
  set( VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY "./" ) # CMAKE_INSTALL_PREFIX is already OK
endif( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "Linux" )

set( DOCUMENTATION_INSTALL_DIRECTORY "${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/doc" )

message( STATUS "output " ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY} )

################################################################################
# Adjust the location of 3rd-party libraries.
# This is required on Mac OS X. Essentially the files are copied to <build/3rd>
# and the rpaths of of inter-library dependencies are adjusted.
# NOTE: This has to happen before the add_subdirectory( src )
if( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "MacOS" )
  include( Adjust3rdPartyLibraries )
endif( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "MacOS" )
################################################################################


################################################################################
# Actual software components (subprojects) to be built are
# contained in the src/ directory
# TODO: This is a quite hidden location for setting the subdirectories and subprojects.
# Restructure the main build file in order to make this line more prominent.
ADD_SUBDIRECTORY( src )



OPTION( BUILD_PYTHON_PLUGINS "Python bindings of selected VISR functionality" OFF )

################################################################################
## Support for building Doxygen documentation
## Taken from: http://mementocodex.wordpress.com/2013/01/19/how-to-generate-code-documentation-with-doxygen-and-cmake-a-slightly-improved-approach/

option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" OFF)
IF(BUILD_DOCUMENTATION)
    add_subdirectory(doc)
ENDIF()

# Instructions for packaging are held in a separate file.
INCLUDE( PackageConfig )
