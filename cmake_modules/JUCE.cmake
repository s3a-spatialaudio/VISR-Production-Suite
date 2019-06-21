set(SUBMODULES_DIR ${CMAKE_SOURCE_DIR}/modules)
set(JUCE_LIBRARY_CODE_DIR ${CMAKE_SOURCE_DIR}/support/JuceLibraryCode/)

# --------------------------------------------------------------------------------------------------

function( addJUCE target_ )

  if(APPLE)
    find_package( OpenGL REQUIRED )
  endif()

  set_target_properties(${CURRENT_TARGET} PROPERTIES
    COMPILE_DEFINITIONS         NDEBUG
    COMPILE_DEFINITIONS_DEBUG   DEBUG
    COMPILE_DEFINITIONS_RELEASE NDEBUG
  )

  if( APPLE )

    set_target_properties( ${CURRENT_TARGET} PROPERTIES
      COMPILE_FLAGS "${COMPILE_FLAGS} -x objective-c++"
    )
    target_link_libraries( ${CURRENT_TARGET} PRIVATE ${OPENGL_gl_LIBRARY} )
    target_link_libraries( ${CURRENT_TARGET} PUBLIC
      "-framework AudioUnit"
      "-framework AudioToolbox"
      "-framework Carbon"
      "-framework Cocoa"
      "-framework CoreFoundation"
      "-framework CoreAudio"
      "-framework CoreMidi"
      "-framework QuartzCore"
      "-framework IOKit"
      "-framework AGL"
      "-framework Accelerate"
      "-framework WebKit"
      "-lobjc"
    )

  elseif(WIN32)

    target_link_libraries(${CURRENT_TARGET} PUBLIC
      advapi32.lib
      comdlg32.lib
      gdi32.lib
      GlU32.Lib
      kernel32.lib
      ole32.lib
      OpenGL32.Lib
      rpcrt4.lib
      shell32.lib
      user32.lib
      vfw32.lib
      wininet.lib
      winmm.lib
      ws2_32.lib
    )

  endif()

endfunction(addJUCE)

# --------------------------------------------------------------------------------------------------

function( addJUCE_VST2 name_)
	set( CurrLib "${name_}VST" )
	set(JUCE_MODULE_AVAILABLE_juce_audio_plugin_client ON)

	get_filename_component(wrapper_module_name ${JUCE_PLUGIN_WRAPPER_DIR} NAME)
	list(APPEND VST_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_utils.cpp)

	list(APPEND VST_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_VST2.cpp)			

	if (APPLE)
		list(APPEND VST_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_VST_utils.mm)
	endif (APPLE)

	source_group( "JUCE\\vst" FILES ${VST_SOURCES} )
	set(JUCE_PLUGIN_SOURCES ${VST_SOURCES}  )
	
    add_library(${CurrLib} MODULE ${JUCE_PLUGIN_SOURCES})

	target_include_directories( ${CurrLib} PRIVATE ${VST_INCLUDE_DIR})
	target_compile_definitions( ${CurrLib} PRIVATE JucePlugin_Build_VST=1 )
#	configure_file(${CMAKE_SOURCE_DIR}/resources/juce/AppConfig.h.in ${JUCE_HEADERS_FOLDER}/AppConfig.h @ONLY)	

	#[GC] OUTPUT_NAME determines both the name of the .vst file AND the binary inside the bundle. Therefore this value must match the CFBundleExecutable property in the corresponding Info.plist.in file
	if (APPLE)
		configure_file (
			"${CMAKE_SOURCE_DIR}/resources/juce/Info.plist.in"
			"${JUCE_INFO_FOLDER}/Info-VST.plist"
		)

        set_target_properties(${CurrLib} PROPERTIES
		        BUNDLE TRUE
			OUTPUT_NAME "VISR ${name_}"
			BUNDLE_EXTENSION "vst"
			XCODE_ATTRIBUTE_WRAPPER_EXTENSION "vst" 
			XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE "YES"
			MACOSX_BUNDLE_INFO_PLIST "${JUCE_INFO_FOLDER}/Info-VST.plist"
			)     
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/res)
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/res)
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/res)
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/res)
	else(APPLE)
		set_target_properties(${CurrLib} PROPERTIES OUTPUT_NAME "VISR ${name_}")
	endif(APPLE)

