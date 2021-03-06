#include "ModuleResourceManager.h"
#include "Application.h"
#include "ModuleEditorWindows.h"
#include "UI_Folder.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourcePrefab.h"
#include "ResourceFont.h"
#include "ModuleFileSystem.h"
#include <vector>
#include <experimental\filesystem>
#include "imgui/imgui.h"
#define UPDATE_RESOURCE_TIME 2.0f
ModuleResourceManager::ModuleResourceManager()
{
	name = "Resource Manager";
}


ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Start()
{
	App->editor_window->FillFiles(files,F_META);
	for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); it++)
	{
		LoadMetaResource((*it).c_str());
	}
	files.clear();

	return true;
}

update_status ModuleResourceManager::PreUpdate(float dt)
{
	if (need_to_delete)
	{
		DeleteResources();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::Update(float dt)
{

	time_update += dt;
	if (time_update > UPDATE_RESOURCE_TIME)
	{
		LookForResources();
	}
	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	std::map<uint, Resource*>::iterator it = resources.begin();
	while (it != resources.end())
	{
		Resource* item = it->second;
		it=resources.erase(it);
		item->CleanUp();
		RELEASE(item);
	}
	files.clear();
	resources.clear();
	return true;
}
void ModuleResourceManager::LookForResources()
{

	time_update = 0.0f;
	App->editor_window->FillFiles(files);
	std::map<uint, Resource*>::const_iterator it = resources.begin();
	for (; it != resources.end(); it++)	
	{
		if (!App->file_system->FileExist(it->second->GetOriginalFile().c_str()))
		{

			it->second->ToDelete();
			need_to_delete = true;
		}
		else
		{
			char* buffer = nullptr;
			bool same = true;
			same=App->file_system->CompareDates(it->second->GetOriginalFile().c_str(), it->second->GetCreationTime().c_str(), &buffer);
			if (!same)
			{
				ReImport(it->second);
			}
			if(buffer!=nullptr)
			RELEASE_ARRAY(buffer);

		}
		
	}
	for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); it++)
	{
	
		if (Find((*it).c_str()) == 0)
		{
			ImportFile((*it).c_str());
		}
	}
	files.clear();

}

uint ModuleResourceManager::Find(const char * asset_file) const
{
	std::map<uint, Resource*>::const_iterator it = resources.begin();
	for (; it != resources.end(); it++)
	{
		
		if (it->second->GetDelete()) {
			continue;
		}
		if (std::experimental::filesystem::equivalent(it->second->GetOriginalFile(), asset_file))
		{
			//if time of creation
			return it->second->GetUID();
		}
	}
	return 0;
}

uint ModuleResourceManager::ImportFile(const char * new_asset_file)
{
	std::string asset = new_asset_file;
	std::size_t found = asset.find_last_of('.');
	bool import_success = false;
	ResourceType type = GetResourceFromFile(new_asset_file);
	uint UID = App->GenerateRandom();
	std::string name = std::to_string(UID);
	switch (type)
	{
	case R_NONE:
		break;
	case R_TEXTURE:
		import_success=App->importer->ImportTexture(new_asset_file, name.c_str());
		
		break;
	case R_PREFAB:
		import_success = App->importer->ImportMesh(new_asset_file, name.c_str());
	case R_FONT:
		import_success = App->importer->ImportFont(new_asset_file, name.c_str());

		break;
	default:
		break;
	}

	if (import_success)
	{
		Resource* ret = CreateResource(type, UID);
		char* date=nullptr;		
		std::string original_file = new_asset_file;
		std::string file_name = App->file_system->ExtractFileName(original_file.c_str());

		App->file_system->CreationTime(original_file.c_str(), &date);
		ret->SetDateOfCreation(date);
		ret->SetOriginalFile(original_file.c_str());
		ret->SetMetaFile(original_file.c_str());
		ret->SetOriginalName(file_name.c_str());
		switch (type)
		{
		case R_NONE:
			break;
		case R_TEXTURE:
			ret->SetLibraryFile(name.c_str(),"dds");
			break;
		case R_PREFAB:
			ret->SetLibraryFile(name.c_str(), "json");
		case R_FONT:
			ret->SetLibraryFile(name.c_str(), "ttf");
			break;
		default:
			break;
		}


		JSONConfig config;
		ret->SaveResource(config);
		char* buffer = nullptr;
		uint size = config.Serialize(&buffer);
		config.Save(ret->GetMetaJsonFile().c_str());

		RELEASE_ARRAY(date);
		RELEASE_ARRAY(buffer);


		return ret->GetUID();

	}
	return 0;
}

bool ModuleResourceManager::ReImport(Resource* resource)
{
	bool ret = App->file_system->RemoveFile(resource->GetLibraryFile().c_str());
	if (!ret)
	{
		return ret;
	}
	uint UID = resource->GetUID();
	std::string name = std::to_string(UID);
	switch (resource->GetResourceType())
	{
	case R_NONE:
		break;
	case R_TEXTURE:
		ret=App->importer->ImportTexture(resource->GetOriginalFile().c_str(), name.c_str());
		break;

	case R_PREFAB:
		break;
	default:
		break;
	}
	if (ret)
	{
		char* date = nullptr;
		App->file_system->CreationTime(resource->GetOriginalFile().c_str(), &date);
		resource->SetDateOfCreation(date);
		resource->ReImport();
		RELEASE_ARRAY(date);
		
	}
}



