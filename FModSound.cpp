/*****************************************************************************/
/*
File Name:		FModSound.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [100%]

brief:			Implementation of FMOD taken from previous GAM150 project,
				which was also my work. Implemented as a class for GAM200.

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#pragma warning (disable : 26812) // unscoped enum in lib file
#include "stdafx.h"
#include "FModSound.h"
#include "FModSoundManager.h"

FModSound::FModSound() :
	soundOn{false},
	soundPlayable{ false },
	currentSound{ nullptr },
	crudeSoundIsPlaying{ false },
	givenFilepath{},
	result{ FMOD_RESULT::FMOD_OK },
	system{nullptr},
	sound{nullptr},
	channel{nullptr},
	volume{SOUNDMGR->GetMasterVolume()}
{
	//FMOD_System_CreateChannelGroup(system, NULL, &SOUNDMGR->channelGroup);
}

//copy constructor
FModSound::FModSound(FModSound& copy) :
	soundOn{ copy.soundOn },
	soundPlayable{copy.soundPlayable},
	currentSound{copy.currentSound},
	crudeSoundIsPlaying{ copy.crudeSoundIsPlaying },
	givenFilepath{copy.givenFilepath},
	result{copy.result},
	system{copy.system},
	sound{copy.sound},
	channel{copy.channel},
	volume{copy.volume}
{
}
//operator =
FModSound& FModSound::operator = (FModSound& copy)
{
	soundOn = copy.soundOn;
	soundPlayable = copy.soundPlayable;
	currentSound = copy.currentSound;
	crudeSoundIsPlaying = copy.crudeSoundIsPlaying;
	givenFilepath = copy.givenFilepath;
	result = copy.result;
	system = copy.system;
	sound = copy.sound;
	channel = copy.channel;
	volume = copy.volume;
	return *this;
}
//destructor
FModSound::~FModSound()
{
	soundOn = false;
	soundPlayable = false;
	currentSound = nullptr;
}

void FModSound::InitSound()
{
	soundPlayable = true;
	//create the sound system. If fails, sound is set to impossible
	if(system == nullptr)
		result = FMOD_System_Create(&system);
	
	if (result != FMOD_RESULT::FMOD_OK)
	{
		soundPlayable = false;
	}
	//if initialise the sound system. If fails, sound is set to impossible
	if (soundPlayable)
	{
		result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, 0);
	}
	if (result != FMOD_RESULT::FMOD_OK)
	{
		soundPlayable = false;
	}
}
//sets the volume of the sound. Always use after the playSound function
void FModSound::SetVolume(float _volume)
{
	if (soundPlayable && soundOn && _volume >= 0.0f && _volume <= 1.0f) {
		FMOD_Channel_SetVolume(channel, _volume);
	}
}

float FModSound::GetVolume() const
{
	return volume;
}

//loads the sound
void FModSound::LoadFile(const char* _filename)
{
	givenFilepath = _filename;
	//std::cout << "File path for sound = " << givenFilepath << std::endl;
	currentSound = const_cast<char*>(_filename);
	soundOn = true;
	soundPlayable = true;
	result = FMOD_Sound_Release(sound);
	result = FMOD_System_CreateStream(system, givenFilepath.c_str(), FMOD_DEFAULT, 0, &sound);
	
	//if failed to load, set sound to not possible
	if (result != FMOD_RESULT::FMOD_OK)
	{
		soundPlayable = false;
		std::cout << "FMOD ERROR" << static_cast<int>(result) << std::endl;
	}

	
}
//unloads the sound
void FModSound::UnloadFile(void)
{
	if (soundPlayable)
	{
		result = FMOD_Sound_Release(sound);
		result = FMOD_System_Release(system);
		
	}
}
//plays the current sound 
void FModSound::FModPlaySound(bool _pause)
{
	if (soundPlayable && soundOn) //if this throws, check the soundMap key (either json or in code)
	{
		result = FMOD_System_PlaySound(system, sound, nullptr, _pause, &channel);
		FMOD_Channel_SetVolume(channel, SOUNDMGR->GetMasterVolume());
	}
	crudeSoundIsPlaying = true;
}

void FModSound::FModPlaySoundPaused(bool _pause)
{
	if (soundPlayable && soundOn) {
		result = FMOD_System_PlaySound(system, sound, nullptr, _pause, &channel);
	}
	crudeSoundIsPlaying = false;
}
//plays the current sound in a infinite loop
void FModSound::PlaySoundLoop(bool _pause)
{
	if (soundPlayable && soundOn) {
		result = FMOD_System_PlaySound(system, sound, nullptr, _pause, &channel);
		FMOD_Channel_SetVolume(channel, SOUNDMGR->GetMasterVolume());
		FMOD_Channel_SetMode(channel, FMOD_LOOP_NORMAL);
	}
	crudeSoundIsPlaying = true;
}
//updates the sound
void FModSound::SoundUpdate()
{
	FMOD_System_Update(system);
}

//get current sound
bool FModSound::GetSound()
{
	return soundOn;
}

//pause the current sound
void FModSound::SetPause(bool _pause)
{
	FMOD_Channel_SetPaused(channel, _pause);
	crudeSoundIsPlaying = false;
}

//sets the current sound
void FModSound::SetSound(bool _sound)
{
	soundOn = _sound;
}

//toggles sound on or off
void FModSound::ToggleSound()
{
	soundOn = !soundOn;
	if (soundOn == true)
	{
		LoadFile(currentSound);
		FModPlaySound();
	}
	if (soundOn == false)
	{
		UnloadFile();
	}
}
//pause the current sound
void FModSound::TogglePause()
{
	FMOD_BOOL p;
	FMOD_Channel_GetPaused(channel, &p);
	FMOD_Channel_SetPaused(channel, !p);
	crudeSoundIsPlaying = false;
}

bool FModSound::SoundIsPlaying()
{
	FMOD_BOOL* isPlaying = nullptr;
	FMOD_Channel_IsPlaying(channel, isPlaying);
	return isPlaying;
}

bool FModSound::CrudeSoundIsPlaying()
{
	return crudeSoundIsPlaying;
}

//void FModSound::LoadChannelIntoChannelGroup()
//{
//	FModPlaySoundPaused();
//	result = FMOD_Channel_SetChannelGroup(channel, SOUNDMGR->channelGroup);
//}

std::string& FModSound::GetFilepath()
{
	return givenFilepath;
}
