include( JUCE )
get_filename_component(CurrLib ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set( ${CurrLib}_SOURCES
 PluginEditor.cpp
 PluginProcessor.cpp
 MetadataTable.cpp
)

set( ${CurrLib}_HEADERS
 PluginEditor.h
 PluginProcessor.h
 MetadataTable.h
)

#--------------------------------------------------------------------------------------------------
set(JUCE_MODULES_NEEDED juce_core juce_data_structures juce_events juce_graphics juce_gui_basics juce_gui_extra juce_audio_basics juce_audio_processors juce_audio_utils juce_audio_devices)
SET(JUCE_HEADERS_FOLDER ${CMAKE_BINARY_DIR}/resources/juce/headers/${CurrLib})	

foreach(dir ${JUCE_MODULES_NEEDED})
	get_filename_component(file_name ${JUCE_SOURCES_${dir}} NAME)
#	list(APPEND JUCE_COMPILE_SOURCES ${JUCE_SOURCES_FOLDER}/include_${file_name} ${JUCE_ROOT_DIR}/modules/${dir}/${dir}.h)
	set(JUCE_MODULE_AVAILABLE_${dir} ON)
endforeach(dir ${JUCE_MODULES_NEEDED})

configure_file( ${CMAKE_SOURCE_DIR}/resources/juce/JuceHeader.h.in ${JUCE_HEADERS_FOLDER}/JuceHeader.h @ONLY )
list(APPEND JUCE_COMPILE_SOURCES ${JUCE_HEADERS_FOLDER}/JuceHeader.h )

#configure_file(${CMAKE_SOURCE_DIR}/resources/juce/AppConfig.h.in ${JUCE_HEADERS_FOLDER}/AppConfig.h @ONLY)
#list(APPEND JUCE_COMPILE_SOURCES ${JUCE_HEADERS_FOLDER}/AppConfig.h )

source_group("JUCE Library Code" FILES ${JUCE_COMPILE_SOURCES})
list(APPEND ${CurrLib}_HEADERS ${JUCE_COMPILE_SOURCES})

#--------------------------------------------------------------------------------------------------

add_library(${CurrLib} STATIC ${${CurrLib}_SOURCES} ${${CurrLib}_HEADERS} )

target_link_libraries(${CurrLib} PUBLIC visrsupport)  
target_link_libraries(${CurrLib} PUBLIC core)
target_link_libraries(${CurrLib} PUBLIC VISR::rcl_shared)

target_include_directories(${CurrLib} PUBLIC ${JUCE_ROOT_DIR}/modules)
target_include_directories(${CurrLib} PUBLIC ${JUCE_HEADERS_FOLDER})

#--------------------------------------------------------------------------------------------------
addJUCEPlugins( ${CurrLib} Scma)
#--------------------------------------------------------------------------------------------------