#	set_target_properties(${CurrLib} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	
	if(BUILD_PLUGINS_IN_BUILD_FOLDER)
		set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/dawplugins/vst" )
	else(BUILD_PLUGINS_IN_BUILD_FOLDER)
		if(APPLE)
    		set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "~/Library/Audio/Plug-Ins/VST/VISR-Production-Suite")
    		set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "~/Library/Audio/Plug-Ins/VST/VISR-Production-Suite")
    	else(APPLE)
    		set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/dawplugins/vst" )
    	# TRY THIS ON WINDOWS: set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "C:\Program Files\Common Files\VST3" )
		endif(APPLE)
	endif(BUILD_PLUGINS_IN_BUILD_FOLDER)
	
	target_link_libraries(${CurrLib} ${name_})

    install(TARGETS ${CurrLib} COMPONENT ${CurrLib} LIBRARY DESTINATION dawplugins/vst BUNDLE DESTINATION dawplugins/vst)
	
##############COMPONENTS##################
	string(TOUPPER ${CurrLib} CURRLIB_UPPER)
	get_property(VST2_GROUP_COMPONENTS GLOBAL PROPERTY vst2_group)
	LIST( APPEND VST2_GROUP_COMPONENTS ${CURRLIB_UPPER})
	set_property(GLOBAL PROPERTY vst2_group "${VST2_GROUP_COMPONENTS}")

	#COMPONENT ${CurrLib}
	#set(COMPONENT_CURRLIB_GROUP "CPACK_COMPONENT_${CurrLib}_GROUP")	
	#set(CPACK_COMPONENT_BINAURALRENDERERVST_GROUP "vst2")
	#set(${COMPONENT_CURRLIB_GROUP} "VST2")

#	message( STATUS "VSTPLUG INST " ${VST_GROUP_COMPONENTS}) 
###############COMPONENTS##################
endfunction( addJUCE_VST2 )

# --------------------------------------------------------------------------------------------------
function( addJUCE_VST3 name_)

	set( CurrLib "${name_}VST3" )
	set(JUCE_MODULE_AVAILABLE_juce_audio_plugin_client ON)
		
	get_filename_component(wrapper_module_name ${JUCE_PLUGIN_WRAPPER_DIR} NAME)
#	file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_utils.cpp "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_utils.cpp>")
	list(APPEND VST_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_utils.cpp)

	
#	file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_VST3.cpp "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_VST3.cpp>")
	list(APPEND VST_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_VST3.cpp)

	if (APPLE)
#		file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_VST_utils.mm "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_VST_utils.mm>")
		list(APPEND VST_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_VST_utils.mm)
	endif (APPLE)

	source_group( "JUCE\\vst" FILES ${VST_SOURCES} )
	set(JUCE_PLUGIN_SOURCES ${VST_SOURCES}  )
	
	add_library(${CurrLib} MODULE ${JUCE_PLUGIN_SOURCES})
		
	target_include_directories( ${CurrLib} PRIVATE ${VST3_INCLUDE_DIR})
	
	target_compile_definitions( ${CurrLib} PRIVATE JucePlugin_Build_VST3=1 )
	target_compile_definitions( ${CurrLib} PRIVATE JUCE_VST3_CAN_REPLACE_VST2=0)
	target_compile_definitions( ${CurrLib} PRIVATE JUCE_VST3HEADERS_INCLUDE_HEADERS_ONLY=0)
