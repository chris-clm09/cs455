void printProjectionMatrix()
{
   float x[16];
   int i;

   //read the 4x4 matrix and store in x
   glGetFloatv (GL_PROJECTION_MATRIX, (float*)x);

   //print the matrix
   for(i=0; i<4; i++)
      cout << x[i] << "  "   <<  x[i+4] << "  " 
           << x[i+8] << "  " << x[i+12] << endl; 

   return;
}

void printModelViewMatrix()
{
   float x[16];
   int i;

   //read the 4x4 matrix and store in x
   glGetFloatv (GL_MODELVIEW_MATRIX, (float*)x);

   //print the matrix
   for(i=0; i<4; i++)
      cout << x[i] << "  "   <<  x[i+4] << "  " 
           << x[i+8] << "  " << x[i+12] << endl; 

   return;
}
