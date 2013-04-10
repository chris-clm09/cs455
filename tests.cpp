/**********************************************************
* This function will draw a simple sphere into world
* coordinate and then use ray tracing to generate
* the screen coordinantes.
**********************************************************/
void draw0()
{
  //Clear the raster
  initRaster();

  //Set up Scene
  currentScene.clear();

  //Add Objects
  double s[] = {500,500,300,0};
  Sphere a(s, 100, vector4(1,0,0,0));
  currentScene.addObj(a);  

  //Added a Light
  currentScene.addLight(Light(vector4(600,400,0,0),
                              vector4(1,1,1,1),
                              vector4(.1,.1,.1,1),
                              vector4(0,1,0,0)));

  //Set Camera
  double s1[] = {500,500,0,0};
  double s2[] = {0,0,1,0};
  currentScene.setCamera(Camera(s1,s2));

  //Ray_Trace
  ray_trace();
  
  cout << "DONE0" << endl;

  return;
}

/**********************************************************
* This function will draw a simple sphere into world
* coordinate and then use ray tracing to generate
* the screen coordinantes.
**********************************************************/
void draw1()
{
  //Clear the raster
  initRaster();

  //Set up Scene
  currentScene.clear();

  double i = 1;

  //Add Objects
  double s[] = {233,290,400,0};
  Sphere a(s, 100, vector4(i,i,0,0), .5);
  currentScene.addObj(a);  

  //Add Objects
  double s1[] = {407,290,400,0};
  Sphere a1(s1, 100, vector4(0,i,i,0), .5);
  currentScene.addObj(a1);  

  //Add Objects
  double s2[] = {320,140,400,0};
  Sphere a2(s2, 100, vector4(i,0,i,0), .5);
  currentScene.addObj(a2);  

  //Added a Light
  currentScene.addLight(Light(vector4(0,240,250,0),
                              vector4(1,1,1,1),
                              vector4(.1,.1,.1,1),
                              vector4(0,1,0,0)));

//Added a Light
  currentScene.addLight(Light(vector4(640,240,0,0),
                              vector4(.09,.09,.09,0),
                              vector4(.1,.1,.1,1),
                              vector4(0,1,0,0)));

  //Set Camera
  double cpos[] = {320,215,0,0};
  double cdir[] = {0,0,1,0};
  currentScene.setCamera(Camera(cpos,cdir));

  //Ray_Trace
  ray_trace();
  
  cout << "DONE1" << endl;

  return;
}

/**********************************************************
**********************************************************/
void draw2()
{
  //Clear the raster
  initRaster();

  //Set up Scene
  currentScene.clear();

  //Add Objects
  double s[] = {500,500,300,0};

  Sphere a(s, 100, vector4(1,0,0,0), 1, vector4(1,1,1,0), 1);
  currentScene.addObj(a);  

  //Added a Light
  currentScene.addLight(Light(vector4(600,400,99,0),
                              vector4(1,1,1,1),
                              vector4(.1,.1,.1,1),
                              vector4(0,1,0,0)));


  //Set Camera
  double s1[] = {500,500,0,0};
  double s2[] = {0,0,1,0};
  currentScene.setCamera(Camera(s1,s2));

  //Ray_Trace
  ray_trace();
  
  cout << "DONE2" << endl;

  return;
}

