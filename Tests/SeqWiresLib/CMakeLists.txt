SET( SEQUENCELIB_TESTS_SRCS
      seqWiresLibTests.cpp
      chordMapProcessorTest.cpp
      fingeredChordsProcessorTest.cpp
      concatenateProcessorTest.cpp
      excerptProcessorTest.cpp
      filteredTrackIteratorTest.cpp
      mergeProcessorTest.cpp
      monophonicNoteIteratorTest.cpp
      monophonicSubtracksProcessorTest.cpp
      musicTypesTest.cpp
      percussionMapProcessorTest.cpp
      percussionSetTest.cpp
      quantizeProcessorTest.cpp
      repeatProcessorTest.cpp
      sanitizingFunctionsTest.cpp
      splitAtPitchProcessorTest.cpp
      trackTest.cpp
      trackTraverserTest.cpp
      transposeProcessorTest.cpp
   )

ADD_EXECUTABLE( seqWiresLibTests ${SEQUENCELIB_TESTS_SRCS} )
TARGET_INCLUDE_DIRECTORIES( seqWiresLibTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../BabelWires ${CMAKE_CURRENT_SOURCE_DIR}/../.. )
TARGET_LINK_LIBRARIES(seqWiresLibTests Common SeqWiresLib testUtils libTestUtils seqTestUtils gtest)
