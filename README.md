## Client part of multiplayer game
### How to compile:
1) use  
$ sudo apt-get install libsfml-dev  
for installig SFML on your linux machine
2) Now you need to compile and link (in Cmakefile) Box2d library.  
To do this use zip box2d file in the repository:
* unzip it
* run build.sh file
* copy /include foulder and /build/scr/libbox2d.a file to project's directory
* change in CMakeLists.txt file the path to the libbox2d.a file