/**********************************************************
**********************************************************/
void draw3()
{
  //Clear the raster
  initRaster();

  //Set up Scene
  currentScene.clear();

  double i = 1;

  //Add Objects
  double s[] = {233,290,400,0};
  Sphere a(s, 100, vector4(i,i,0,0), .5, vector4(1,1,1,0), 1);
  currentScene.addObj(a);  

  //Add Objects
  double s1[] = {407,290,400,0};
  Sphere a1(s1, 100, vector4(0,i,i,0), .5, vector4(1,0,1,0), 1);
  currentScene.addObj(a1);  

  //Add Objects
  double s2[] = {320,140,400,0};
  Sphere a2(s2, 100, vector4(i,0,i,0), .5, vector4(0,1,1,0), 1);
  currentScene.addObj(a2);  

  //Added a Light
  currentScene.addLight(Light(vector4(0,240,250,0),
                              vector4(1,1,1,1),
                              vector4(.1,.1,.1,1),
                              vector4(0,1,0,0)));

//Added a Light
  currentScene.addLight(Light(vector4(640,240,0,0),
                              vector4(.09,.09,.09,0),
                              vector4(.1,.1,.1,1),
                              vector4(0,1,0,0)));

  //Set Camera
  double cpos[] = {320,215,0,0};
  double cdir[] = {0,0,1,0};
  currentScene.setCamera(Camera(cpos,cdir));

  //Ray_Trace
  ray_trace();
  
  cout << "DONE3" << endl;

  return;
}

/**********************************************************
**********************************************************/
void draw4()
{
  //Clear the raster
  initRaster();

  //Set up Scene
  currentScene.clear();

  double i = 1;

  //Add Objects
  double s[] = {233,290,400,0};
  Sphere a(s, 100, vector4(i,0,0,0), .5, vector4(1,1,1,0), 1);
  currentScene.addObj(a);  

  //Add Objects
  double s1[] = {407,290,400,0};
  Sphere a1(s1, 100, vector4(0,i,0,0), .5, vector4(1,1,1,0), 1);
  currentScene.addObj(a1);  

  //Add Objects
  double s2[] = {320,140,400,0};
  Sphere a2(s2, 100, vector4(0,0,i,0), .5, vector4(1,1,1,0), 1);
  currentScene.addObj(a2);  

  //Added a Light
  currentScene.addLight(Light(vector4(0,240,250,0),
                              vector4(1,1,1,1),
                              vector4(.1,.1,.1,1),
                              vector4(1,1,1,0)));

//Added a Light
  currentScene.addLight(Light(vector4(700,240,200,0),
                              vector4(1,1,1,0),
                              vector4(.1,.1,.1,1),
                              vector4(1,1,1,0)));

  //Set Camera
  double cpos[] = {320,215,0,0};
  double cdir[] = {0,0,1,0};
  currentScene.setCamera(Camera(cpos,cdir));

  //Ray_Trace
  ray_trace();
  
  cout << "DONE4" << endl;

  return;
}

/**********************************************************
**********************************************************/
void draw5()
{
  //Clear the raster
  initRaster();

  //Set up Scene
  currentScene.clear();

  double i = 1;

  //Add Objects
  double s[] = {233,290,400,0};
  Sphere a(s, 100, vector4(i,0,0,0), .5, vector4(1,1,1,0), 1);
  currentScene.addObj(a);  

  //Add Objects
  double s1[] = {407,290,400,0};
  Sphere a1(s1, 100, vector4(0,i,0,0), .5, vector4(1,1,1,0), 1);
  currentScene.addObj(a1);  

  //Add Objects
  double s2[] = {320,140,400,0};
  Sphere a2(s2, 100, vector4(0,0,i,0), .5, vector4(1,1,1,0), 1);
  currentScene.addObj(a2);  

  //Added a Light
  currentScene.addLight(Light(vector4(0,240,250,0),
                              vector4(1,1,1,1),
                              vector4(.1,.1,.1,1),
                              vector4(1,1,1,0)));

//Added a Light
  currentScene.addLight(Light(vector4(700,240,200,0),
                              vector4(1,1,1,0),
                              vector4(.1,.1,.1,1),
                              vector4(1,1,1,0)));

  //Set Camera
  vector4 cpos(620,100,180,0);
  vector4 cdir = (a2.pos - cpos).normalize();
  currentScene.setCamera(Camera(cpos,cdir));

  //Ray_Trace
  ray_trace();
  
  cout << "DONE5" << endl;

  return;
}

