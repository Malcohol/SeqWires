SET( SEQWIRESLIB_UI_SRCS
    ValueModels/trackValueModel.cpp
    libRegistration.cpp
)

ADD_LIBRARY( SeqWiresLibUi ${SEQWIRESLIB_UI_SRCS} )
TARGET_INCLUDE_DIRECTORIES( SeqWiresLibUi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )
TARGET_LINK_LIBRARIES( SeqWiresLibUi BabelWiresQtUi BabelWiresLib SeqWiresLib Common)