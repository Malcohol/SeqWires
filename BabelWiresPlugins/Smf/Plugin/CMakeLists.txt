SET( SMFLIB_SRCS
	libRegistration.cpp
	smfSourceModel.cpp
	smfTargetModel.cpp
	smfModelCommon.cpp
	smfWriter.cpp
	smfParser.cpp
	Percussion/smfPercussion.cpp
	smfFormat.cpp
	gmSpec.cpp
   )

ADD_LIBRARY( SmfLib ${SMFLIB_SRCS} )
TARGET_INCLUDE_DIRECTORIES( SmfLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../BabelWires )
TARGET_LINK_LIBRARIES(SmfLib BabelWiresLib SeqWiresLib Common)