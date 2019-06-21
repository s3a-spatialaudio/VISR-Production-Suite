include( JUCE )

get_filename_component(CurrLib ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set( ${CurrLib}_SOURCES
 PluginEditor.cpp
 PluginProcessor.cpp
 Settings.cpp
)

set( ${CurrLib}_HEADERS
 PluginEditor.h
 PluginProcessor.h
 Settings.h
)

set( ${CurrLib}_SOURCES_COMPONENTS
 visr_components/top_level_component_no_net.cpp
 visr_components/top_level_component.cpp
 visr_components/binaural_renderer_component.cpp
 visr_components/tracker_component.cpp
)

set( ${CurrLib}_HEADERS_COMPONENTS
 visr_components/top_level_component_no_net.hpp
 visr_components/top_level_component.hpp
 visr_components/binaural_renderer_component.hpp
 visr_components/tracker_component.hpp
)

source_group( "Source Files\\visr_components" FILES ${${CurrLib}_SOURCES_COMPONENTS})
source_group( "Header Files\\visr_components" FILES ${${CurrLib}_HEADERS_COMPONENTS})

list(APPEND ${CurrLib}_SOURCES ${${CurrLib}_SOURCES_COMPONENTS} )
list(APPEND ${CurrLib}_HEADERS ${${CurrLib}_HEADERS_COMPONENTS} )

#--------------------------------------------------------------------------------------------------
set(JUCE_MODULES_NEEDED juce_core juce_data_structures juce_events juce_graphics juce_gui_basics juce_audio_basics juce_audio_processors juce_audio_utils juce_audio_devices )
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
#target_include_directories(${CurrLib} PRIVATE ${CMAKE_BINARY_DIR}/resources/juce/include/${CurrLib} ) 
#message( STATUS "VSTPLUG INST0 " ${CurrLib} ${CMAKE_BINARY_DIR}/resources/juce/include/${CurrLib} ) 

target_link_libraries(${CurrLib} PRIVATE VISR::pythonsupport_shared )
target_link_libraries(${CurrLib} PRIVATE VISR::pythoncomponents_shared )
target_link_libraries(${CurrLib} PRIVATE Boost::filesystem )

target_link_libraries(${CurrLib} PRIVATE visrsupport)
target_link_libraries(${CurrLib} PRIVATE core )

target_include_directories(${CurrLib} PUBLIC ${JUCE_ROOT_DIR}/modules)
target_include_directories(${CurrLib} PUBLIC ${JUCE_HEADERS_FOLDER})

#--------------------------------------------------------------------------------------------------
addJUCEPlugins( ${CurrLib} Bire )
#source_group("JUCE Library Code" FILES ${CMAKE_BINARY_DIR}/resources/juce/include/${CurrLib}/AppConfig.h)
#--------------------------------------------------------------------------------------------------