#include "Mesh.h"
#include "Application.h"

#define TEXTURE_SIZE 64
#define TEXTURE_SIZE_HOVER 128

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint> indices, std::vector<Texture> textures) :vertices(vertices), indices(indices), textures(textures)
{
	SetupMesh();
}



Mesh::~Mesh()
{
}

void Mesh::SetupMesh()
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

}

void Mesh::Draw()
{


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	
	for (int i = 0; i < textures.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		glColor4f(textures[i].rgba_color.x, textures[i].rgba_color.y, textures[i].rgba_color.z, textures[i].rgba_color.w);

	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), NULL);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normals));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	

	
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	
	//Draw normals Sonic Mode
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT)
	{

		for (int i = 0; i < vertices.size(); i++)
		{
			glBegin(GL_LINES);
			glVertex3f(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
			glVertex3f(vertices[i].normals.x + vertices[i].position.x, vertices[i].normals.y + vertices[i].position.y, vertices[i].normals.z + vertices[i].position.z);			
			glEnd();
		}
	}
	//Reset TextureColor
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);


}

void Mesh::OnGuiDraw()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::Text("Position");
		ImGui::Text("x %.2f y %.2f z %.2f", position.x, position.y, position.z);
		ImGui::Text("Rotation");
		ImGui::Text("x %.2f y %.2f z %.2f", rotation.GetEuler().x, rotation.GetEuler().y, rotation.GetEuler().z);
		ImGui::Text("Scale");
		ImGui::Text("x %.2f y %.2f z %.2f", scale.x, scale.y, scale.z);
		ImGui::Text("Texture");
			for (int i = 0; i < textures.size(); i++)
			{
				ImGui::Image((GLuint*)textures[i].id, ImVec2(TEXTURE_SIZE, TEXTURE_SIZE), ImVec2(0, 0), ImVec2(1, 1), *(ImVec4*)&textures[i].rgba_color);
				ImGui::PushItemWidth(200);
				ImGui::Text("Image RGBA");
				//ImGui::InputFloat4("##image_rgba", &textures[i].rgba_color[0],2);
				ImGui::ColorEdit4("##image_rgba", &textures[i].rgba_color.x);

				ImGui::PopItemWidth();
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Image((GLuint*)textures[i].id, ImVec2(TEXTURE_SIZE_HOVER, TEXTURE_SIZE_HOVER), ImVec2(0, 0), ImVec2(1, 1), *(ImVec4*)&textures[i].rgba_color);
					ImGui::EndTooltip();
				}
			}	
		ImGui::TreePop();
	}
}

void Mesh::SetInfo(aiNode* node)
{
	node->mTransformation.Decompose(scale, rotation, position);
	name = node->mName.C_Str();

}

const char * Mesh::GetName()
{
	return name.c_str();
}

const aiVector3D Mesh::GetPosition()
{
	return position;
}

const aiQuaternion Mesh::GetRotation()
{
	return rotation;
}

const aiVector3D Mesh::GetScale()
{
	return scale;
}

void Mesh::CleanUp()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	for (int i = 0; i < textures.size(); i++)
	{
		glDeleteTextures(1, &textures[i].id);
	}
	vertices.clear();
	indices.clear();
	textures.clear();
}