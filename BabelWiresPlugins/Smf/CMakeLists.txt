SET( SMFLIB_SRCS
	libRegistration.cpp
	smfModel.cpp
	smfWriter.cpp
	smfParser.cpp
	smfFormat.cpp
   )

ADD_LIBRARY( SmfLib ${SMFLIB_SRCS} )
TARGET_INCLUDE_DIRECTORIES( SmfLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../BabelWires )
TARGET_LINK_LIBRARIES(SmfLib BabelWiresLib SeqwiresLib Common)