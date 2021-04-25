## Client part of multiplayer game
### How to compile:
1) Use  
$ sudo apt-get install libsfml-dev  
for installig SFML on your linux machine
2) Here CMake 3.13 or higher is required. Check your version with   
$ cmake --version  
if you do not have right one, just run install_cmake.sh(DO NOT RUN IF YOU DONT WANT TO REINSTALL CMake) script

3) Now to install Box2d library just run build_box2d.sh script  

4) To build the project run build_project.sh

To run the game go to cmake-build-debug foulder and run Game file with an argument corresponding to number of players.  
Then enter your player index(1 , 2, ... ect). So if you have two player game, you, for example, enter 1 index and your freind 2 index here.