/**********************************************************
**********************************************************/
void draw6()
{
  //Clear the raster
  initRaster();

  //Set up Scene
  currentScene.clear();

  double i = 1;

  //Add Objects
  double s[] = {233,290,400,0};
  Sphere a(s, 100, vector4(i,0,0,0), .5, vector4(1,1,1,0), 1);
  currentScene.addObj(a);  

  //Add Objects
  double s1[] = {407,290,400,0};
  Sphere a1(s1, 100, vector4(0,i,0,0), .5, vector4(1,1,1,0), 1);
  currentScene.addObj(a1);  

  //Add Objects
  double s2[] = {320,140,400,0};
  Sphere a2(s2, 100, vector4(0,0,i,0), .5, vector4(1,1,1,0), 1);
  currentScene.addObj(a2);  

  //Added a Light
  currentScene.addLight(Light(vector4(0,240,250,0),
                              vector4(1,1,1,1),
                              vector4(.1,.1,.1,1),
                              vector4(1,1,1,0)));

//Added a Light
  currentScene.addLight(Light(vector4(700,240,200,0),
                              vector4(1,1,1,0),
                              vector4(.1,.1,.1,1),
                              vector4(1,1,1,0)));

  //Set Camera
  vector4 cpos(820,100,400,0);
  vector4 cdir = (a2.pos - cpos).normalize();
  currentScene.setCamera(Camera(cpos,cdir));

  //Ray_Trace
  ray_trace();
  
  cout << "DONE6" << endl;

  return;
}

/**********************************************************
**********************************************************/
void draw7()
{
  //Clear the raster
  initRaster();

  //Set up Scene
  currentScene.clear();

  double i = 1;

  //Add Objects
  double s[] = {233,290,400,0};
  Sphere a(s, 100, vector4(i,0,0,0), .5, vector4(1,1,1,0), 1);
  currentScene.addObj(a);  

  //Add Objects
  double s1[] = {407,290,400,0};
  Sphere a1(s1, 100, vector4(0,i,0,0), .5, vector4(1,1,1,0), 1);
  currentScene.addObj(a1);  

  //Add Objects
  double s2[] = {320,140,400,0};
  Sphere a2(s2, 100, vector4(0,0,i,0), .5, vector4(1,1,1,0), 1);
  currentScene.addObj(a2);  

  //Added a Light
  currentScene.addLight(Light(vector4(0,240,250,0),
                              vector4(1,1,1,1),
                              vector4(.1,.1,.1,1),
                              vector4(1,1,1,0)));

//Added a Light
  currentScene.addLight(Light(vector4(700,240,200,0),
                              vector4(1,1,1,0),
                              vector4(.1,.1,.1,1),
                              vector4(1,1,1,0)));

  //Set Camera
  vector4 cpos(720,100,900,0);
  vector4 cdir = (a2.pos - cpos).normalize();
  currentScene.setCamera(Camera(cpos,cdir));

  //Ray_Trace
  ray_trace();
  
  cout << "DONE7" << endl;

  return;
}

/**********************************************************
**********************************************************/
void draw8()
{
  //Clear the raster
  initRaster();

  //Set Camera
  vector4 cpos(250,115,700,0);
  vector4 cdir = (currentScene.sceneObjects[2].pos - cpos).normalize();
  currentScene.setCamera(Camera(cpos,cdir));

  //Ray_Trace
  ray_trace();
  
  cout << "DONE8" << endl;

  return;
}

/**********************************************************
**********************************************************/
void draw9()
{
  //Clear the raster
  initRaster();

  //Set Camera
  vector4 cpos(0,115,250,0);
  vector4 cdir = (currentScene.sceneObjects[2].pos - cpos).normalize();
  currentScene.setCamera(Camera(cpos,cdir));

  //Ray_Trace
  ray_trace();
  
  cout << "DONE8" << endl;

  return;
}