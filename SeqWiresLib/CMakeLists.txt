SET( SEQWIRESLIB_SRCS
	Utilities/musicUtilities.cpp
	Features/tempoFeature.cpp
	Features/trackFeature.cpp
	Processors/concatenateProcessor.cpp
	Processors/excerptProcessor.cpp
	Processors/repeatProcessor.cpp
	Processors/silenceProcessor.cpp
	Functions/appendTrackFunction.cpp
	Functions/excerptFunction.cpp
	Functions/repeatFunction.cpp
	Functions/monophonicSubtracksFunction.cpp
	Tracks/trackEvent.cpp
	Tracks/noteEvents.cpp
	Tracks/chordEvents.cpp
	Tracks/track.cpp
	musicTypes.cpp
	Utilities/monophonicNoteIterator.cpp
	libRegistration.cpp
   )

ADD_LIBRARY( SeqWiresLib ${SEQWIRESLIB_SRCS} )
TARGET_INCLUDE_DIRECTORIES( SeqWiresLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../BabelWires )
# Add pthread for Linux and/or GCC?
TARGET_LINK_LIBRARIES(SeqWiresLib PUBLIC BabelWiresLib tinyxml2)
