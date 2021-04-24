SET( SEQ2TAPELIB_SRCS
	tapeFile.cpp
	tapeFileFormat.cpp
	waveReader.cpp
	sampleReader.cpp
   )

ADD_LIBRARY( Seq2tapeLib ${SEQ2TAPELIB_SRCS} )
TARGET_INCLUDE_DIRECTORIES( Seq2tapeLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../BabelWires ${CMAKE_CURRENT_SOURCE_DIR}/.. )
TARGET_LINK_LIBRARIES(Seq2tapeLib Common)
