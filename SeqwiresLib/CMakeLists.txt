SET( SEQWIRESLIB_SRCS
	Utilities/musicUtilities.cpp
	Features/tempoFeature.cpp
	Features/trackFeature.cpp
	Processors/excerptProcessor.cpp
	Functions/excerptFunction.cpp
	Functions/monophonicSubtracksFunction.cpp
	Tracks/trackEvent.cpp
	Tracks/noteEvents.cpp
	Tracks/chordEvents.cpp
	Tracks/track.cpp
	musicTypes.cpp
	Utilities/monophonicNoteIterator.cpp
	libRegistration.cpp
   )

ADD_LIBRARY( SeqwiresLib ${SEQWIRESLIB_SRCS} )
TARGET_INCLUDE_DIRECTORIES( SeqwiresLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../BabelWires )
TARGET_LINK_LIBRARIES(SeqwiresLib PUBLIC BabelWiresLib tinyxml2 pthread)
