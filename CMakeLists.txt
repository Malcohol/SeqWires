CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

###############################################################################
# Build the project
###############################################################################

PROJECT( Seqwires )

SET( CMAKE_EDIT_COMMAND /usr/bin/cmake -i )
SET( CMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT TRUE )
SET(CMAKE_CXX_STANDARD 17)
# Lots of work will be required to support shared libs. Waiting to see whether C++ Modules will help.
set(BUILD_SHARED_LIBS OFF)
SET(gtest_force_shared_crt ON)

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR} )

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

ADD_SUBDIRECTORY( BabelWires )
ADD_SUBDIRECTORY( SeqWiresLib )
ADD_SUBDIRECTORY( Seq2tapeLib )
ADD_SUBDIRECTORY( SeqWiresLibUi )

ADD_SUBDIRECTORY( BabelWiresPlugins/Smf )

ADD_SUBDIRECTORY( BabelWiresAudio )

ADD_SUBDIRECTORY( SeqWiresExe )
ADD_SUBDIRECTORY( Seq2tapeExe )

ADD_SUBDIRECTORY( Tests/SeqWiresLib )
ADD_SUBDIRECTORY( Tests/Seq2tapeLib )
