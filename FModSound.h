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

#pragma once
#include "fmod.h"


class FModSound
{
	bool soundOn;			//check if sound is on
	bool soundPlayable;		//check if possible to play sound
	char* currentSound;		//check for current played sound

	bool crudeSoundIsPlaying; //fmod bool seems odd so this is brute force check if im playing the sound

	std::string givenFilepath;

	FMOD_RESULT result;
	FMOD_SYSTEM* system;
	FMOD_SOUND* sound;
	FMOD_CHANNEL* channel;
	float volume;

public:
	FModSound();
	FModSound(FModSound&);
	FModSound& operator = (FModSound&);
	~FModSound();

	void InitSound();								//initialises sound
	void SetVolume(float volume);					//sets volume
	float GetVolume() const;						//gets the volume
	void LoadFile(const char* filename);			//loads the file
	void UnloadFile(void);							//unloads the file
	void FModPlaySound(bool pause = false);			//plays the sound
	void FModPlaySoundPaused(bool pause = true);		//plays the sound
	void SoundUpdate();								//updates the sound
	bool GetSound();								//gets the current sound
	void PlaySoundLoop(bool pause = false);			//plays sounds in the loop
	void ToggleSound();								//toggles the current sound
	void TogglePause();								//toggle pause for the current sound
	bool SoundIsPlaying();
	bool CrudeSoundIsPlaying();

	//void LoadChannelIntoChannelGroup();

	std::string& GetFilepath();
	FMOD_CHANNEL* GetChannel() { return channel; }
	void SetPause(bool pause);//pauses current sound
	void SetSound(bool _sound);//set current sound
};

