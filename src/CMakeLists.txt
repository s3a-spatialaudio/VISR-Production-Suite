# Copyright Institute of Sound and Vibration Research - All rights reserved

# Build configuration for the top-level src/ directory

#DAW plugins support
ADD_SUBDIRECTORY( libvisrsupport )
ADD_SUBDIRECTORY( libcore )

#DAW plugins
if (BUILD_DAWPLUGINS_VST2 OR BUILD_DAWPLUGINS_VST3 OR BUILD_DAWPLUGINS_AU OR BUILD_DAWPLUGINS_AAX)
ADD_SUBDIRECTORY( dawplugins )
endif (BUILD_DAWPLUGINS_VST2 OR BUILD_DAWPLUGINS_VST3 OR BUILD_DAWPLUGINS_AU OR BUILD_DAWPLUGINS_AAX)