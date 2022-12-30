SET( SMFLIB_SRCS
	libRegistration.cpp
	smfSourceModel.cpp
	smfTargetModel.cpp
	smfModelCommon.cpp
	smfWriter.cpp
	smfParser.cpp
	Percussion/gmPercussionSet.cpp
	Percussion/gm2AnalogPercussionSet.cpp
	Percussion/gm2BrushPercussionSet.cpp
	Percussion/gm2ElectronicPercussionSet.cpp
	Percussion/gm2JazzPercussionSet.cpp
	Percussion/gm2OrchestraPercussionSet.cpp
	Percussion/gm2PowerPercussionSet.cpp
	Percussion/gm2SFXPercussionSet.cpp
	Percussion/gm2StandardPercussionSet.cpp
	Percussion/gm2RoomPercussionSet.cpp
	Percussion/gsStandard1PercussionSet.cpp
	Percussion/percussionSet.cpp
	Percussion/standardPercussionSets.cpp
	Percussion/xgStandard1PercussionSet.cpp
	Percussion/xgRoomPercussionSet.cpp
	smfFormat.cpp
	gmSpec.cpp
   )

ADD_LIBRARY( SmfLib ${SMFLIB_SRCS} )
TARGET_INCLUDE_DIRECTORIES( SmfLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../BabelWires )
TARGET_LINK_LIBRARIES(SmfLib BabelWiresLib SeqWiresLib Common)