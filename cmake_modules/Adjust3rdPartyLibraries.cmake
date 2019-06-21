# Copyright Institute of Sound and Vibration Research - All rights reserved

# 3rd party library fixing required on Mac OS X


function(copy_lib libpath dest )
  get_filename_component( libname ${${libpath}} NAME )
  set (EXECUTE_COMMAND  cp ${${libpath}} ${PROJECT_BINARY_DIR}/${dest}/${libname})
  execute_process(COMMAND ${EXECUTE_COMMAND} RESULT_VARIABLE rv)

  set( ${libpath} ${PROJECT_BINARY_DIR}/${dest}/${libname})
  set( ${libpath} ${PROJECT_BINARY_DIR}/${dest}/${libname} PARENT_SCOPE )

  set (EXECUTE_COMMAND  chmod 777 ${PROJECT_BINARY_DIR}/${dest}/${libname})
  execute_process(COMMAND ${EXECUTE_COMMAND} RESULT_VARIABLE rv)
endfunction()


function(fix_rpath libpath dest )
  get_filename_component( libname ${${libpath}} NAME )
  set( ${libpath} ${PROJECT_BINARY_DIR}/${dest}/${libname})
  set( ${libpath} ${PROJECT_BINARY_DIR}/${dest}/${libname} PARENT_SCOPE )

  set (EXECUTE_COMMAND install_name_tool -id @rpath/${libname} ${${libpath}})
  #  message( STATUS " output " ${libname} ": " ${${libpath}} )
  execute_process(COMMAND ${EXECUTE_COMMAND} RESULT_VARIABLE rv)
endfunction()

################################################################################


file( MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/3rd )

################################################################################
# Do the same for the boost libs.

function( fixBoostLibrary BOOSTLIB )
  get_target_property( LIBLOCATION Boost::${BOOSTLIB} IMPORTED_LOCATION)
  get_filename_component( LIBREALPATH ${LIBLOCATION} REALPATH)
  copy_lib( LIBREALPATH 3rd )
  fix_rpath( LIBREALPATH 3rd )
  get_filename_component( LIBNAME ${LIBREALPATH} NAME )
  set( ADJUSTEDLIBPATH ${PROJECT_BINARY_DIR}/3rd/${LIBNAME})
  set_target_properties( Boost::${BOOSTLIB} PROPERTIES
                         IMPORTED_LOCATION ${ADJUSTEDLIBPATH}
			 IMPORTED_LOCATION_DEBUG ${ADJUSTEDLIBPATH}
			 IMPORTED_LOCATION_RELEASE ${ADJUSTEDLIBPATH})
endfunction()

foreach( BOOSTLIB ${VISR_PRODUCTION_SUITE_BOOST_LIBRARIES} )
  fixBoostLibrary( ${BOOSTLIB} )
endforeach()
