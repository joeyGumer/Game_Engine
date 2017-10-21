#pragma once
#include "Module.h"

class ModuleFileSystem:public Module
{
public:
	ModuleFileSystem();
	~ModuleFileSystem();
	bool Awake(const JSON_Object* data);
	bool CreateOwnFile(const char * name, char * buffer,int buffer_int, const char * directory,const char* extension);
	const char* GetMeshesFolder();
	const char* GetMaterialFolder();
	const char* GetAssetsFolder();

private:
	void CreateFolder(const char* name,  bool hide = false);
	std::string meshes;
	std::string materials;
	std::string assets;
};

