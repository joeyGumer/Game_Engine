#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
class GameObject;
class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status GuiUpdate();

	update_status Update(float dt);
	bool CleanUp();

	GameObject* GenerateGameObject(GameObject* parent = nullptr);
	
	Plane_p plane;
	
private:
	bool hierarchy = true;
	bool inspector = true;
public:
	void LoadTexture(const char* path);
	void SendGameObject(GameObject* go);
	void SetSelectedGameObject(GameObject* go);
private:
	GameObject* scene_go = nullptr;
	GameObject* selected_go = nullptr;


};