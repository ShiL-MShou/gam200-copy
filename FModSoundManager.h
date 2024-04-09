/*****************************************************************************/
/*
File Name:		FModSoundManager.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [95%]
				Kok Zhe Yan			 [5%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/

#pragma once
#include "stdafx.h"
#include "System.h"
#include "FModSoundCPP.h"
//#include "FModSound.h"
#include "RapidjsonSerialization.h"
#include <map>

class FModSoundManager : public ISystem
{
	//shared ptr gave me issues when deleting the sound in unload,
	//so using normal ptr for now.

	std::map <std::string, std::shared_ptr<FModSoundCPP>> soundMap;
	//std::map < std::string, std::shared_ptr<FModSound>> soundMap;

	

	FMOD::System* system;
	//FMOD::ChannelGroup* BGMGroup;
	//FMOD::ChannelGroup* SFXGroup;

	float masterVolume;
	float lastVolume;

public:
	FModSoundManager();
	~FModSoundManager();

	int ErrorCheck(FMOD_RESULT _result);
	//FMOD_CHANNELGROUP* channelGroup;

	void Initialize() override;
	void Update(float _dt) override;
	void Clear() override;
	std::string GetName() override;
	void SendMessage(Message& _message);

	std::map< std::string, std::shared_ptr<FModSoundCPP>>&  GetMap();
	//std::map< std::string, std::shared_ptr<FModSound>>&  GetMap();

	//int ErrorCheck(FMOD_RESULT _result);

	void PlaySoundFromMap(const std::string& _key);
	void PlaySoundLoopFromMap(const std::string& _key);
	void PauseSoundFromMap(const std::string& _key);
	void UnPauseSoundFromMap(const std::string& _key);
	void UnloadSoundFromMap(const std::string& _key);

	void AddToMap(const std::string& _key, std::shared_ptr<FModSoundCPP> _soundptr);
	//void AddToMap(const std::string& _key, std::shared_ptr<FModSound> _soundptr);

	float GetMasterVolume() const;
	void SetMasterVolume(float _volume);
	void MuteMasterVolume(bool _b = true);

	void DecrementMasterVolume(float _decrement);
	void IncrementMasterVolume(float _increment);

	void PauseAllSounds();
	void UnPauseAllSounds();
	bool GetPauseStatus(const std::string& _key);

	bool getForcePause(const std::string& _key);
	void setForcePause(const std::string& _key, bool _b);
	void resetAllForcePause();

	void LoadSound(const std::string& _keyname, const std::string& _filename);
};	

//in other files do std::shared_ptr<FModSoundManager> SOUNDMGR;
extern std::shared_ptr<FModSoundManager> SOUNDMGR;