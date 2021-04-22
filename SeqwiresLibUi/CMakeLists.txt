SET( SEQWIRESLIB_UI_SRCS
    RowModels/trackRowModel.cpp
    libRegistration.cpp
)

ADD_LIBRARY( SeqwiresLibUi ${SEQWIRESLIB_UI_SRCS} )
TARGET_INCLUDE_DIRECTORIES( SeqwiresLibUi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../BabelWires ${CMAKE_CURRENT_SOURCE_DIR}/.. )
TARGET_LINK_LIBRARIES( SeqwiresLibUi BabelWiresQtUi BabelWiresLib SeqwiresLib Common)