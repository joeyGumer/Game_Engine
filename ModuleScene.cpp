#include "Application.h"
#include "Globals.h"
#include"ModuleScene.h"
#include "Primitive.h"


ModuleScene::ModuleScene(bool start_enabled)
{
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Start()
{
	bool ret = true;
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	//Vertex
	AABB cube;
	float3 pos(15, 2.5, 2.5);
	float3 size(5, 5, 5);
	float3 outpos[36] = {};
	cube.SetFromCenterAndSize(pos, size);
	cube.Triangulate(1, 1, 1, outpos, NULL, NULL, false);
	array_size = sizeof(outpos);
	glGenBuffers(1, &array_id);
	glBindBuffer(GL_ARRAY_BUFFER, array_id);
	glBufferData(GL_ARRAY_BUFFER, array_size, outpos, GL_STATIC_DRAW);

	//index
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
	
	//AABB cube1;
	//float3 pos = float3(1, 8, 1);
	//float3 size = float3(1, 1, 1);

	//cube1.SetFromCenterAndSize(pos, size);
	//float3 vertes[36];
	//cube1.Triangulate(1, 1, 1, vertes, NULL, NULL, NULL);
	//GLAllocateElement(vertes, sizeof(vertes));



	//AABB cube22;
	//float3 pos2 = float3(9, 8, 1);
	//float3 size2 = float3(1, 1, 1);
	//cube22.SetFromCenterAndSize(pos2, size2);
	//float3 vertes2[36];
	//cube22.Triangulate(1, 1, 1, vertes2, NULL, NULL, NULL);
	//GLAllocateElement(vertes2, sizeof(vertes2));

	///
	
	
	return ret;
}




update_status ModuleScene::Update(float dt)
{
    //Draw modes
	DrawElements();
	cubeDirectMode();
	CubeVertexMode();
	CubeIndexMode();

	ImGui::Begin("Colision Menu");

	static int element_1 = 0;
	//------------------Sphere_1------------
	ImGui::PushItemWidth(90);

	ImGui::RadioButton("Sphere", &element_1, 0);
	if (element_1 == 0)
	{
		ImGui::InputFloat("Sphere pos x", &sphere_x, 0.1f, 1.0f, 1); ImGui::SameLine();
		ImGui::InputFloat("Sphere pos y", &sphere_y, 0.1f, 1.0f, 1); ImGui::SameLine();
		ImGui::InputFloat("Sphere pos z", &sphere_z, 0.1f, 1.0f, 1);
		ImGui::InputFloat("Sphere radius", &sphere_radius, 0.1f, 1.0f, 1);
		if (ImGui::Button("Create##create_sphere"))
		{
			Sphere_p* item = new Sphere_p(sphere_radius);
			item->SetPos(sphere_x, sphere_y, sphere_z);
		}
	}
	ImGui::RadioButton("Cube", &element_1, 1);
	

	if (element_1 == 1)
	{
		ImGui::InputFloat("Cube pos x", &cube_pos_x, 0.1f, 1.0f, 1); ImGui::SameLine();
		ImGui::InputFloat("Cube pos y", &cube_pos_y, 0.1f, 1.0f, 1); ImGui::SameLine();
		ImGui::InputFloat("Cube pos z", &cube_pos_z, 0.1f, 1.0f, 1);

		ImGui::InputFloat("Cube size x", &cube_size_x, 0.1f, 1.0f, 1); ImGui::SameLine();
		ImGui::InputFloat("Cube size y", &cube_size_y, 0.1f, 1.0f, 1); ImGui::SameLine();
		ImGui::InputFloat("Cube size z", &cube_size_z, 0.1f, 1.0f, 1);
		if (ImGui::Button("Create##create_cube"))
		{
			Cube_p* item = new Cube_p(cube_size_x, cube_size_y, cube_size_z);
			item->SetPos(cube_pos_x, cube_pos_y, cube_pos_z);
		}
	}

	ImGui::End();
	
	plane.Render();

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	bool ret = true;

	return ret;
}

void ModuleScene::CubeVertexMode()
{

	//load buffer
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, array_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, array_size);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);

}

void ModuleScene::cubeDirectMode()
{
	//TODO
	glBegin(GL_TRIANGLES);

	//front
	glVertex3f(0.0f, 0.0f, 5.0f);
	glVertex3f(5.0f, 0.0f, 5.0f);
	glVertex3f(5.0f, 5.0f, 5.0f);

	glVertex3f(0.0f, 0.0f, 5.0f);
	glVertex3f(5.0f, 5.0f, 5.0f);
	glVertex3f(0.0f, 5.0f, 5.0f);

	//Right
	glVertex3f(5.0f, 0.0f, 5.0f);
	glVertex3f(5.0f, 0.0f, 0.0f);
	glVertex3f(5.0f, 5.0f, 5.0f);

	glVertex3f(5.0f, 0.0f, 0.0f);
	glVertex3f(5.0f, 5.0f, 0.0f);
	glVertex3f(5.0f, 5.0f, 5.0f);

	//Back
	glVertex3f(5.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 5.0f, 0.0f);
	glVertex3f(5.0f, 5.0f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 5.0f, 0.0f);
	glVertex3f(5.0f, 0.0f, 0.0f);

	//left
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 5.0f);
	glVertex3f(0.0f, 5.0f, 0.0f);

	glVertex3f(0.0f, 0.0f, 5.0f);
	glVertex3f(0.0f, 5.0f, 5.0f);
	glVertex3f(0.0f, 5.0f, 0.0f);

	//Up
	glVertex3f(5.0f, 5.0f, 5.0f);
	glVertex3f(5.0f, 5.0f, 0.0f);
	glVertex3f(0.0f, 5.0f, 0.0f);

	glVertex3f(5.0f, 5.0f, 5.0f);
	glVertex3f(0.0f, 5.0f, 0.0f);
	glVertex3f(0.0f, 5.0f, 5.0f);

	//bottom
	glVertex3f(5.0f, 0.0f, 5.0f);
	glVertex3f(0.0f, 0.0f, 5.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glVertex3f(5.0f, 0.0f, 5.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(5.0f, 0.0f, 0.0f);


	glEnd();
}

void ModuleScene::CubeIndexMode()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);
	
	glBindBuffer(GL_ARRAY_BUFFER, array_index_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_element_id);
	glDrawElements(GL_TRIANGLES, sizeof(GLushort) * 72, GL_UNSIGNED_SHORT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);
	glDisableClientState(GL_VERTEX_ARRAY);

}

void ModuleScene::GLAllocateElement(float3 * vertex, int size)
{
	GLuint element_id;

	float3* get_vertex = new float3[size];
	memcpy(get_vertex, vertex, size);
	glGenBuffers(1, &element_id);
	glBindBuffer(GL_ARRAY_BUFFER, element_id);
	glBufferData(GL_ARRAY_BUFFER, size, get_vertex, GL_STATIC_DRAW);
	elements.push_back(new PGeometry(element_id, get_vertex, size));
	delete[] get_vertex;
}

void ModuleScene::DrawElements()
{
	std::list<PGeometry*>::iterator item = elements.begin();
	while (item != elements.end())
	{

		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, item._Ptr->_Myval->buffer_id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawArrays(GL_TRIANGLES, 0, item._Ptr->_Myval->size);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		item++;


	
	}

}

PGeometry::PGeometry(GLuint buffer_id, float3* _vertex, int size) :buffer_id(buffer_id), size(size)
{
	vertex = new float3[size];
	memcpy(vertex, _vertex, size);
}

PGeometry::~PGeometry()
{
	delete[] vertex;
}

