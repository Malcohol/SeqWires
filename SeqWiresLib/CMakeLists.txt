SET( SEQWIRESLIB_SRCS
	Utilities/musicUtilities.cpp
	Features/trackFeature.cpp
	Processors/fingeredChordsProcessor.cpp
	Processors/chordMapProcessor.cpp
	Processors/concatenateProcessor.cpp
	Processors/excerptProcessor.cpp
	Processors/mergeProcessor.cpp
	Processors/monophonicSubtracksProcessor.cpp
	Processors/percussionMapProcessor.cpp
	Processors/quantizeProcessor.cpp
	Processors/repeatProcessor.cpp
	Processors/silenceProcessor.cpp
	Processors/splitAtPitchProcessor.cpp
	Processors/transposeProcessor.cpp
	Functions/appendTrackFunction.cpp
	Functions/fingeredChordsFunction.cpp
	Functions/mapChordsFunction.cpp
	Functions/excerptFunction.cpp
	Functions/repeatFunction.cpp
	Functions/percussionMapFunction.cpp
	Functions/mergeFunction.cpp
	Functions/monophonicSubtracksFunction.cpp
	Functions/quantizeFunction.cpp
	Functions/sanitizingFunctions.cpp
	Functions/splitAtPitchFunction.cpp
	Functions/transposeFunction.cpp
	Tracks/chordEvents.cpp
	Tracks/noteEvents.cpp
	Tracks/percussionEvents.cpp
	Tracks/trackEvent.cpp
	Tracks/track.cpp
	Tracks/trackType.cpp
	Tracks/trackTypeConstructor.cpp
	chord.cpp
	musicTypes.cpp
	Percussion/abstractPercussionSet.cpp
	Percussion/builtInPercussionInstruments.cpp
	Percussion/percussionSetWithPitchMap.cpp
	pitch.cpp
	Utilities/monophonicNoteIterator.cpp
	libRegistration.cpp
   )

ADD_LIBRARY( SeqWiresLib ${SEQWIRESLIB_SRCS} )
TARGET_INCLUDE_DIRECTORIES( SeqWiresLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../BabelWires )
# Add pthread for Linux and/or GCC?
TARGET_LINK_LIBRARIES(SeqWiresLib PUBLIC BabelWiresLib tinyxml2)
