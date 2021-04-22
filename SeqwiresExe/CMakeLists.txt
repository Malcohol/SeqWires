SET( SEQWIRES_SRCS
	main.cpp
	seqwiresOptions.cpp
   )

ADD_EXECUTABLE( seqwires ${SEQWIRES_SRCS} )
TARGET_INCLUDE_DIRECTORIES( seqwires PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../BabelWires ${CMAKE_CURRENT_SOURCE_DIR}/.. )
TARGET_LINK_LIBRARIES( seqwires Pmc100Lib SmfLib SeqwiresLibUi BabelWiresQtUi BabelWiresLib Common tinyxml2)
