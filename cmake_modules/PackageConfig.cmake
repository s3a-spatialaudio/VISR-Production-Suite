# Package generation configuration
#message( STATUS "VISR INST " ${CMAKE_INSTALL_PREFIX} )

SET( CPACK_PACKAGE_NAME "VISR-Production-Suite" )
SET( CPACK_PACKAGE_VENDOR "ISVR")

# Use the version numbers centrally defined for the VISR-Production-Suite project
set( CPACK_PACKAGE_VERSION_MAJOR ${VISR_PRODUCTION_SUITE_MAJOR_VERSION} )
set( CPACK_PACKAGE_VERSION_MINOR ${VISR_PRODUCTION_SUITE_MINOR_VERSION} )
set( CPACK_PACKAGE_VERSION_PATCH ${VISR_PRODUCTION_SUITE_PATCH_VERSION} )

set( PKG_FILE_NAME ${VISR_PRODUCTION_SUITE_TOPLEVEL_NAME}-${CMAKE_SYSTEM_NAME})
# Encode the Python major/minor version in the package file name if Python is enabled.
if( BUILD_PYTHON_PLUGINS )
  # Extract the Python version encoded as a compile definition in libpythonsupport.
  get_property( PYTHONSUPPORT_DEFINITIONS TARGET VISR::pythonsupport_shared PROPERTY INTERFACE_COMPILE_DEFINITIONS )
  message( STATUS "PYTHONSUPPORT_DEFINITIONS: " ${PYTHONSUPPORT_DEFINITIONS} )
  string( REGEX REPLACE ".*=([0-9]+.[0-9]+).*" "\\1" PYTHON_MAJOR_MINOR ${PYTHONSUPPORT_DEFINITIONS} )
  # message( STATUS "PYLIB: " ${PYTHONSUPPORT_DEFINITIONS})
  set( PKG_FILE_NAME ${VISR_PRODUCTION_SUITE_TOPLEVEL_NAME}-Python${PYTHON_MAJOR_MINOR}-${CMAKE_SYSTEM_NAME} )
else()
  set( PKG_FILE_NAME ${VISR_PRODUCTION_SUITE_TOPLEVEL_NAME}-${CMAKE_SYSTEM_NAME} )
endif( BUILD_PYTHON_PLUGINS )

set( CPACK_PACKAGE_FILE_NAME ${PKG_FILE_NAME} )
get_property(VST2_GROUP_COMPONENTS GLOBAL PROPERTY vst2_group)
foreach(comp ${VST2_GROUP_COMPONENTS})
	set(CPACK_COMPONENT_${comp}_GROUP "VST")
#	message( STATUS "VSTPLUG INST " ${comp})
endforeach(comp ${VST2_GROUP_COMPONENTS})

get_property(VST3_GROUP_COMPONENTS GLOBAL PROPERTY vst3_group)
foreach(comp ${VST3_GROUP_COMPONENTS})
	set(CPACK_COMPONENT_${comp}_GROUP "VST3")
endforeach(comp ${VST3_GROUP_COMPONENTS})

get_property(AU_GROUP_COMPONENTS GLOBAL PROPERTY au_group)
foreach(comp ${AU_GROUP_COMPONENTS})
	set(CPACK_COMPONENT_${comp}_GROUP "AU")
endforeach(comp ${AU_GROUP_COMPONENTS})

get_property(AAX_GROUP_COMPONENTS GLOBAL PROPERTY aax_group)
foreach(comp ${AAX_GROUP_COMPONENTS})
	set(CPACK_COMPONENT_${comp}_GROUP "AAX")
endforeach(comp ${AAX_GROUP_COMPONENTS})

get_property(STANADLONE_GROUP_COMPONENTS GLOBAL PROPERTY standalone_group)
foreach(comp ${STANADLONE_GROUP_COMPONENTS})
	set(CPACK_COMPONENT_${comp}_GROUP "Standalone")
endforeach(comp ${STANADLONE_GROUP_COMPONENTS})

if( BUILD_REVERB )
set(CPACK_COMPONENT_REVERB_RESOURCES_DISABLED 1)

set(CPACK_COMPONENT_REVERBOBJECTEDITORVST_DISABLED 1)
set(CPACK_COMPONENT_REVERBOBJECTEDITORVST3_DISABLED 1)
set(CPACK_COMPONENT_REVERBOBJECTEDITORAU_DISABLED 1)
set(CPACK_COMPONENT_REVERBOBJECTEDITORSTANDALONE_DISABLED 1)
set(CPACK_COMPONENT_REVERBOBJECTEDITORAAX_DISABLED 1)
endif( BUILD_REVERB )

