
IF( NOT AAX_FOUND )

	FILE(GLOB AAX_INCLUDE_PATHS "${HOME}/AAX")

	FIND_PATH(
	  AAX_INCLUDE_DIR
	  ./base/source/*
	  ${AAX_INCLUDE_PATHS}
	)
ENDIF( NOT AAX_FOUND )

