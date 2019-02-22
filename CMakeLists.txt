cmake_minimum_required(VERSION 3.13)
project(Game)

set(CMAKE_CXX_STANDARD 17)



add_executable(Game main.cpp entity.cpp entity.h player.cpp player.h weapon.cpp weapon.h enemy.cpp enemy.h object.cpp object.h)




target_link_libraries(Game -lsfml-graphics -lsfml-window
        -lsfml-system -lGL /home/leonid/Fun/Box2D/Build/bin/x86_64/Debug/libBox2D.a)