if( BUILD_MDO )
if( BUILD_PYTHON_PLUGINS )
set(CPACK_COMPONENT_METADAPTERRENDERERVST_DISABLED 1)
set(CPACK_COMPONENT_METADAPTERRENDERERVST3_DISABLED 1)
set(CPACK_COMPONENT_METADAPTERRENDERERAU_DISABLED 1)
set(CPACK_COMPONENT_METADAPTERRENDERERSTANDALONE_DISABLED 1)
set(CPACK_COMPONENT_METADAPTERRENDERERAAX_DISABLED 1)
endif( BUILD_PYTHON_PLUGINS )

set(CPACK_COMPONENT_MDO_RESOURCES_DISABLED 1)

set(CPACK_COMPONENT_METADATAEDITORVST_DISABLED 1)
set(CPACK_COMPONENT_METADATAEDITORVST3_DISABLED 1)
set(CPACK_COMPONENT_METADATAEDITORAU_DISABLED 1)
set(CPACK_COMPONENT_METADATAEDITORSTANDALONE_DISABLED 1)
set(CPACK_COMPONENT_METADATAEDITORAAX_DISABLED 1)
endif( BUILD_MDO )

if( BUILD_CAP )
set(CPACK_COMPONENT_CAP_RESOURCES_DISABLED 1)

set(CPACK_COMPONENT_CAPRENDERERVST_DISABLED 1)
set(CPACK_COMPONENT_CAPRENDERERVST3_DISABLED 1)
set(CPACK_COMPONENT_CAPRENDERERAU_DISABLED 1)
set(CPACK_COMPONENT_CAPRENDERERSTANDALONE_DISABLED 1)
set(CPACK_COMPONENT_CAPRENDERERRAAX_DISABLED 1)
endif( BUILD_CAP )




set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Libraries")
set(CPACK_COMPONENT_VISR_DISPLAY_NAME "VISR")

set(CPACK_COMPONENT_LIBRARIES_REQUIRED 1)
#set(CPACK_COMPONENT_VISR_DISABLED 1)

############ VISR libraries and resources ######################################################

get_property(visr_root TARGET VISR::apputilities_shared PROPERTY LOCATION_RELEASE)
get_filename_component(visr_root ${visr_root} DIRECTORY)
get_filename_component(visr_root ${visr_root} DIRECTORY)

get_filename_component(visr_root_lib ${visr_root} NAME)
message( STATUS "VISR INST LIB " ${visr_root_lib})

IF( visr_root_lib MATCHES "lib" )
get_filename_component(visr_root ${visr_root} DIRECTORY)
ENDIF( visr_root_lib MATCHES "lib" )
message( STATUS "VISR INST " ${visr_root})

if( BUILD_PYTHON_PLUGINS )
SET( VISR_LIBRARIES efl objectmodel panning pml pythoncomponents pythonsupport rbbl rcl reverbobject rrl signalflows signalflowspython visr)
else( BUILD_PYTHON_PLUGINS )
SET( VISR_LIBRARIES efl objectmodel panning pml rbbl rcl reverbobject rrl signalflows visr)
endif( BUILD_PYTHON_PLUGINS )

foreach(v IN LISTS VISR_LIBRARIES)
	get_property(visr_lib_shared TARGET VISR::${v}_shared PROPERTY LOCATION_RELEASE)
	message( STATUS "VISR shared " ${visr_lib_shared})
	INSTALL( FILES ${visr_lib_shared} COMPONENT visr DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/3rd )
endforeach()

get_filename_component(visr_root_parent ${visr_root} DIRECTORY)
message( STATUS "VISR ROOT PARENT " ${visr_root_parent})

INSTALL( DIRECTORY ${visr_root_parent}/VISR/config/generic/ COMPONENT visr DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/loudspeaker_layouts/generic
	FILES_MATCHING PATTERN "*.xml"
	PATTERN "downmix_matrices" EXCLUDE
    	PATTERN "scripts" EXCLUDE
)

INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/loudspeaker_layouts/5.1_film_order.xml COMPONENT loudspeaker_layouts DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/loudspeaker_layouts )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/loudspeaker_layouts/5.1_SMPTE-ITU_order.xml COMPONENT loudspeaker_layouts DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/loudspeaker_layouts )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/loudspeaker_layouts/Dolby_TrueHD_7.1_alternative.xml COMPONENT loudspeaker_layouts DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/loudspeaker_layouts )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/loudspeaker_layouts/Dolby_TrueHD_7.1_BluRay_default.xml COMPONENT loudspeaker_layouts DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/loudspeaker_layouts )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/loudspeaker_layouts/stereo.xml COMPONENT loudspeaker_layouts DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/loudspeaker_layouts )

if( BUILD_PYTHON_PLUGINS )
	INSTALL( DIRECTORY ${visr_root}/python/Release/ COMPONENT visr DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/python
	FILES_MATCHING PATTERN "*.so" PATTERN "*.pyd"
 )
#	INSTALL( DIRECTORY ${PROJECT_BINARY_DIR}/python COMPONENT visr DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}  )
# TODO: Giacomo - this will not package correctly if (in my case and in many others), people put build folders *WITHIN* the source, as actually is something that is required when building with CI, which is what I did with VISR. i.e. need to remove "/VISR"...
	INSTALL( DIRECTORY ${visr_root_parent}/VISR/src/python/packages/metadapter COMPONENT visr DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/python  )
	INSTALL( DIRECTORY ${visr_root_parent}/VISR/src/python/packages/visr_bst COMPONENT visr DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/python  )
#	INSTALL( DIRECTORY ${CMAKE_SOURCE_DIR}/src/dawplugins/IntelligibilityMeter/python/intelligibility DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/python  )
endif( BUILD_PYTHON_PLUGINS )

######################################################



############ Default plugins resources ######################################################
IF( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "MacOS" )
#boost and the other 3rd party libraries
INSTALL( DIRECTORY ${visr_root}/3rd/ COMPONENT visr DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/3rd
	FILES_MATCHING PATTERN "*.dylib"
	PATTERN "libpython*" EXCLUDE)
######## that would be better:
#if( NOT Boost_USE_STATIC_LIBS )
#   foreach( BOOSTLIB ${VISR_BOOST_LIBRARIES} )
#      get_target_property( BOOSTLIBPATH Boost::${BOOSTLIB} IMPORTED_LOCATION )
#      install( FILES ${BOOSTLIBPATH} DESTINATION 3rd COMPONENT thirdparty_libraries)
#    endforeach()
#  endif( NOT Boost_USE_STATIC_LIBS )
ENDIF( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "MacOS" )

IF( WIN32 )
  SET( VISR_BOOST_LIBRARIES chrono date_time filesystem program_options system thread regex timer atomic )
  get_filename_component( SNDFILE_LIBRARY_DIR ${SNDFILE_LIBRARY} DIRECTORY )
  install( FILES ${SNDFILE_LIBRARY_DIR}/libsndfile-1.dll COMPONENT visr DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/3rd)
#SET( 3RD_LIBRARIES SNDFILE_LIBRARY PORTAUDIO_LIBRARIES )
#foreach(v IN LISTS 3RD_LIBRARIES)
#    INSTALL( FILES ${${v}} COMPONENT visr DESTINATION 3rd )
#endforeach()
# Boost
  if( NOT Boost_USE_STATIC_LIBS )
    foreach( BOOSTLIB ${VISR_BOOST_LIBRARIES} )
      get_target_property( BOOSTLIBPATH Boost::${BOOSTLIB} IMPORTED_LOCATION_RELEASE )
      get_filename_component( BOOSTLIBNAME ${BOOSTLIBPATH} NAME_WE )
      get_filename_component( BOOSTLIBDIR ${BOOSTLIBPATH} DIRECTORY )
      install( FILES ${BOOSTLIBDIR}/${BOOSTLIBNAME}.dll COMPONENT visr DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/3rd)
    endforeach()
  endif( NOT Boost_USE_STATIC_LIBS )
ENDIF( WIN32 )

INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Sessions/REAPER/loudspeaker_renderer/example_project_VST_loudspeakers.RPP COMPONENT examples DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Sessions/REAPER/loudspeaker_renderer )

INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/10obj_session_loudspeaker_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/20obj_session_loudspeaker_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/30obj_session_loudspeaker_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/40obj_session_loudspeaker_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/60obj_session_loudspeaker_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )

INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/10obj_session_no_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/20obj_session_no_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/30obj_session_no_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/40obj_session_no_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/60obj_session_no_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )

INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/10obj_session_loudspeaker_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/20obj_session_loudspeaker_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/30obj_session_loudspeaker_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/40obj_session_loudspeaker_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/60obj_session_loudspeaker_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )

INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/10obj_session_no_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/20obj_session_no_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/30obj_session_no_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/40obj_session_no_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/40obj_session_no_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )

INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/audio_object.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/loudspeaker_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/scene_master.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )

INSTALL( DIRECTORY  ${CMAKE_SOURCE_DIR}/resources/DAW_Sessions/common_material/ COMPONENT examples DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Sessions/common_material
FILES_MATCHING 
PATTERN "*.mp3" 
PATTERN "*.reapeaks" EXCLUDE)
######################################################

############ Python plugins resources ######################################################

if(BUILD_PYTHON_PLUGINS )

IF( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "MacOS" )
 # Prepare an example launchagent file
  set( LAUNCHAGENT_PLIST_FILENAME VISR-Production-Suite.plist )
  set( LAUNCHAGENT_PLIST_FILEPATH ${PROJECT_BINARY_DIR}/resources/launchagents/${LAUNCHAGENT_PLIST_FILENAME} )
  configure_file( ${CMAKE_SOURCE_DIR}/resources/launchagents/VISR-launchagent.plist.in
                  ${LAUNCHAGENT_PLIST_FILEPATH}	@ONLY )
  # Hack: we treat the launchagents file as part of shared_libraries because we want to use the same script.
  install( FILES ${LAUNCHAGENT_PLIST_FILEPATH}
           DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources )
ENDIF( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "MacOS" )

INSTALL( DIRECTORY ${visr_root_parent}/VISR/src/python/packages/visr_bst/data/loudspeaker_configs/ COMPONENT visr DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/loudspeaker_layouts/bbcrd-brirs
FILES_MATCHING PATTERN "*.xml"
)

INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Sessions/REAPER/binaural_renderer/example_project_VST_binaural.RPP COMPONENT examples DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Sessions/REAPER/binaural_renderer )
INSTALL( DIRECTORY ${CMAKE_SOURCE_DIR}/resources/SOFA/koln/ DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/SOFA/koln )

INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/10obj_session_binaural_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/20obj_session_binaural_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/30obj_session_binaural_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/40obj_session_binaural_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/projecet_templates/60obj_session_binaural_renderer.RPP COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/projecet_templates )

INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/10obj_session_binaural_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/20obj_session_binaural_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/30obj_session_binaural_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/40obj_session_binaural_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/60obj_session_binaural_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )
INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Templates/REAPER/track_templates/binaural_renderer.RTrackTemplate COMPONENT templates DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Templates/REAPER/track_templates )

endif( BUILD_PYTHON_PLUGINS )
######################################################

# Install location for the Reverb Json Library
if (BUILD_REVERB)
   INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/RoomIRs.json COMPONENT reverb_resources DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources)
   INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Sessions/REAPER/loudspeaker_renderer/example_project_reverb_VST_loudspeakers.RPP COMPONENT reverb_resources DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Sessions/REAPER/loudspeaker_renderer )
endif( BUILD_REVERB )

if( BUILD_MDO )
	INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/DAW_Sessions/REAPER/MDO_renderer/example_project_VST_MDO.RPP COMPONENT mdo_resources DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/DAW_Sessions/REAPER/MDO_renderer )
	INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/metadataeditor.xml COMPONENT mdo_resources DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources)
	INSTALL( FILES  ${CMAKE_SOURCE_DIR}/resources/loudspeaker_layouts/mdo_production.xml COMPONENT mdo_resources DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources/loudspeaker_layouts )
endif( BUILD_MDO )

if (BUILD_CAP)
   INSTALL( DIRECTORY ${CMAKE_SOURCE_DIR}/resources/CAP COMPONENT cap_resources DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY}/resources  )
endif( BUILD_CAP )

############ Installers configuration ######################################################

configure_file (${CMAKE_SOURCE_DIR}/LICENSE.md
        "${PROJECT_BINARY_DIR}/resources/LICENSE.txt" @ONLY)
set( CPACK_RESOURCE_FILE_LICENSE ${PROJECT_BINARY_DIR}/resources/LICENSE.txt )

configure_file (${CMAKE_SOURCE_DIR}/resources/welcome.txt.in
        "${PROJECT_BINARY_DIR}/resources/welcome.txt" @ONLY)
set(CPACK_RESOURCE_FILE_WELCOME ${PROJECT_BINARY_DIR}/resources/welcome.txt)

