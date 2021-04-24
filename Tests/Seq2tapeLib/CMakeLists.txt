SET( SEQ2TAPELIB_TESTS_SRCS
      seq2tapeLibTests.cpp
      waveReaderTest.cpp
   )

ADD_EXECUTABLE( seq2tapeLibTests ${SEQ2TAPELIB_TESTS_SRCS} )
TARGET_INCLUDE_DIRECTORIES( seq2tapeLibTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../BabelWires ${CMAKE_CURRENT_SOURCE_DIR}/../.. )
TARGET_LINK_LIBRARIES(seq2tapeLibTests Common Seq2tapeLib testUtils gtest)
