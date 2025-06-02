SET( SEQ2TAPE_SRCS
	seq2tape.cpp
	seq2tapeOptions.cpp
	audioInit.cpp
   )

FIND_PACKAGE( ALSA )
IF( ALSA_FOUND )
	SET( ALSA_SRCS
		Alsa/alsaAudioDest.cpp
		Alsa/alsaAudioSource.cpp
		Alsa/alsaCommon.cpp
		Alsa/alsaAudioInterface.cpp
		Alsa/alsaInit.cpp
	)
	LIST(APPEND SEQ2TAPE_SRCS ${ALSA_SRCS})
ELSE()
    MESSAGE(NOTICE "ALSA libs not found. Will not use ALSA for audio.")
ENDIF( ALSA_FOUND )

ADD_EXECUTABLE( seq2tape ${SEQ2TAPE_SRCS} )
TARGET_INCLUDE_DIRECTORIES( seq2tape PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )
TARGET_LINK_LIBRARIES( seq2tape Seq2tapeLib Common )

IF (ALSA_FOUND)
	TARGET_LINK_LIBRARIES( seq2tape asound )
ENDIF(ALSA_FOUND)