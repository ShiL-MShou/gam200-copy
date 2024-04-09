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
#include "fmod.hpp"


class FModSoundCPP
{
	bool soundOn;			//check if sound is on
	bool soundPlayable;		//check if possible to play sound
	char* currentSound;		//check for current played sound

	bool forcePause; //fmod bool seems odd so this is brute force check if im playing the sound

	std::string givenFilepath;

	FMOD::System* system;
	
	
	float volume;

public:
	FMOD::Sound* sound;
	FMOD::Channel* channel;

	FModSoundCPP();
	//FModSoundCPP(FModSoundCPP&);
	//FModSoundCPP& operator = (FModSoundCPP&);
	~FModSoundCPP();

	int ErrorCheck(FMOD_RESULT _result);

	void InitSound();								//initialises sound
	void SetVolume(float volume);					//sets volume
	float GetVolume();						//gets the volume
	void LoadFile(const char* filename);			//loads the file
	void UnloadFile(void);							//unloads the file

	//void FModPlaySound(bool pause = false);			//plays the sound
	void FModPlaySoundPaused(bool pause = true);	//plays the sound

	void SoundUpdate();								//updates the sound
	//void PlaySoundLoop(bool pause = false);			//plays sounds in the loop
	void ToggleSound();								//toggles the current sound
	void TogglePause();								//toggle pause for the current sound
	bool SoundIsPlaying();

	bool getForcePause();
	void setForcePause(bool b);

	std::string& GetFilepath();
	void SetFilepath(std::string _filename) { givenFilepath = _filename; }
	FMOD::Sound* GetSound() { return sound; }				//gets the current sound
	FMOD::Channel* GetChannel() { return channel; }

	void SetPause(bool pause);//pauses current sound
	bool GetPause();
	void SetSound(bool _sound);//set current sound
};

