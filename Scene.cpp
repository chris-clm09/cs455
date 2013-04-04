#ifndef clm_scene
#define clm_scene

#include "WorldStuff.cpp"
#include "SceneObject.cpp"
#include <vector>
using namespace std;

/************************************************************
* A Scene contains all the lights, objects, and the cammera
* for a particular world.
************************************************************/
class Scene
{
public:
	Scene(){}

	void addLight(const Light & l) 		{lights.push_back(l);}
	void addObj(const SceneObject &obj) {sceneObjects.push_back(obj);}
	void setCamera(const Camera &cam)   {camera = cam;}

	void clear()
	{	
		lights.clear();
		sceneObjects.clear();
		camera.clear();
	}
//private:
	vector<Light> lights;
	vector<SceneObject> sceneObjects;
	Camera camera;
};

#endif