uint ModuleResourceManager::ResourceWindows(ResourceType type)
{
	uint ret = 0;
	ImGui::OpenPopup("Select Resource");
	if (ImGui::BeginPopupModal("Select Resource", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static int uid = 0;
		std::map<uint, Resource*>::const_iterator it = resources.begin();

		for (; it != resources.end(); it++)
		{
			if (it->second->GetResourceType() == type)
			{
				ImGuiWindowFlags tree_flags = 0;

				if (uid == it->first)
				{
					tree_flags |= ImGuiTreeNodeFlags_Selected;
				}
				ImGui::TreeNodeEx(it->second->GetOriginalFile().c_str() , tree_flags| ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
				if (ImGui::IsItemClicked())
				{
					uid = it->first;

				}
			}
		}	
		ImGui::Separator();
		if (ImGui::Button("Select##resource_select", ImVec2(120, 0)))
		{
			ret = uid;
			ImGui::CloseCurrentPopup();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel", ImVec2(120, 0)))
	{
		ret = -1;
		ImGui::CloseCurrentPopup();
	}

	ImGui::EndPopup();

	return ret;
}

ResourceType ModuleResourceManager::GetResourceFromFile(const char * file)const
{
	std::string extension_check = file;
	std::size_t found = extension_check.find_last_of('.');
	std::string extension = extension_check.substr(found + 1);
	if (_stricmp(extension.c_str(), "png") == 0
		|| _stricmp(extension.c_str(), "jpg") == 0
		|| _stricmp(extension.c_str(), "dds") == 0
		|| _stricmp(extension.c_str(), "tga") == 0)
	{
		return R_TEXTURE;
	}
	else if (_stricmp(extension.c_str(), "obj") == 0
		|| _stricmp(extension.c_str(), "fbx") == 0)
	{
		return R_PREFAB;

	}
	else if (_stricmp(extension.c_str(), "TTF") == 0)
	{
		return R_FONT;

	}
	else
	{
		return R_NONE;
	}
}

const Resource * ModuleResourceManager::Get(uint UID) const
{
	std::map<uint, Resource*>::const_iterator it = resources.find(UID);
	if (it != resources.end())
		return it->second;
	return nullptr;
}

Resource * ModuleResourceManager::Get(uint UID)
{
	std::map<uint, Resource*>::iterator it = resources.find(UID);
	if (it != resources.end())
		return it->second;
	return nullptr;
}

Resource * ModuleResourceManager::CreateResource(ResourceType type, uint custom_UID)
{
	Resource* ret = nullptr;
	uint UID = (custom_UID == -1) ? App->GenerateRandom() : custom_UID;
	switch (type)
	{
	case R_NONE:
		break;
	case R_TEXTURE:
		ret = new ResourceTexture(UID);
		break;
	case R_MESH:
		ret = new ResourceMesh(UID);
		break;
	case R_PREFAB:
		ret = new ResourcePrefab(UID);
		break;
	case R_FONT:
		ret = new ResourceFont(UID);
		break;
	default:
		break;
	}
	if (ret != nullptr)
	{
		resources[UID] = ret;
	}
	return ret;
}

void ModuleResourceManager::DeleteResources()
{
	App->editor_window->CleanInspector();
	std::map<uint, Resource*>::iterator it = resources.begin();

	while (it != resources.end())
	{
		if (it->second->GetDelete())
		{
			Resource* tmp = it->second;
			switch (tmp->GetResourceType())
			{
			case R_NONE:
				break;
			case R_TEXTURE:
				App->file_system->RemoveFile(tmp->GetLibraryFile().c_str(), App->file_system->GetMaterialFolder());
				break;
			case R_MESH:
				App->file_system->RemoveFile(tmp->GetLibraryFile().c_str(), App->file_system->GetMeshesFolder());
				break;
			case R_PREFAB:
				App->file_system->RemoveFile(tmp->GetLibraryFile().c_str(), App->file_system->GetMeshesFolder());
				break;
			default:
				break;
			}
			App->file_system->RemoveFile(tmp->GetMetaJsonFile().c_str());
			it=resources.erase(it);
			RELEASE(tmp);
		}
		else
		{
			it++;
		}
	}
	need_to_delete = false;
}

void ModuleResourceManager::SaveMetaResource(const char* path)
{

}

void ModuleResourceManager::LoadMetaResource(const char* path)
{
		JSONConfig config;
		if (!config.ParseFile(path))
			return;
		Resource*resource = nullptr;
		resource = CreateResource(static_cast<ResourceType>(config.GetInt("Resource Type")), config.GetInt("Resource UID"));
		resource->LoadResource(config);
		config.CleanUp();
}