#	configure_file(${CMAKE_SOURCE_DIR}/resources/juce/AppConfig.h.in ${JUCE_HEADERS_FOLDER}/AppConfig.h @ONLY)	

	if (APPLE)
		configure_file (
			"${CMAKE_SOURCE_DIR}/resources/juce/Info.plist.in"
			"${JUCE_INFO_FOLDER}/Info-VST3.plist"
		)
 
        #set(PluginInfoPlist ${JUCE_INFO_FOLDER}/Info.plist)

		set_target_properties(${CurrLib} PROPERTIES
		        BUNDLE TRUE
	 	    	OUTPUT_NAME "VISR ${name_}"
			BUNDLE_EXTENSION "vst3"
			XCODE_ATTRIBUTE_WRAPPER_EXTENSION "vst3"
			XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE "YES"
			MACOSX_BUNDLE_INFO_PLIST "${JUCE_INFO_FOLDER}/Info-VST3.plist"
			)     
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/res)
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/res)
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/res)
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/res)
	else(APPLE)
		set_target_properties(${CurrLib} PROPERTIES
		    OUTPUT_NAME "VISR ${name_}"
			SUFFIX ".vst3"
		)
	endif(APPLE)

	set_target_properties(${CurrLib} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	
	
	if(BUILD_PLUGINS_IN_BUILD_FOLDER)
		set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/dawplugins/vst3" )
	else(BUILD_PLUGINS_IN_BUILD_FOLDER)
		if(APPLE)
	    	set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "~/Library/Audio/Plug-Ins/VST3/VISR-Production-Suite")
		    set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "~/Library/Audio/Plug-Ins/VST3/VISR-Production-Suite")
    	else(APPLE)
    		set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/dawplugins/vst3" )
    	# TRY THIS ON WINDOWS: set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "C:\Program Files\Common Files\VST3" )
		endif(APPLE)
	endif(BUILD_PLUGINS_IN_BUILD_FOLDER)
	
		#alternatives
 		# add_custom_command(TARGET ${CurrLib} POST_BUILD 
    	#	COMMAND cp -a "$<TARGET_BUNDLE_DIR:${CurrLib}>" "/Library/Audio/Plug-Ins/VST3/VISR-Production-Suite"
		# )
		
		#or
		#add_custom_command(TARGET ${CurrLib} POST_BUILD 
    	#	COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_BUNDLE_DIR:${CurrLib}> "/Library/Audio/Plug-Ins/VST3/VISR-Production-Suite/VISR ${name_}.vst3"
		#)
		
		#or
	
	target_link_libraries(${CurrLib} ${name_})

    #install(TARGETS ${CurrLib} PERMISSIONS WORLD_WRITE COMPONENT ${CurrLib} LIBRARY DESTINATION dawplugins/vst3 BUNDLE DESTINATION dawplugins/vst3)
	install(TARGETS ${CurrLib} COMPONENT ${CurrLib} LIBRARY DESTINATION dawplugins/vst3 BUNDLE DESTINATION dawplugins/vst3 )
	string(TOUPPER ${CurrLib} CURRLIB_UPPER)
	get_property(VST3_GROUP_COMPONENTS GLOBAL PROPERTY vst3_group)
	LIST( APPEND VST3_GROUP_COMPONENTS ${CURRLIB_UPPER})
	set_property(GLOBAL PROPERTY vst3_group "${VST3_GROUP_COMPONENTS}")

	target_compile_definitions( ${CurrLib} PRIVATE JucePlugin_Build_VST3=1)

endfunction( addJUCE_VST3 )

# --------------------------------------------------------------------------------------------------

function( addJUCE_AAX name_ )

	set( CurrLib "${name_}AAX" )

	set(JUCE_MODULE_AVAILABLE_juce_audio_plugin_client ON)
		
	get_filename_component(wrapper_module_name ${JUCE_PLUGIN_WRAPPER_DIR} NAME)
#	file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_utils.cpp "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_utils.cpp>")
	list(APPEND AAX_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_utils.cpp)
	
	if (APPLE)
#		file(WRITE ${JUCE_SOURCES_FOLDER}/juce_audio_plugin_client_AAX.mm "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_AAX.mm>")
		list(APPEND AAX_SOURCES ${JUCE_SOURCES_FOLDER}/juce_audio_plugin_client_AAX.mm)
	else (APPLE)
