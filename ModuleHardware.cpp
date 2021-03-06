#include "Globals.h"
#include "Application.h"
#include "ModuleHardware.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "imgui\imgui.h"

ModuleHardware::ModuleHardware(bool start_enabled)
{
	name = "Hardware";
}

ModuleHardware::~ModuleHardware()
{
}



bool ModuleHardware::Start()
{
	bool ret = true;
	SDL_GetVersion(&sdl_vers);
	cpu_numbers = SDL_GetCPUCount();
	cpu_cache = SDL_GetCPUCacheLineSize();
	system_ram = SDL_GetSystemRAM() / 1024;

	return ret;
}

bool ModuleHardware::CleanUp()
{
	bool ret = true;
	return ret;
}



void ModuleHardware::GuiConfigUpdate()
{
	if (ImGui::CollapsingHeader(name.c_str()))
	{
		ImGui::Text("SDL Version:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i.%i.%i", sdl_vers.major, sdl_vers.minor, sdl_vers.patch);
		ImGui::Separator();
		ImGui::Text("CPU's:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i, (Cache: %ikb)", cpu_numbers, cpu_cache);
		ImGui::Text("System RAM:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i Gb", system_ram);
		ImGui::Separator();
		ImGui::Text("GPU Vendor:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_VENDOR));
		ImGui::Text("GPU:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_RENDERER));
		ImGui::Text("GPU Version:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_VERSION));
		ImGui::Text("OpenGL Version:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glGetString(GL_SHADING_LANGUAGE_VERSION));
		ImGui::Text("Glew Version:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", glewGetString(GLEW_VERSION));

		
	}
}
