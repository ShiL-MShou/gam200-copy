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
#include "FModSoundCPP.h"
#include "FModSoundManager.h"

FModSoundCPP::FModSoundCPP() :
	soundOn{ false },
	soundPlayable{ false },
	currentSound{ nullptr },
	forcePause{ false },
	givenFilepath{},
	system{nullptr},
	sound{ nullptr },
	channel{ nullptr },
	volume{ SOUNDMGR->GetMasterVolume() }
{
	//system = nullptr;
	//FMOD_System_CreateChannelGroup(system, NULL, &SOUNDMGR->channelGroup);
}

////copy constructor
//FModSoundCPP::FModSoundCPP(FModSoundCPP& copy) :
//	soundOn{ copy.soundOn },
//	soundPlayable{ copy.soundPlayable },
//	currentSound{ copy.currentSound },
//	crudeSoundIsPlaying{ copy.crudeSoundIsPlaying },
//	givenFilepath{ copy.givenFilepath },
//	sound{ copy.sound },
//	channel{ copy.channel },
//	volume{ copy.volume }
//{
//}
////operator =
//FModSoundCPP& FModSoundCPP::operator = (FModSoundCPP& copy)
//{
//	soundOn = copy.soundOn;
//	soundPlayable = copy.soundPlayable;
//	currentSound = copy.currentSound;
//	crudeSoundIsPlaying = copy.crudeSoundIsPlaying;
//	givenFilepath = copy.givenFilepath;
//	sound = copy.sound;
//	channel = copy.channel;
//	volume = copy.volume;
//	return *this;
//}

//destructor
FModSoundCPP::~FModSoundCPP()
{
	soundOn = false;
	soundPlayable = false;
	currentSound = nullptr;
}

//returns 1 if error, 0 if all good
int FModSoundCPP::ErrorCheck(FMOD_RESULT _result)
{
	if (_result != FMOD_RESULT::FMOD_OK)
	{
		std::cout << "FMOD ERROR" << static_cast<int>(_result) << std::endl;
		return 1;
	}

	return 0;
}

void FModSoundCPP::InitSound()
{
	soundPlayable = true;
	//create the sound system. If fails, sound is set to impossible
	if (system == nullptr)
	{
		
		if (ErrorCheck(FMOD::System_Create(&system)))
		{
			soundPlayable = false;
			std::cout << "didnt system_create" << std::endl;
		}
		if (soundPlayable)
		{

			if (ErrorCheck(system->init(32, FMOD_INIT_NORMAL, nullptr)))
			{
				soundPlayable = false;
			}
		}
	}
}

//sets the volume of the sound. Always use after the playSound function
void FModSoundCPP::SetVolume(float _volume)
{
	if ( _volume >= 0.0f && _volume <= 1.0f) 
	{
		// FMOD_Channel_SetVolume(channel, _volume);
		channel->setVolume(_volume);
	}
}

float FModSoundCPP::GetVolume()
{
	float* vol = new float;
	channel->getVolume(vol);
	volume = *vol;
	delete vol;
	return volume;
}

//loads the sound
void FModSoundCPP::LoadFile(const char* _filename)
{
	givenFilepath = _filename;
	//std::cout << "File path for sound = " << givenFilepath << std::endl;
	currentSound = const_cast<char*>(_filename);
	soundOn = true;
	soundPlayable = true;

	if(sound)
		sound->release();
	//system->createStream(givenFilepath.c_str(), FMOD_LOOP_OFF | FMOD_2D, 0, &sound);
	
	
	//if failed to load, set sound to not possible
	if (ErrorCheck(system->createStream(givenFilepath.c_str(), FMOD_LOOP_OFF | FMOD_2D, 0, &sound)))
	{
		soundPlayable = false;
		std::cout << "loadFile error: "  << _filename << std::endl;
	}

}

//unloads the sound
void FModSoundCPP::UnloadFile(void)
{
	if (soundPlayable)
	{
		sound->release();
	}
}

void FModSoundCPP::FModPlaySoundPaused(bool _pause)
{
	if (soundPlayable && soundOn) {
		system->playSound(sound, nullptr, _pause, &channel);
		//result = FMOD_System_PlaySound(system, sound, nullptr, _pause, &channel);
	}
}


//updates the sound
void FModSoundCPP::SoundUpdate()
{
	system->update();
}

//pause the current sound
void FModSoundCPP::SetPause(bool _pause)
{
	channel->setPaused(_pause);
}

bool FModSoundCPP::GetPause()
{
	bool b;
	channel->getPaused(&b);
	return b;
}

//sets the current sound
void FModSoundCPP::SetSound(bool _sound)
{
	soundOn = _sound;
}

//toggles sound on or off
void FModSoundCPP::ToggleSound()
{
	soundOn = !soundOn;
	if (soundOn == true)
	{
		LoadFile(currentSound);
		//FModPlaySound();
	}
	if (soundOn == false)
	{
		UnloadFile();
	}
}
//pause the current sound
void FModSoundCPP::TogglePause()
{
	bool p;
	channel->getPaused(&p);
	channel->setPaused(!p);
	//FMOD_Channel_GetPaused(channel, &p);
	//FMOD_Channel_SetPaused(channel, !p);
	forcePause = false;
}

bool FModSoundCPP::SoundIsPlaying()
{
	bool* isPlaying = nullptr;
	channel->isPlaying(isPlaying);
	//FMOD_Channel_IsPlaying(channel, isPlaying);
	return isPlaying;
}

bool FModSoundCPP::getForcePause()
{
	return forcePause;
}

void FModSoundCPP::setForcePause(bool b)
{
	forcePause = b;
}

//void FModSound::LoadChannelIntoChannelGroup()
//{
//	FModPlaySoundPaused();
//	result = FMOD_Channel_SetChannelGroup(channel, SOUNDMGR->channelGroup);
//}

std::string& FModSoundCPP::GetFilepath()
{
	return givenFilepath;
}