#		file(WRITE ${JUCE_SOURCES_FOLDER}/juce_audio_plugin_client_AAX.cpp "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_AAX.cpp>")
		list(APPEND AAX_SOURCES ${JUCE_SOURCES_FOLDER}/juce_audio_plugin_client_AAX.cpp)
	endif (APPLE)

	set(JUCE_PLUGIN_SOURCES ${AAX_SOURCES} )

	source_group( "JUCE\\aax" FILES ${AAX_SOURCES} )

	add_library(${CurrLib} MODULE ${JUCE_PLUGIN_SOURCES} )
	
	target_include_directories( ${CurrLib} PRIVATE ${AAX_INCLUDE_DIR}/Interfaces ${AAX_INCLUDE_DIR}/Interfaces/ACF)

	if (APPLE)
        
		configure_file (
			"${CMAKE_SOURCE_DIR}/resources/juce/InfoAAX.plist.in"
			"${JUCE_INFO_FOLDER}/Info-AAX.plist"
		)
# 		SET(MACOSX_BUNDLE_ICON_FILE QRoboMetImporter.icns)

        #set(PluginInfoPlist ${JUCE_INFO_FOLDER}/Info.plist)
		
		set_target_properties(${CurrLib} PROPERTIES
		        BUNDLE TRUE
	 	    	OUTPUT_NAME "VISR ${name_}"
			BUNDLE_EXTENSION "aaxplugin"
			XCODE_ATTRIBUTE_WRAPPER_EXTENSION "aaxplugin"			
			XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE "YES"
			MACOSX_BUNDLE_INFO_PLIST "${JUCE_INFO_FOLDER}/Info-AAX.plist"
			)     

		#SET_SOURCE_FILES_PROPERTIES("${CMAKE_SOURCE_DIR}/resources/juce/PkgInfo"
                 #              PROPERTIES
                  #             MACOSX_PACKAGE_LOCATION Resources)

		
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/res)
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/res)
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/res)
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/res)
	
		target_link_libraries(${CurrLib} debug ${AAX_INCLUDE_DIR}/Libs/Debug/libAAXLibrary_libcpp.a)
		target_link_libraries(${CurrLib} optimized ${AAX_INCLUDE_DIR}/Libs/Release/libAAXLibrary_libcpp.a)
	else(APPLE)
		set_target_properties(${CurrLib} PROPERTIES OUTPUT_NAME "VISR ${name_}")
		target_link_libraries(${CurrLib} debug ${AAX_INCLUDE_DIR}/Libs/Debug/AAXLibrary_x64_D.lib)
		target_link_libraries(${CurrLib} optimized ${AAX_INCLUDE_DIR}/Libs/Release/AAXLibrary_x64.lib)
	endif(APPLE)

	set_target_properties(${CurrLib} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/dawplugins/aax" )

	target_link_libraries(${CurrLib} ${name_})

	install(TARGETS ${CurrLib} COMPONENT ${CurrLib} LIBRARY DESTINATION dawplugins/aax BUNDLE DESTINATION dawplugins/aax)    

	string(TOUPPER ${CurrLib} CURRLIB_UPPER)
	get_property(AAX_GROUP_COMPONENTS GLOBAL PROPERTY aax_group)
	LIST( APPEND AAX_GROUP_COMPONENTS ${CURRLIB_UPPER})
	set_property(GLOBAL PROPERTY aax_group "${AAX_GROUP_COMPONENTS}")

	target_compile_definitions( ${CurrLib} PRIVATE JucePlugin_Build_AAX=1)
	target_compile_definitions( ${CurrLib} PRIVATE JucePlugin_AAXLibs_path=${AAX_INCLUDE_DIR}/Libs)

#  source_group( "JUCE\\audio\\vst" FILES ${juce_library_audio_plugin_client_vst} )

#  if(APPLE)
#    install( TARGETS ${CurrLib} DESTINATION "/Library/Application Support/Avid/Audio/Plug-Ins" )
#  else(APPLE)
#        install( TARGETS ${CurrLib} DESTINATION dawplugins/vst )
#  endif(APPLE)



#  source_group( "JUCE\\audio\\vst" FILES ${juce_library_audio_plugin_client_vst} )
#  source_group( "JUCE\\audio\\vst3" FILES ${juce_library_audio_plugin_client_vst3} )



endfunction( addJUCE_AAX )
# --------------------------------------------------------------------------------------------------


function( addJUCE_AU name_)
  if(APPLE)
    set( CurrLib "${name_}AU" )
    set(JUCE_MODULE_AVAILABLE_juce_audio_plugin_client ON)
		
    get_filename_component(wrapper_module_name ${JUCE_PLUGIN_WRAPPER_DIR} NAME)
#    file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_utils.cpp "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_utils.cpp>")
    list(APPEND AU_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_utils.cpp)

#    file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_AU_1.mm "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_AU_1.mm>")
    list(APPEND AU_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_AU_1.mm)

#    file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_AU_2.mm "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_AU_2.mm>")
    list(APPEND AU_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_AU_2.mm)


#    file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_VST_utils.mm "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/#juce_audio_plugin_client_VST_utils.mm>")   
#    list(APPEND AU_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_VST_utils.mm)

    set(JUCE_PLUGIN_SOURCES ${AU_SOURCES}  )

    source_group( "JUCE\\au" FILES ${AU_SOURCES} )
    add_library(${CurrLib} MODULE ${JUCE_PLUGIN_SOURCES})

		configure_file (
			"${CMAKE_SOURCE_DIR}/resources/juce/InfoAU.plist.in"
			"${JUCE_INFO_FOLDER}/Info-AU.plist"
		)
 
        #set(PluginInfoPlist ${JUCE_INFO_FOLDER}/Info.plist)

		set_target_properties(${CurrLib} PROPERTIES
		        BUNDLE TRUE
	 	    	OUTPUT_NAME "VISR ${name_}"
			BUNDLE_EXTENSION "component"
			XCODE_ATTRIBUTE_WRAPPER_EXTENSION "component"
			XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE "YES"
			MACOSX_BUNDLE_INFO_PLIST "${JUCE_INFO_FOLDER}/Info-AU.plist"
			)     

	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/res)
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/res)
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/res)
	#	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/res)

	set_target_properties(${CurrLib} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
   	
   	
   	if(BUILD_PLUGINS_IN_BUILD_FOLDER)
		set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/dawplugins/au" )
	else(BUILD_PLUGINS_IN_BUILD_FOLDER)
			set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "~/Library/Audio/Plug-Ins/Components")
    		set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "~/Library/Audio/Plug-Ins/Components")
    	# TRY THIS ON WINDOWS: set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "C:\Program Files\Common Files\VST3" )
	endif(BUILD_PLUGINS_IN_BUILD_FOLDER)
   	
   	
	if(APPLE)
    	
    else(APPLE)
    # TRY THIS ON WINDOWS: set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "C:\Program Files\Common Files\VST3" )
	endif(APPLE)
	
	target_link_libraries(${CurrLib} ${name_})

	install(TARGETS ${CurrLib} COMPONENT ${CurrLib} LIBRARY DESTINATION dawplugins/au BUNDLE DESTINATION dawplugins/au)    


	string(TOUPPER ${CurrLib} CURRLIB_UPPER)
	get_property(AU_GROUP_COMPONENTS GLOBAL PROPERTY au_group)
	LIST( APPEND AU_GROUP_COMPONENTS ${CURRLIB_UPPER})
	set_property(GLOBAL PROPERTY au_group "${AU_GROUP_COMPONENTS}")

	target_compile_definitions( ${CurrLib} PRIVATE JucePlugin_Build_AU=1 )

