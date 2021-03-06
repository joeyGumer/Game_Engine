#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "imgui\imgui.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )
#define MIX_MAX_VOLUME 128
#define MIX_MIN_VOLUME 0

ModuleAudio::ModuleAudio(bool start_enabled) : music (NULL)
{
	name = "Audio";
}

// Destructor
ModuleAudio::~ModuleAudio()
{}
// Called before render is available

bool ModuleAudio::Awake(const JSONConfig& data)
{
	
		LOG("Loading Audio Mixer");
		bool ret = true;
		SDL_Init(0);
		if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
		{
			LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{

			music_volume = data.GetInt("music_volume");
			fx_volume = data.GetInt("fx_volume"); 

			// load support for the OGG format
			int flags = MIX_INIT_OGG;
			int init = Mix_Init(flags);

			if ((init & flags) != flags)
			{
				LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
				ret = false;
			}

			//Initialize SDL_mixer
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			{
				LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
				ret = false;
			}

			return true;
		}

		return ret;

}


// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}


	std::list<Mix_Chunk*>::iterator item = fx.begin();
	while (item != fx.end())
	{
		Mix_FreeChunk(item._Ptr->_Myval);
		item++;
	}
	fx.clear();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	return true;
}


void ModuleAudio::GuiConfigUpdate()
{
	if (ImGui::CollapsingHeader(name.c_str()))
	{

		ImGui::Text("Audio Driver:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", SDL_GetAudioDriver(audio_driver));
		ImGui::Separator();
		ImGui::Text("Audio Device:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", SDL_GetAudioDeviceName(audio_device, captured));
		ImGui::Separator();

		if (ImGui::SliderInt("Music Volume", &music_volume, MIX_MIN_VOLUME, MIX_MAX_VOLUME))
		{
			ChangeMusicVolume();

		}
		if (ImGui::SliderInt("FX Volume", &fx_volume, MIX_MIN_VOLUME, MIX_MAX_VOLUME))
		{
			ChangeFXVolume();
		}
	}
}

bool ModuleAudio::SaveConfig(JSONConfig& data)const
{
	data.SetInt(music_volume, "music_volume");
	data.SetInt(fx_volume, "fx_volume");
	return true;
}

bool ModuleAudio::LoadConfig(const JSONConfig& data)
{
	music_volume = data.GetInt("music_volume");
	fx_volume = data.GetInt("fx_volume");
	return true;

}

// Play a music file
bool ModuleAudio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic((int) (fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}
	Mix_VolumeMusic(music_volume);
	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int ModuleAudio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		Mix_VolumeChunk(chunk, fx_volume);
		ret = fx.size();
	}

	return ret;
}

// Play WAV
bool ModuleAudio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;
	uint k = 0;
	Mix_Chunk* chunk = NULL;
	
	if (id > 0 && id < fx.size())
	{

		std::list<Mix_Chunk*>::iterator item = fx.begin();
		while (k < id)
		{
			k++;
			item++;
		}

		Mix_PlayChannel(-1, item._Ptr->_Myval, repeat);
	}
	

	return ret;
}

const int ModuleAudio::GetMusicVolume()
{
	return music_volume;
}



void ModuleAudio::ChangeMusicVolume()
{
	Mix_VolumeMusic(music_volume);

}

void ModuleAudio::ChangeFXVolume()
{
	std::list<Mix_Chunk*>::iterator item = fx.begin();
	while (item != fx.end())
	{
		Mix_VolumeChunk(item._Ptr->_Myval, fx_volume);
		item++;
	}
}
