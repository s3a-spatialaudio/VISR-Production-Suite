
IF( NOT VST_FOUND )

	FILE(GLOB VST_INCLUDE_PATHS "${CMAKE_SOURCE_DIR}/3rdPartyLibs/vstsdk2.4")

	FIND_PATH(
	  VST_INCLUDE_DIR
	  ./public.sdk/source/vst2.x/audioeffect.h
	  ${VST_INCLUDE_PATHS}
	)

	include (FindPackageHandleStandardArgs)
	find_package_handle_standard_args(VST DEFAULT_MSG VST_INCLUDE_DIR)

ENDIF( NOT VST_FOUND )