#    addJUCE( ${CURRENT_TARGET} )
#    install( TARGETS ${CURRENT_TARGET} DESTINATION "$ENV{HOME}/Library/Audio/Plug-Ins/Components" )
  endif()
endfunction(addJUCE_AU)

# --------------------------------------------------------------------------------------------------
function( addJUCE_Standalone name_ )

	get_filename_component(wrapper_module_name ${JUCE_PLUGIN_WRAPPER_DIR} NAME)
	set(JUCE_MODULE_AVAILABLE_juce_audio_utils ON)
#	file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_Standalone.cpp "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_Standalone.cpp>")
#	file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_utils.cpp "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_utils.cpp>")
	
	list(APPEND VST_STANDALONE_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_Standalone.cpp)
	list(APPEND VST_STANDALONE_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_utils.cpp)
		
	if (APPLE)
#		file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_VST_utils.mm "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_VST_utils.mm>")
		list(APPEND VST_STANDALONE_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_utils.mm ${JUCE_ROOT_DIR}/modules/juce_audio_utils/juce_audio_utils.h)
		list(APPEND VST_STANDALONE_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_formats.mm ${JUCE_ROOT_DIR}/modules/juce_audio_formats/juce_audio_formats.h)
	else(APPLE)
		list(APPEND VST_STANDALONE_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_utils.cpp ${JUCE_ROOT_DIR}/modules/juce_audio_utils/juce_audio_utils.h)
		list(APPEND VST_STANDALONE_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_formats.cpp ${JUCE_ROOT_DIR}/modules/juce_audio_formats/juce_audio_formats.h)
	endif (APPLE)
	
	if (APPLE)
