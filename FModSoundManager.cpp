/*****************************************************************************/
/*
File Name:		FModSoundManager.cpp
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [95%]
				Kok Zhe Yan			 [5%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/

#include "stdafx.h"
#include "FModSoundManager.h"

std::shared_ptr<FModSoundManager> SOUNDMGR;

FModSoundManager::FModSoundManager() :	
	soundMap{}, 
	masterVolume{ 0.5f },
	lastVolume{ 0.0f },
	system{nullptr}
{
}

FModSoundManager::~FModSoundManager()
{
}

void FModSoundManager::SendMessage(Message& _message)
{
	UNREFERENCED_PARAMETER(_message);

}

//gettor for the soundMap
std::map< std::string, std::shared_ptr<FModSoundCPP>>& FModSoundManager::GetMap()
//std::map< std::string, std::shared_ptr<FModSound>>& FModSoundManager::GetMap()
{
	return soundMap;
}


int FModSoundManager::ErrorCheck(FMOD_RESULT _result)
{
	if (_result != FMOD_RESULT::FMOD_OK)
	{
		//std::cout << "FMOD ERROR" << static_cast<int>(_result) << std::endl;
		return 1;
	}
	//std::cout << "FMOD Result " << static_cast<int>(_result) << std::endl;
	return 0;
}

void FModSoundManager::Initialize()
{
	//std::cout << "Sound Manager Init" << std::endl;
	if (system == nullptr)
	{
		ErrorCheck(FMOD::System_Create(&system));
		ErrorCheck(system->init(32, FMOD_INIT_NORMAL, nullptr));
	}
}

//iterates through the sounds, runs the update function for each sound, and sets the volume constantly.
void FModSoundManager::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	system->update();

	for (auto [name, sound] : soundMap)
	{
		//sound->SoundUpdate();
		if(sound->GetVolume() != masterVolume)
			sound->SetVolume(masterVolume);
	}
}

void FModSoundManager::Clear()
{
	//soundMap.clear();
}

std::string FModSoundManager::GetName()
{
	return "Sound Manager";
}

void FModSoundManager::PlaySoundFromMap(const std::string& _key)
{
	//soundMap[_key]->FModPlaySound();
	system->playSound(soundMap[_key]->sound, nullptr, false, &soundMap[_key]->channel);
	soundMap[_key]->channel->setVolume(masterVolume);
}

void FModSoundManager::PlaySoundLoopFromMap(const std::string& _key)
{
	//soundMap[_key]->PlaySoundLoop();
	if (soundMap.find(_key) == std::end(soundMap))
		return;

	system->playSound(soundMap[_key]->sound, nullptr, false, &soundMap[_key]->channel);
	soundMap[_key]->channel->setVolume(SOUNDMGR->GetMasterVolume());
	soundMap[_key]->channel->setMode(FMOD_LOOP_NORMAL);
}

void FModSoundManager::PauseSoundFromMap(const std::string& _key)
{
	soundMap[_key]->SetPause(true);
}

void FModSoundManager::UnPauseSoundFromMap(const std::string& _key)
{
	soundMap[_key]->SetPause(false);
}

void FModSoundManager::UnloadSoundFromMap(const std::string& _key)
{
	soundMap[_key]->UnloadFile();
}

void FModSoundManager::AddToMap(const std::string& _key, std::shared_ptr<FModSoundCPP> _soundptr)
//void FModSoundManager::AddToMap(const std::string& _key, std::shared_ptr<FModSound> _soundptr)
{
	soundMap[_key] = _soundptr;
}

float FModSoundManager::GetMasterVolume() const
{
	return masterVolume;
}

void FModSoundManager::SetMasterVolume(float _volume)
{
	if (_volume >= 0.0f && _volume <= 1.0f)
		masterVolume = _volume;
	else if (_volume < 0.0f)
		masterVolume = 0.0f;
	else if (_volume > 1.0f)
		masterVolume = 1.0f;
}

void FModSoundManager::MuteMasterVolume(bool _b)
{
	if (_b)
	{
		lastVolume = masterVolume;
		masterVolume = 0.0f;
	}
	else
	{
		masterVolume = 0.6f;
	}
}

void FModSoundManager::DecrementMasterVolume(float _decrement)
{
	SetMasterVolume(masterVolume - _decrement);
}

void FModSoundManager::IncrementMasterVolume(float _increment)
{
	SetMasterVolume(masterVolume + _increment);
}

void FModSoundManager::PauseAllSounds()
{
	for (auto [name, sound] : soundMap)
		sound->SetPause(true);
}

void FModSoundManager::UnPauseAllSounds()
{
	for (auto [name, sound] : soundMap)
		if(!sound->getForcePause())
			sound->SetPause(false);
}

bool FModSoundManager::GetPauseStatus(const std::string& _key)
{
	return soundMap[_key]->GetPause();
}

bool FModSoundManager::getForcePause(const std::string& _key)
{
	return soundMap[_key]->getForcePause();
}

void FModSoundManager::setForcePause(const std::string& _key, bool _b)
{
	soundMap[_key]->setForcePause(_b);
}

void FModSoundManager::resetAllForcePause()
{
	for (auto [name, sound] : soundMap)
		sound->setForcePause(false);
}

void FModSoundManager::LoadSound(const std::string& _keyname, const std::string& _filename)
{
	if (strcmp(_filename.substr(_filename.find_last_of(".") + 1, std::string::npos).c_str(), "ogg") != 0)
	{
		std::string errormsg = _filename + " is not in .ogg format";
		MessageBoxA(NULL, errormsg.c_str(), "Error", MB_OK);
		return;
	}

	if (soundMap[_keyname] == nullptr)
	{
		/////////////////////load sounds
		std::shared_ptr<FModSoundCPP> testSound = std::make_shared<FModSoundCPP>();
		//if failed to load, set sound to not possible
		if (ErrorCheck(system->createStream(_filename.c_str(), FMOD_LOOP_OFF | FMOD_2D, 0, &testSound->sound ) ))
		{
			std::cout << "loadFile error: " << _filename << std::endl;
		}
		/////////////////////
		testSound->SetFilepath(_filename);

		soundMap[_keyname] = testSound;
	}
}



