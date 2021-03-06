#include "ResourceTexture.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "Glew\include\GL\glew.h"
#include "imgui/imgui.h"
#include "p2Defs.h"
ResourceTexture::ResourceTexture(uint UID):Resource(UID,ResourceType::R_TEXTURE), rgba_color (float4(1.0f,1.0f,1.0f,1.0f))
{
}


ResourceTexture::~ResourceTexture()
{
}

void ResourceTexture::SetID(uint set_id)
{
	id = set_id;
}

void ResourceTexture::SetWidth(uint width)
{
	this->width = width;
}

void ResourceTexture::SetHeight(uint height)
{
	this->height = height;
}

void ResourceTexture::LoadInMemory()
{
	if (IsLoadInMemory())
	{
		Load();
	}
	load_count++;
}

void ResourceTexture::Load()
{
	App->importer->LoadTexture(this);

}

void ResourceTexture::UnLoadInMemory()
{
	if(load_count>0)
	load_count--;
	if (IsLoadInMemory())
	{
		UnLoad();
	}
}

void ResourceTexture::UnLoad()
{
	glDeleteTextures(1, &id);
	id = -1;

}

void ResourceTexture::InspectorUpdate()
{
	//Change name
	ImGui::Text("ResourceTexture: %s", original_file.c_str());
	ImGui::Text("Load in memory: %i", load_count);
	ImGui::Text("UID: %i", UID);


	ImGui::Text("ID: %i", id);

	ImGui::Checkbox("Flip textrue##flip_texture", &flip);


}

void ResourceTexture::CleanUp()
{
	if(id!=-1)
	UnLoad();
}

void ResourceTexture::ReImport()
{
	JSONConfig config;
	SaveResource(config);
	char* buffer = nullptr;
	uint size = config.Serialize(&buffer);
	config.Save(meta_file.c_str());
	RELEASE_ARRAY(buffer);

	if (!IsLoadInMemory())
	{
		UnLoad();
		Load();
	}
	
}

void ResourceTexture::SaveResource(JSONConfig & config) const
{
	config.SetInt(type, "Resource Type");
	config.SetInt(UID, "Resource UID");
	config.SetString(creation_time, "File Creation");

	config.SetString(original_file, "Original File");
	config.SetString(library_file, "Library File");
	config.SetString(meta_file, "Meta File");

	config.SetBool(flip, "Flip Image");

}

void ResourceTexture::LoadResource(const JSONConfig & config)
{

	creation_time = config.GetString("File Creation");

	original_file= config.GetString("Original File");
	library_file = config.GetString("Library File");
	meta_file = config.GetString("Meta File");
	flip = config.GetBool("Flip Image");
}

uint ResourceTexture::GetID() const
{
	return id;
}



float4 ResourceTexture::GetRGBA() const
{
	return rgba_color;
}

bool ResourceTexture::GetFlip() const
{
	return flip;
}

uint ResourceTexture::GetHeight() const
{
	return height;
}

uint ResourceTexture::GetWidth() const
{
	return width;
}

void ResourceTexture::SetRGBA(float4 rgba)
{
	rgba_color = rgba;
}