#		file(WRITE ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_VST_utils.mm "#include \"AppConfig.h\"\n#include <${wrapper_module_name}/juce_audio_plugin_client_VST_utils.mm>")
		list(APPEND VST_SOURCES ${JUCE_SOURCES_FOLDER}/include_juce_audio_plugin_client_VST_utils.mm)
	endif (APPLE)

#######################REPROJUCER##########################

	source_group( "JUCE\\vst" FILES ${VST_STANDALONE_SOURCES} )
	set( CurrLib "${name_}Standalone" )

	if (APPLE)
        add_executable(${CurrLib} MACOSX_BUNDLE
	   ${${CurrLib}_SOURCES} 
	   ${${CurrLib}_HEADERS} 
	   ${VST_STANDALONE_SOURCES} 
	   ${VST_STANDALONE_HEADERS}
        )
	else(APPLE)
 		add_executable(${CurrLib} WIN32
	   		${${CurrLib}_SOURCES} 
	   		${${CurrLib}_HEADERS} 
	   		${VST_STANDALONE_SOURCES} 
	   		${VST_STANDALONE_HEADERS}
		)
		#set_target_properties(${CurrLib} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
		#set_target_properties(${CurrLib} PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
		#set_target_properties(${CurrLib} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")

   
  
	endif (APPLE)

    #    target_link_libraries(${CurrLib} ${shared_code_target})
    #    add_dependencies(${CurrLib} ${auv3_target})
    #    __generate_plist_file(${CurrLib} "AUv3_Standalone" "APPL" "????"
    #      "${main_plist_entries}" ""
    #    )
    #    __set_common_target_properties(${CurrLib})
    #    __set_JucePlugin_Build_defines(${CurrLib} "StandalonePlugIn")
    #    __link_osx_frameworks(${CurrLib}
    #      ${JUCER_PROJECT_OSX_FRAMEWORKS} ${JUCER_EXTRA_FRAMEWORKS}
    #    )
    #    __add_xcode_resources(${CurrLib} ${JUCER_CUSTOM_XCODE_RESOURCE_FOLDERS})
    #    install(TARGETS ${auv3_target} COMPONENT _embed_app_extension_in_standalone_app
    #      DESTINATION "$<TARGET_FILE_DIR:${CurrLib}>/../PlugIns"
    #    )
    #    add_custom_command(TARGET ${CurrLib} POST_BUILD
    #      COMMAND
    #      "${CMAKE_COMMAND}"
    #      "-DCMAKE_INSTALL_CONFIG_NAME=$<CONFIG>"
    #      "-DCMAKE_INSTALL_COMPONENT=_embed_app_extension_in_standalone_app"
    #      "-P" "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake"
    #    )

