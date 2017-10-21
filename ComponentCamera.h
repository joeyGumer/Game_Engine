#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Globals.h"
#include "MathGeoLib-1.5\src\Geometry\Frustum.h"
#include "SDL\include\SDL.h"

class ComponentCamera :public Component
{
public:
	ComponentCamera(GameObject* my_go,float near_distance,float far_distance,bool active);
	~ComponentCamera();
	bool ObjectInside();
	void InspectorUpdate();

	//Getters
	float GetNearDistance();
	float GetFardistance();
	int   GetAspectRatio();

	//Setters
	void SetNearDistance(float distance);
	void SetFarDistance(float distance);



public:
	Frustum camera_frustrum;

private:
	bool enable = false;
	float3 front{ 0, 0, 1 };
	float3 up{ 0,1,0 };
	float3 pos{ 0,0,0 };
	float vertical_fov = 1.0f;
	SDL_DisplayMode current;
	float window_aspect_ratio;
};
