echo "Removing a.out"
rm a.out
echo "Compile."
g++ -l GL -l glut -l GLU -I cml-1_0_2/ project2.cpp
echo "run"
./a.out
