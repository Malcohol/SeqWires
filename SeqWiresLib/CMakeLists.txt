SET( SEQWIRESLIB_SRCS
	Utilities/musicUtilities.cpp
	Features/trackFeature.cpp
	Features/pitchFeature.cpp
	Processors/concatenateProcessor.cpp
	Processors/excerptProcessor.cpp
	Processors/mergeProcessor.cpp
	Processors/repeatProcessor.cpp
	Processors/silenceProcessor.cpp
	Processors/splitAtPitchProcessor.cpp
	Processors/transposeProcessor.cpp
	Functions/appendTrackFunction.cpp
	Functions/excerptFunction.cpp
	Functions/repeatFunction.cpp
	Functions/mergeFunction.cpp
	Functions/monophonicSubtracksFunction.cpp
	Functions/splitAtPitchFunction.cpp
	Functions/transposeFunction.cpp
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
