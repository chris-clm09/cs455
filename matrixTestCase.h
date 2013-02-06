void tree(int depth) {
  static const double r2 = 1/sqrt(2);
  static const double mdown[16] = { 0,-r2,0,0, r2,0,0,0, 0,0,1,0, 0,-r2,0,1 };
  static const double mup[16] = { 0,r2,0,0, -r2,0,0,0, 0,0,1,0, 0,r2,0,1 };
   if (depth <= 0) return;
   clm_glBegin(GL_LINES);
    clm_glVertex2f(0,-r2);
    clm_glVertex2f(0, r2);
   clm_glEnd();
   clm_glPushMatrix();
   clm_glMultMatrixd(mdown);
   tree(depth-1);
   clm_glPopMatrix();
   clm_glPushMatrix();
   clm_glMultMatrixd(mup);
   tree(depth-1);
   clm_glPopMatrix();
}
void matrixTest() {
  tree(8);
}
