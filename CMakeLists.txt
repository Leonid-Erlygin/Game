cmake_minimum_required(VERSION 3.13)
project(Game)

set(CMAKE_CXX_STANDARD 17)



add_executable(Game main.cpp entity.cpp entity.h player.cpp player.h object.cpp object.h MyContactListener.cpp MyContactListener.h
        virtualPlayer.cpp virtualPlayer.h weapon.h
        weapon.cpp handWeapon.cpp handWeapon.h grenade.cpp grenade.h
        Map/level.cpp Map/level.h Map/tinystr.cpp Map/tinystr.h Map/tinyxml.cpp Map/tinyxml.h Map/tinyxmlerror.cpp Map/tinyxmlparser.cpp
        GameCore.cpp GameCore.h)








target_link_libraries(Game -lsfml-graphics -lsfml-window
        -lsfml-system -lGL -lsfml-network /home/leonid/Fun/Box2D/Build/bin/x86_64/Debug/libBox2D.a)