configure_file (${CMAKE_SOURCE_DIR}/Readme.md
        "${PROJECT_BINARY_DIR}/resources/Readme.txt" @ONLY)
set( CPACK_RESOURCE_FILE_README ${PROJECT_BINARY_DIR}/resources/Readme.txt )

IF( WIN32 )
SET( CPACK_GENERATOR NSIS)
SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
# only valid in NSIS
# set(CPACK_ALL_INSTALL_TYPES Full Developer)
#INSTALL( DIRECTORY ${visr_root} COMPONENT visr DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY} )
SET( CPACK_PACKAGE_INSTALL_DIRECTORY VISR-Production-Suite )

#TODO: apparently the command doesn't work, even if it's prepended at the beginning of the installation commands in NSIS
SET (CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS  "RMDir \\\"$INSTDIR\\\" \\n")
SET (CPACK_NSIS_EXTRA_INSTALL_COMMANDS  " SetShellVarContext all \\n"
										"Some comment\\n CreateDirectory \\\"C:\\\\Program Files\\\\Steinberg\\\\VstPlugins\\\"  \\n"
										"Some comment\\n CopyFiles \\\"$INSTDIR\\\\dawplugins\\\\vst3\\\\*.vst3\\\" \\\"C:\\\\Program Files\\\\Common Files\\\\VST3\\\" \\n" )
ENDIF( WIN32 )

IF( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "MacOS" )
  #SET( CPACK_GENERATOR PackageMaker )
#  SET( CPACK_GENERATOR productbuild DragNDrop)
   SET( CPACK_GENERATOR productbuild )
   SET( CPACK_PACKAGE_NAME ${PKG_FILE_NAME}.pkg )

##########################################
#set(CPACK_COMPONENTS_ALL vst2 vst3 au aax standalone libraries Unspecified)
#set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)
#set(CPACK_COMPONENT_DAWSUPPORT_GROUP "libraries")

#message( STATUS "VSTPLUG INST " ${PORTAUDIO_LIBRARY_DIR})
get_property(VST2_GROUP_COMPONENTS GLOBAL PROPERTY vst2_group)
foreach(comp ${VST2_GROUP_COMPONENTS})
	set(CPACK_COMPONENT_${comp}_GROUP "VST")
#	message( STATUS "VSTPLUG INST " ${comp})
endforeach(comp ${VST2_GROUP_COMPONENTS})

get_property(VST3_GROUP_COMPONENTS GLOBAL PROPERTY vst3_group)
foreach(comp ${VST3_GROUP_COMPONENTS})
	set(CPACK_COMPONENT_${comp}_GROUP "VST3")
endforeach(comp ${VST3_GROUP_COMPONENTS})

get_property(AU_GROUP_COMPONENTS GLOBAL PROPERTY au_group)
foreach(comp ${AU_GROUP_COMPONENTS})
	set(CPACK_COMPONENT_${comp}_GROUP "AU")
endforeach(comp ${AU_GROUP_COMPONENTS})

get_property(AAX_GROUP_COMPONENTS GLOBAL PROPERTY aax_group)
foreach(comp ${AAX_GROUP_COMPONENTS})
	set(CPACK_COMPONENT_${comp}_GROUP "AAX")
endforeach(comp ${AAX_GROUP_COMPONENTS})

get_property(STANADLONE_GROUP_COMPONENTS GLOBAL PROPERTY standalone_group)
foreach(comp ${STANADLONE_GROUP_COMPONENTS})
	set(CPACK_COMPONENT_${comp}_GROUP "Standalone")
endforeach(comp ${STANADLONE_GROUP_COMPONENTS})

#VISR
#set(CPACK_COMPONENT_VISR_DOWNLOADED "VISR")
#http://cvssp.org/data/s3a/public/VISR/visr-tutorial-code-1.0.0.zip
#INSTALL( DIRECTORY ${visr_root} COMPONENT visr DESTINATION ".." )

#set(CPACK_COMPONENT_VST2_DISPLAY_NAME "VST")
#set(CPACK_COMPONENT_VST3_DISPLAY_NAME "VST3")
#set(CPACK_COMPONENT_AU_DISPLAY_NAME "AU")
#set(CPACK_COMPONENT_AAX_DISPLAY_NAME "AAX")
#set(CPACK_COMPONENT_STANDALONE_DISPLAY_NAME "Standalone")
#set(CPACK_COMPONENT_LOUDSPEAKERLAYOUTS_REQUIRED 1)
#set(CPACK_COMPONENT_PYTHONPACKAGES_REQUIRED 1)

