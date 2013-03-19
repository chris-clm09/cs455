echo "Removing a.out"
rm a.out
echo "Compile."
g++ -Ofast -l GL -l glut -l GLU -I cml-1_0_2/ project4.cpp
echo "run"
./a.out
