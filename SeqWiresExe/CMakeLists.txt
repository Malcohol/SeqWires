SET( SEQWIRES_SRCS
	main.cpp
	seqWiresOptions.cpp
   )

ADD_EXECUTABLE( seqWires ${SEQWIRES_SRCS} )
TARGET_INCLUDE_DIRECTORIES( seqWires PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../BabelWires ${CMAKE_CURRENT_SOURCE_DIR}/.. )
TARGET_LINK_LIBRARIES( seqWires SmfLib SeqWiresLibUi BabelWiresQtUi BabelWiresLib Common tinyxml2)
