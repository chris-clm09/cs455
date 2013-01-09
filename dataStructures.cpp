#include <iostream>
#include <vector>
#include "cml/cml.h"

using namespace std;

typedef cml::matrix44f_c matrix4;
typedef cml::vector3f vector3;
typedef cml::vector4f vector4;

vector3 cross(vector4 l, vector4 r)
{ 
   return cross(vector3(l[0],l[1],l[3]), 
                vector3(r[0],r[1],r[3]));
}

int main (int argc, char** argv)
{
   vector<int> v;
   v.push_back(1);
   v.push_back(2);
   
   for (int i = 0; i < v.size(); i++)
      cout << v[i] << endl;
   
   //Matrix Stuff
   cout << "List to Matrix:\n";   
   matrix4 m(1, 0, 0, 0, 2, 5, 0, 0, 3, 6, 8, 0, 4, 7, 9, 0);
   m = m.transpose();   
   cout << m << endl << endl;      

   cout << "Matrix to List:\n";
   for (int i = 0; i < 16; i++)
      cout << *(m.data() + i) << ' ';  
   cout << endl;
      
   //Vector Stuff
   cout << "Create Vector:\n";
   vector4 V(1,2,3,4);
   cout << V << endl;

   cout << "Vector to list:";   
   for (int i = 0; i < 4; i++)
      cout << *(V.data() + i) << ' ';  
   cout << endl;
   
   cout << "v*4:" << V * 4 << endl;
   cout << "v-v:" << V - V << endl;
   cout << "v+v:" << V + V << endl;
   
   //M*M
   matrix4 m2(1,0,0,0,0,2,0,0,1,0,3,0,0,2,0,4);
   m2 = m2.transpose();

   cout << "m*m2:\n" << m * m2 << endl;
   
   //M*V  
   vector4 V2(2,3,5,7);
   cout << "m*Vecotr:" << m * V2 << endl;
   
   //Dot Product
   cout << "v.v:" << dot(V, V) << endl;
   
   //3D Cross
   cout << "v(3X)v:\n" << cross(V, V) << endl;
   
   //Inverse
   matrix4 b = m2;
   b.inverse();
   cout << "Inverse of b:\n" << b << endl;  
   cout << "Proof of Inverse:\n" << b*m2 << endl;
   
   vector<matrix4> ms;
   ms.push_back(m2);
   ms.push_back(m);
   cout << "Vector created with Matrix's" << endl;
    
   return 0;
}
