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
	
	
	//index
	/*
	GLfloat cube2[] =
	{
		8.0, -1.0, 7.0,
		10.0,-1.0, 7.0,
		10.0, 1.0, 7.0,
		8.0,  1.0, 7.0,
		// back
		8.0, -1.0, 5.0,
		10.0,-1.0, 5.0,
		10.0, 1.0, 5.0,
		8.0,  1.0, 5.0,
	};
	array_index_size = sizeof(cube2);

	glGenBuffers(1, &array_index_id);
	glBindBuffer(GL_ARRAY_BUFFER, array_index_id);
	glBufferData(GL_ARRAY_BUFFER, array_index_size, cube2, GL_STATIC_DRAW);
	

	GLushort cube_id[] =
	{
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3,
	};

	buffer_element_size = sizeof(cube_id);
	glGenBuffers(1, &buffer_element_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_element_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_element_size, cube_id, GL_STATIC_DRAW);

	*/
	

	
	lenna = App->texture->LoadTextureFromFile("Lenna.png");
	return ret;
}

update_status ModuleScene::GuiUpdate()
{
	
	//ImGui::Begin("Colision Menu");
	//Primitive_p primitive;

	//static int element_1 = 0;

	//ImGui::PushItemWidth(90);
	////------------------Sphere_1------------
	
	//ImGui::RadioButton("Sphere", &element_1, 0);
	//if (element_1 == 0)
	//{
	//	ImGui::InputFloat("Sphere pos x", &sphere_x, 0.1f, 1.0f, 1); ImGui::SameLine();
	//	ImGui::InputFloat("Sphere pos y", &sphere_y, 0.1f, 1.0f, 1); ImGui::SameLine();
	//	ImGui::InputFloat("Sphere pos z", &sphere_z, 0.1f, 1.0f, 1);
	//	ImGui::InputFloat("Sphere radius", &sphere_radius, 0.1f, 1.0f, 1);
	//	if (ImGui::Button("Create##create_sphere"))
	//	{

	//	}
	//}
	//ImGui::RadioButton("Cube", &element_1, 1);


	//if (element_1 == 1)
	//{
	//	ImGui::InputFloat("Cube pos x", &cube_pos_x, 0.1f, 1.0f, 1); ImGui::SameLine();
	//	ImGui::InputFloat("Cube pos y", &cube_pos_y, 0.1f, 1.0f, 1); ImGui::SameLine();
	//	ImGui::InputFloat("Cube pos z", &cube_pos_z, 0.1f, 1.0f, 1);

	//	ImGui::InputFloat("Cube size x", &cube_size, 0.1f, 1.0f, 1); ImGui::SameLine();
	//	if (ImGui::Button("Create##create_cube"))
	//	{
	//		primitive.CreateCube(vertex_cube,float3(cube_pos_x, cube_pos_y, cube_pos_z), cube_size);
	//	}
	//}

	//ImGui::End();
	

	return UPDATE_CONTINUE;
}
void ModuleScene::cubeDirectMode()
{

	//TODO

	glBindTexture(GL_TEXTURE_2D, lenna);
	glBegin(GL_TRIANGLES);
	//front
	glTexCoord2d(0.0f, 0.0f);	glVertex3f(0.0f, 0.0f, 5.0f);
	glTexCoord2d(0.0f, 1.0f);	glVertex3f(5.0f, 0.0f, 5.0f);
	glTexCoord2d(1.0f, 1.0f);	glVertex3f(5.0f, 5.0f, 5.0f);

	glTexCoord2d(0.0f, 0.0f);	glVertex3f(0.0f, 0.0f, 5.0f);
	glTexCoord2d(1.0f, 1.0f);	glVertex3f(5.0f, 5.0f, 5.0f);
	glTexCoord2d(1.0f,	0.0f);	glVertex3f(0.0f, 5.0f, 5.0f);

	//Right
	glTexCoord2d(0.0f, 0.0f);	glVertex3f(5.0f, 0.0f, 5.0f);
	glTexCoord2d(0.0f, 1.0f);	glVertex3f(5.0f, 0.0f, 0.0f);
	glTexCoord2d(1.0f, 0.0f);	glVertex3f(5.0f, 5.0f, 5.0f);

	glTexCoord2d(0.0f, 1.0f);	glVertex3f(5.0f, 0.0f, 0.0f);
	glTexCoord2d(1.0f, 1.0f);	glVertex3f(5.0f, 5.0f, 0.0f);
	glTexCoord2d(1.0f, 0.0f);	glVertex3f(5.0f, 5.0f, 5.0f);

	//Back
	glTexCoord2d(0.0f, 0.0f);	glVertex3f(5.0f, 0.0f, 0.0f);
	glTexCoord2d(1.0f, 1.0f);	glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2d(1.0f, 0.0f);	glVertex3f(5.0f, 5.0f, 0.0f);

	glTexCoord2d(0.0f, 1.0f);	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2d(1.0f, 1.0f);	glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2d(0.0f, 0.0f);	glVertex3f(5.0f, 0.0f, 0.0f);

	//left
	glTexCoord2d(0.0f, 0.0f);	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2d(0.0f, 1.0f);	glVertex3f(0.0f, 0.0f, 5.0f);
	glTexCoord2d(1.0f, 0.0f);	glVertex3f(0.0f, 5.0f, 0.0f);

	glTexCoord2d(0.0f, 1.0f);	glVertex3f(0.0f, 0.0f, 5.0f);
	glTexCoord2d(1.0f, 1.0f);	glVertex3f(0.0f, 5.0f, 5.0f);
	glTexCoord2d(1.0f, 0.0f);	glVertex3f(0.0f, 5.0f, 0.0f);

	//Up
	glTexCoord2d(0.0f, 1.0f);	glVertex3f(5.0f, 5.0f, 5.0f);
	glTexCoord2d(0.0f, 0.0f);	glVertex3f(5.0f, 5.0f, 0.0f);
	glTexCoord2d(1.0f, 0.0f);	glVertex3f(0.0f, 5.0f, 0.0f);

	glTexCoord2d(0.0f, 1.0f);	glVertex3f(5.0f, 5.0f, 5.0f);
	glTexCoord2d(1.0f, 0.0f);	glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2d(1.0f, 1.0f);	glVertex3f(0.0f, 5.0f, 5.0f);

	//bottom
	glTexCoord2d(1.0f, 1.0f);	glVertex3f(5.0f, 0.0f, 5.0f);
	glTexCoord2d(0.0f, 1.0f);	glVertex3f(0.0f, 0.0f, 5.0f);
	glTexCoord2d(0.0f, 0.0f);	glVertex3f(0.0f, 0.0f, 0.0f);

	glTexCoord2d(1.0f, 1.0f);	glVertex3f(5.0f, 0.0f, 5.0f);
	glTexCoord2d(0.0f, 0.0f);	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2d(1.0f, 0.0f);	glVertex3f(5.0f, 0.0f, 0.0f);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

}
void ModuleScene::CubeIndexMode()
{/*
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);


	glBindBuffer(GL_ARRAY_BUFFER, array_index_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_element_id);
	glDrawElements(GL_TRIANGLES, sizeof(GLushort) * 72 / 3, GL_UNSIGNED_SHORT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);
	glDisableClientState(GL_VERTEX_ARRAY);
	*/
}


update_status ModuleScene::Update(float dt)
{
    //Draw modes
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->Draw();
		ImGui::Begin("Colision Menu");
		models[i]->OnGuiDraw();
		ImGui::End();
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		if (models.size() > 0)
		{
			AABB cube_model;
			cube_model.SetNegativeInfinity();
			for (int i = 0; i < models[0]->meshes.size(); i++)
			{
				for (int x = 0; x < models[0]->meshes[i].vertices.size(); x++)
				{
					cube_model.Enclose(models[0]->meshes[i].vertices[x].position);
				}
			}
			float3 center = cube_model.CenterPoint();
			vec3 tmp(center.x, center.y, center.z);
			LOG("Focus Mesh");
			App->camera->Focus(tmp);
		}
	}

	cubeDirectMode();
	CubeIndexMode();
	plane.Render();

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	bool ret = true;

	return ret;
}
void ModuleScene::LoadModel(const char * path)
{
	LOG("Drag Model path:%s", path);
	models.push_back(new Model(path));
}

void ModuleScene::LoadTexture(const char * path)
{
	LOG("Drag Texture path:%s", path);
}
