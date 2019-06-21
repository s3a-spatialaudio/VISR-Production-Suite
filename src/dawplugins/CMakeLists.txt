set_property(GLOBAL PROPERTY vst2_group)
set_property(GLOBAL PROPERTY vst3_group)
set_property(GLOBAL PROPERTY au_group)
set_property(GLOBAL PROPERTY aax_group)
set_property(GLOBAL PROPERTY standalone_group)

################################################

add_subdirectory( ObjectEditor )
add_subdirectory( SceneMaster )
add_subdirectory( LoudspeakerRenderer )


if( BUILD_PYTHON_PLUGINS )
  add_subdirectory( BinauralRenderer )
endif( BUILD_PYTHON_PLUGINS )

if( BUILD_REVERB )
add_subdirectory( ReverbObjectEditor )
endif (BUILD_REVERB)

if( BUILD_CAP )
add_subdirectory( CAPRenderer )
endif (BUILD_CAP)

if( BUILD_MDO )
add_subdirectory( MetadataEditor )
if( BUILD_PYTHON_PLUGINS )
  add_subdirectory( MetadapterRenderer )
endif( BUILD_PYTHON_PLUGINS )

endif( BUILD_MDO )

################################################

if( NOT BUILD_ECOSYSTEM_ONLY )
if( BUILD_PYTHON_PLUGINS )
  add_subdirectory( MultiRenderer )
endif( BUILD_PYTHON_PLUGINS )

endif( NOT BUILD_ECOSYSTEM_ONLY )

################################################
