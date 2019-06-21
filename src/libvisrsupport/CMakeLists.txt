    # Copyright Institute of Sound and Vibration Research - All rights reserved
SET( SOURCES
VISRComponentWrapper.cpp
VISRComponentWrapperJUCEParam.cpp
VISRComponentWrapperObjVector.cpp
VISRComponentWrapperMulti.cpp
VISRObjectWrapper.cpp
VISRPointSourceWrapper.cpp
VISRPointSourceExtWrapper.cpp
VISRPointSourceRevWrapper.cpp
VISRObjectVectorWrapper.cpp
VISRListenerPosWrapper.cpp
)

SET( HEADERS
VISRComponentWrapper.h
VISRComponentWrapperJUCEParam.h
VISRComponentWrapperObjVector.h
VISRComponentWrapperMulti.h
VISRObjectWrapper.h
VISRPointSourceWrapper.h
VISRPointSourceExtWrapper.h
VISRPointSourceRevWrapper.h
VISRObjectVectorWrapper.h
VISRListenerPosWrapper.h
VISRObjectVectorObserver.h
)

SET( SOURCES_COMPONENTS
visr_components/MetadataExposingRenderer.cpp
visr_components/ObjectRenderer.cpp
visr_components/ObjectVectorEncoder.cpp
visr_components/DualbandCapRenderer.cpp
visr_components/listener_compensation_CAP.cpp
)


SET( HEADERS_COMPONENTS
visr_components/MetadataExposingRenderer.hpp
visr_components/ObjectRenderer.hpp
visr_components/ObjectVectorEncoder.hpp
visr_components/DualbandCapRenderer.hpp
visr_components/listener_compensation_CAP.hpp
)

set(LocalDir ${CMAKE_SOURCE_DIR}/src/libdawsupport)

source_group( "Source Files\\visr_components" FILES ${SOURCES_COMPONENTS})
source_group( "Header Files\\visr_components" FILES ${HEADERS_COMPONENTS})

SET(JUCE_HEADERS_FOLDER ${CMAKE_BINARY_DIR}/resources/juce/include/visrsupport )
configure_file(
  ${CMAKE_SOURCE_DIR}/resources/juce/JuceHeader_coreLib.h.in 
  ${JUCE_HEADERS_FOLDER}/JuceHeader.h @ONLY
)

list(APPEND JUCE_HEADERS ${JUCE_HEADERS_FOLDER}/JuceHeader.h )
source_group("JUCE Library Code" FILES ${JUCE_HEADERS})
list(APPEND HEADERS ${JUCE_HEADERS})

add_library( visrsupport STATIC ${SOURCES} ${SOURCES_COMPONENTS} ${HEADERS} ${HEADERS_COMPONENTS})

target_compile_definitions(visrsupport PRIVATE JUCE_STANDALONE_APPLICATION)
target_include_directories(visrsupport PRIVATE ${JUCE_ROOT_DIR}/modules)
target_include_directories(visrsupport PRIVATE ${JUCE_HEADERS_FOLDER} )
#target_link_libraries(visrsupport PRIVATE JuceLibraryCode)

target_link_libraries(visrsupport PUBLIC VISR::pml_shared)
target_link_libraries(visrsupport PUBLIC VISR::rrl_shared)
target_link_libraries(visrsupport PRIVATE Boost::boost)

#target_link_libraries(visrsupport )
#target_include_directories(visrsupport PUBLIC "/Users/giacomocostantini/VISR/visr/src")

#set_target_properties( visrsupport PROPERTIES INSTALL_RPATH "/Library/Application\ Support/ObjectPanner/lib/")
#install(TARGETS visrsupport ARCHIVE DESTINATION lib COMPONENT libraries)
