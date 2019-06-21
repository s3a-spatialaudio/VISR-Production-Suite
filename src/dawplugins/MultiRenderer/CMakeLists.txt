include( JUCE )
get_filename_component(CurrLib ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# get all source files
set(LocalDir ${CMAKE_SOURCE_DIR}/src/dawplugins/${CurrLib})

file(GLOB ${CurrLib}_SOURCES RELATIVE ${LocalDir} 
				*.cpp)
file(GLOB ${CurrLib}_HEADERS RELATIVE ${LocalDir} 
				*.h;*.hpp)
file(GLOB ${CurrLib}_SOURCES_COMPONENTS RELATIVE ${LocalDir}
				visr_components/*.cpp)
file(GLOB ${CurrLib}_HEADERS_COMPONENTS RELATIVE ${LocalDir}
				visr_components/*.h;visr_components/*.hpp)

source_group( "Source Files\\visr_components" FILES ${${CurrLib}_SOURCES_COMPONENTS})
source_group( "Header Files\\visr_components" FILES ${${CurrLib}_HEADERS_COMPONENTS})

list(APPEND ${CurrLib}_SOURCES ${${CurrLib}_SOURCES_COMPONENTS} )
list(APPEND ${CurrLib}_HEADERS ${${CurrLib}_HEADERS_COMPONENTS} )

if (BUILD_DAWPLUGINS_VST3 OR BUILD_DAWPLUGINS_VST2 OR BUILD_DAWPLUGINS_AU OR BUILD_DAWPLUGINS_AAX)

SET(JUCE_CONFIG_FOLDER_PLUGINS ${CMAKE_BINARY_DIR}/include/${CurrLib}/JuceConfigPlugins )
include_directories(${JUCE_CONFIG_FOLDER_PLUGINS})	

set( ALL_SOURCES ${${CurrLib}_SOURCES} ${${CurrLib}_HEADERS})
add_library(${CurrLib} STATIC ${ALL_SOURCES} )

target_link_libraries(${CurrLib} PUBLIC visrsupport)  
target_link_libraries(${CurrLib} PUBLIC core)
target_link_libraries(${CurrLib} PUBLIC VISR::signalflows_shared)
target_link_libraries(${CurrLib} PUBLIC VISR::pythoncomponents_shared )
target_link_libraries(${CurrLib} PUBLIC VISR::pythonsupport_shared )

#set_target_properties( ${CurrLib} PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_RPATH}")

addJUCEPlugins( ${CurrLib} Scmr )
endif (BUILD_DAWPLUGINS_VST3 OR BUILD_DAWPLUGINS_VST2 OR BUILD_DAWPLUGINS_AU OR BUILD_DAWPLUGINS_AAX)
