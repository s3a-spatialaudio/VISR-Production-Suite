# Copyright Institute of Sound and Vibration Research - All rights reserved

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/currentDefaultPaths.h.in ${CMAKE_BINARY_DIR}/resources/CurrentDefaultPaths.h @ONLY )

SET( GRAPHICS_SOURCES
graphics/ErrorPanel.cpp
graphics/NetSettingsBase.cpp
graphics/NetSettings.cpp
graphics/OpenGLBase.cpp
graphics/Object.cpp
graphics/Scene.cpp
graphics/SceneWithLoudspeakers.cpp
graphics/SceneWithHeadOrientation.cpp
graphics/PluginEditorBase.cpp
graphics/PluginEditorObject.cpp
graphics/PluginEditorLoudspeakers.cpp
graphics/PluginLookAndFeel.cpp
)

SET( GRAPHICS_HEADERS
graphics/ErrorPanel.h
graphics/NetSettingsBase.h
graphics/NetSettings.h
graphics/OpenGLBase.h
graphics/Object.h
graphics/Scene.h
graphics/SceneWithLoudspeakers.h
graphics/SceneWithHeadOrientation.h
graphics/PluginEditorBase.h
graphics/PluginEditorObject.h
graphics/PluginEditorLoudspeakers.h
graphics/PluginLookAndFeel.h
)

SET( PROCESSOR_SOURCES
processors/PluginProcessorLoudspeakers.cpp
processors/PluginProcessorBase.cpp
processors/PluginProcessorScene.cpp
)

SET( PROCESSOR_HEADERS
processors/PluginProcessorBase.h
processors/PluginProcessorScene.h
processors/PluginProcessorLoudspeakers.h
)

SET( NETWORK_SOURCES
network/UDPSender.cpp
network/UDPReceiver.cpp
)

SET( NETWORK_HEADERS
network/UDPSender.h
network/UDPReceiver.h
)

SET( SOURCES
PluginBinaryData.cpp
)

SET( HEADERS
export_symbols.hpp
IDs.h
PluginBinaryData.h
)

source_group( "Source Files\\graphics" FILES ${GRAPHICS_SOURCES} )
source_group( "Source Files\\processors" FILES ${PROCESSOR_SOURCES} )
source_group( "Source Files\\network" FILES ${NETWORK_SOURCES} )

source_group( "Header Files\\graphics" FILES ${GRAPHICS_HEADERS})
source_group( "Header Files\\processors" FILES ${PROCESSOR_HEADERS})
source_group( "Header Files\\network" FILES ${NETWORK_HEADERS})

list(APPEND SOURCES ${GRAPHICS_SOURCES} ${PROCESSOR_SOURCES} ${NETWORK_SOURCES})
list(APPEND HEADERS ${GRAPHICS_HEADERS} ${PROCESSOR_HEADERS} ${NETWORK_HEADERS})

set(JUCE_MODULES_NEEDED juce_core juce_data_structures juce_events juce_graphics juce_gui_basics juce_gui_extra juce_audio_basics juce_audio_processors juce_audio_utils juce_audio_devices juce_opengl)

foreach(dir ${JUCE_MODULES_NEEDED})
	get_filename_component(file_name ${JUCE_SOURCES_${dir}} NAME)
	list(APPEND JUCE_COMPILE_SOURCES ${JUCE_SOURCES_FOLDER}/include_${file_name} ${JUCE_ROOT_DIR}/modules/${dir}/${dir}.h)
	set(JUCE_MODULE_AVAILABLE_${dir} ON)
endforeach(dir ${JUCE_MODULES_NEEDED})


source_group("JUCE Library Code" FILES ${JUCE_COMPILE_SOURCES})

SET(JUCE_HEADERS_FOLDER ${CMAKE_BINARY_DIR}/resources/juce/include/core )
configure_file(
  ${CMAKE_SOURCE_DIR}/resources/juce/JuceHeader_coreLib.h.in 
  ${JUCE_HEADERS_FOLDER}/JuceHeader.h @ONLY
)

configure_file(
  ${CMAKE_SOURCE_DIR}/resources/juce/AppConfig_coreLib.h.in 
  ${JUCE_HEADERS_FOLDER}/AppConfig.h @ONLY
)

#list(APPEND JUCE_HEADERS ${JUCE_HEADERS_FOLDER}/JuceHeader.h )
source_group("JUCE Library Code" FILES ${JUCE_HEADERS})
list(APPEND HEADERS ${JUCE_COMPILE_SOURCES} ${JUCE_HEADERS})

add_library( core STATIC ${SOURCES} ${HEADERS} )

target_compile_definitions(core PRIVATE JUCE_STANDALONE_APPLICATION)
target_include_directories(core PRIVATE ${JUCE_ROOT_DIR}/modules)
target_include_directories(core PRIVATE ${JUCE_HEADERS_FOLDER} )
target_include_directories(core PUBLIC ${CMAKE_BINARY_DIR}/resources)

#install(TARGETS core ARCHIVE DESTINATION lib COMPONENT libraries )
#target_link_libraries(core PRIVATE JuceLibraryCode)

target_link_libraries(core PUBLIC VISR::panning_shared)