#################################################
	target_compile_definitions( ${CurrLib} PRIVATE JUCE_STANDALONE_APPLICATION)

	target_include_directories(${CurrLib} PUBLIC ${JUCE_ROOT_DIR}/modules)
	set_target_properties(${CurrLib} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(${CurrLib} PROPERTIES OUTPUT_NAME "VISR ${name_}")
	set_target_properties(${CurrLib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/dawplugins/Standalone" )

	target_link_libraries(${CurrLib} ${name_})

	install(TARGETS ${CurrLib} COMPONENT ${CurrLib} ARCHIVE DESTINATION dawplugins/Standalone RUNTIME DESTINATION dawplugins/Standalone LIBRARY DESTINATION dawplugins/Standalone BUNDLE DESTINATION dawplugins/Standalone)

	string(TOUPPER ${CurrLib} CURRLIB_UPPER)
	get_property(STANDALONE_GROUP_COMPONENTS GLOBAL PROPERTY standalone_group)
	LIST( APPEND STANDALONE_GROUP_COMPONENTS ${CURRLIB_UPPER})
	set_property(GLOBAL PROPERTY standalone_group "${STANDALONE_GROUP_COMPONENTS}")

#	message( STATUS "STANDALONE INST " ${name_}) 


endfunction(addJUCE_Standalone)

# --------------------------------------------------------------------------------------------------
function( addJUCEPlugins name_ pluginCode)

if(BUILD_DAWPLUGINS_VST2)
	SET(JucePlugin_Build_VST  1)
else(BUILD_DAWPLUGINS_VST2)
	SET(JucePlugin_Build_VST  0)
endif(BUILD_DAWPLUGINS_VST2)
if(BUILD_DAWPLUGINS_VST3)  
	SET(JucePlugin_Build_VST3 1)
else(BUILD_DAWPLUGINS_VST3)
	SET(JucePlugin_Build_VST3 0)
endif(BUILD_DAWPLUGINS_VST3)
if(BUILD_DAWPLUGINS_AU)  
	SET(JucePlugin_Build_AU 1)
else(BUILD_DAWPLUGINS_AU)
	SET(JucePlugin_Build_AU 0)
endif(BUILD_DAWPLUGINS_AU)
if(BUILD_DAWPLUGINS_AAX)  
	SET(JucePlugin_Build_AAX 1)
else(BUILD_DAWPLUGINS_AAX)
	SET(JucePlugin_Build_AAX 0)
endif(BUILD_DAWPLUGINS_AAX)
if(BUILD_DAWPLUGINS_STANDALONE)  
	SET(JucePlugin_Build_STANDALONE 1)
else(BUILD_DAWPLUGINS_STANDALONE)
	SET(JucePlugin_Build_STANDALONE 0)
endif(BUILD_DAWPLUGINS_STANDALONE)

file(WRITE ${CMAKE_SOURCE_DIR}/resources/juce/.temp.txt "${pluginCode}")
file(READ ${CMAKE_SOURCE_DIR}/resources/juce/.temp.txt PLCODE LIMIT 4 HEX)
file(REMOVE ${CMAKE_SOURCE_DIR}/resources/juce/.temp.txt)
SET( HexPluginCode 0x${PLCODE} )

configure_file(${CMAKE_SOURCE_DIR}/resources/juce/AppConfig.h.in ${JUCE_HEADERS_FOLDER}/AppConfig.h @ONLY)

SET(JUCE_INFO_FOLDER ${CMAKE_BINARY_DIR}/resources/juce/info/${name_})

  if( APPLE )
  	target_link_libraries(${name_} PRIVATE ${OS_LIBS})
  	
	if(BUILD_DAWPLUGINS_AU)
	    addJUCE_AU( ${name_} )
	endif(BUILD_DAWPLUGINS_AU)
  endif( APPLE )

if (BUILD_DAWPLUGINS_VST2)
  addJUCE_VST2( ${name_})

endif(BUILD_DAWPLUGINS_VST2)		

if (BUILD_DAWPLUGINS_AAX)
  addJUCE_AAX( ${name_})
endif(BUILD_DAWPLUGINS_AAX)

if (BUILD_DAWPLUGINS_VST3)
  addJUCE_VST3( ${name_} )
endif(BUILD_DAWPLUGINS_VST3)		

if (BUILD_DAWPLUGINS_STANDALONE)
  addJUCE_Standalone( ${name_} )
endif(BUILD_DAWPLUGINS_STANDALONE)

endfunction( addJUCEPlugins )
