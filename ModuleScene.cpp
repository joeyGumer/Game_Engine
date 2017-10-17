#include "Application.h"
#include "Globals.h"
#include"ModuleScene.h"
#include "Primitive_p.h"
#include "ModuleCamera3D.h"

ModuleScene::ModuleScene(bool start_enabled)
{
	name = "Scene";
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Start()
{
	bool ret = true;
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	scene_go = new GameObject();

	return ret;
}

update_status ModuleScene::GuiUpdate()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_ShowBorders;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_Once);
	if (!ImGui::Begin("Hierarchy", &hierarchy, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return UPDATE_CONTINUE;
	}
	
	if (scene_go != nullptr)
	{

		scene_go->GuiUpdate();

	}
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_Once);

	if (!ImGui::Begin("Inspector", &inspector, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return UPDATE_CONTINUE;
	}

	if (selected_go != nullptr)
	{

		selected_go->InspectorUpdate();

	}
	ImGui::End();

	return UPDATE_CONTINUE;
}



update_status ModuleScene::Update(float dt)
{
    //Draw modes
	if (scene_go != nullptr)
	{
		scene_go->Update();

	}
	plane.Render();

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	bool ret = true;
	
	if (scene_go != nullptr)
	{
		scene_go->CleanUp();
		RELEASE(scene_go);
	}
	
	return ret;
}
void ModuleScene::LoadModel(const char * path)
{
	LOG("Drag Model path:%s", path);

	
	if (App->importer->LoadModel(path))
	{
		LOG("Load Model Succes");
	}
	else
	{
		LOG("Load Model Fail");
	}

}

void ModuleScene::LoadTexture(const char * path)
{
	LOG("Drag Texture path:%s", path);


	if (scene_go != nullptr)
	{
		LOG("ERROR no model to overlay texture");

		//model->OverlayTexture(path);
	}
	else
	{
		LOG("ERROR no model to overlay texture");
	}
}

void ModuleScene::SendGameObject(GameObject * go)
{
	go->SetParent(scene_go);
}

void ModuleScene::SetSelectedGameObject(GameObject * go)
{
	selected_go = go;
}

