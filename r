rm a.out
g++ -Ofast -g -std=c++11 -l GL -l glut -l GLU -I cml-1_0_2/ example.cpp
./a.out