configure_file (${CMAKE_SOURCE_DIR}/resources/scripts/prescript.sh.in
        "${PROJECT_BINARY_DIR}/resources/scripts/prescript.sh" @ONLY)
set(CPACK_PREFLIGHT_UNSPECIFIED_SCRIPT ${PROJECT_BINARY_DIR}/resources/scripts/prescript.sh)

configure_file (${CMAKE_SOURCE_DIR}/resources/scripts/postscript.sh.in
        "${PROJECT_BINARY_DIR}/resources/scripts/postscript.sh" @ONLY)
set(CPACK_POSTFLIGHT_UNSPECIFIED_SCRIPT ${PROJECT_BINARY_DIR}/resources/scripts/postscript.sh)

SET( CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
SET( CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" )
set( CPACK_DMG_VOLUME_NAME ${PKG_FILE_NAME})
#set(CPACK_POSTUPGRADE_SCRIPT "${CMAKE_SOURCE_DIR}/resources/scripts/pkg_rename.sh" ${PROJECT_BINARY_DIR}/${VISR_PRODUCTION_SUITE_TOPLEVEL_NAME}.dmg ${PROJECT_BINARY_DIR}/PKG_FILE_NAME.dmg )
#set(CPACK_POSTUPGRADE_SCRIPT ${CMAKE_SOURCE_DIR}/resources/scripts/pkg_rename.sh)


##############################
#for monolithic:
#set(CPACK_MONOLITHIC_INSTALL 1)
#set(CPACK_POSTFLIGHT_SCRIPT ${CMAKE_SOURCE_DIR}/resources/scripts/temp_script.sh)
#######################
#set(CPACK_POSTFLIGHT_VISR_SCRIPT ${CMAKE_SOURCE_DIR}/resources/scripts/postscript_visr.sh)
#set(CPACK_POSTUPGRADE_SCRIPT "${CMAKE_SOURCE_DIR}/resources/scripts/pkg_rename.sh" ${PROJECT_BINARY_DIR}/${VISR_TOPLEVEL_NAME}.dmg ${PROJECT_BINARY_DIR}/PKG_FILE_NAME.dmg )
  #disabled ZIP and TBZ2
# SET( CPACK_GENERATOR DragNDrop )
  #SET( CPACK_GENERATOR Bundle )
  #SET( CPACK_PACKAGE_INSTALL_DIRECTORY "/Applications" )
 # set( CPACK_PACKAGING_INSTALL_PREFIX "/${VISR_PRODUCTION_SUITE_TOPLEVEL_NAME}")
#  SET( CPACK_DMG_BACKGROUND_IMAGE ${CMAKE_SOURCE_DIR}/resources/s3a_logo.jpg )
#  SET( CPACK_BUNDLE_NAME "VISR-Production-Suite-0.9.0-Darwin/VISR-Production-Suite" )
#  SET( CPACK_BUNDLE_NAME "VISR-Production-Suite" )
#  SET( CPACK_BUNDLE_ICON ${CMAKE_SOURCE_DIR}/resources/s3a_logo.png )
#  SET( CPACK_BUNDLE_PLIST ${CMAKE_SOURCE_DIR}/cmake_modules/Info.plist )
#  SET( CPACK_BUNDLE_STARTUP_COMMAND ${CMAKE_SOURCE_DIR}/resources/scripts/postscript.sh )
   #SET(CPACK_PACKAGE_FILE_NAME VISR-Production-Suite)
   # CPACK_PACKAGE_FILE_NAME - provides the name of the final compressed disk image (the name of the file that is distributed).
   # CPACK_PACKAGE_ICON - provides the icon for the mounted disk image (appears after the user mounts the disk image).
ENDIF( VISR_PRODUCTION_SUITE_SYSTEM_NAME MATCHES "MacOS" )

#INSTALL( DIRECTORY config DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY} )
INSTALL( FILES ${CMAKE_SOURCE_DIR}/LICENSE.md DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY} )

install( FILES ${CMAKE_SOURCE_DIR}/LICENSE.md
               ${CMAKE_SOURCE_DIR}/Readme.md
               ${CMAKE_SOURCE_DIR}/ChangeLog.txt
               DESTINATION ${VISR_PRODUCTION_SUITE_TOPLEVEL_INSTALL_DIRECTORY})
               
INCLUDE( CPack )