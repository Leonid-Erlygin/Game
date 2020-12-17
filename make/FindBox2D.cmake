set(FIND_BOX2D_PATHS
        /home/leonid/Fun/Box2D/Build/Box2D)

find_path(BOX2D_INCLUDE_DIR Box2D.h
        PATH_SUFFIXES include
        PATHS ${FIND_BOX2D_PATHS})

find_library(BOX2D_LIBRARY
        NAMES libBox2D.a
        PATH_SUFFIXES lib
        PATHS ${FIND_BOX2D_PATHS})