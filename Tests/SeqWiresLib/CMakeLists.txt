SET( SEQUENCELIB_TESTS_SRCS
      sequenceLibTests.cpp
      concatenateProcessorTest.cpp
      excerptProcessorTest.cpp
      filteredTrackIteratorTest.cpp
      mergeProcessorTest.cpp
      monophonicNoteIteratorTest.cpp
      monophonicSubtracksProcessorTest.cpp
      musicTypesTest.cpp
      repeatProcessorTest.cpp
      splitAtPitchProcessorTest.cpp
      trackTest.cpp
      trackTraverserTest.cpp
      transposeProcessorTest.cpp
   )

ADD_EXECUTABLE( sequenceLibTests ${SEQUENCELIB_TESTS_SRCS} )
TARGET_INCLUDE_DIRECTORIES( sequenceLibTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../BabelWires ${CMAKE_CURRENT_SOURCE_DIR}/../.. )
TARGET_LINK_LIBRARIES(sequenceLibTests Common SeqWiresLib testUtils seqTestUtils gtest)
