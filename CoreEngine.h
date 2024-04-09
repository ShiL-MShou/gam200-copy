/*************************************************************************************/
/*
File Name:		CoreEngine.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "Engine.h"
#include <ctime>

class CoreEngine
{
public:
	std::string mPath;

	CoreEngine();
	~CoreEngine();

	// Initializes all systems in the game.
	void Init(HINSTANCE _hInst);
	// Update every system while game is active
	void Update();
	// Clears all objects created when loading a new level
	void Clear();
	// Read in data from config file
	bool InitConfig();
	// Init the systems
	void InitSystem();
	// Add Systems to the Engine
	void AddSystems();
	// Destroy all systems in reverse order that they were added.
	void DestroySystems();
	// Broadcasts a message to all systems.
	void BroadcastMessage(Message& _message);
	// Record starting time of system
	void SystemStartUpdate();
	// Record ending time of system
	void SystemEndUpdate(const std::string& _systemname);

	// Tracks the performance data of the systems
	std::map<std::string, double> system_Performance;
	double mCPUTime;
	int currNumSteps;

	bool GetEditorStatus() { return mIsEditorActive; }
	void ToggleEditorStatus();

	bool GetPausedStatus() { return mIsGamePaused; }
	void SetPausedStatus(bool _status);
	void TogglePausedStatus() { mIsGamePaused = !mIsGamePaused; }

	bool isFullScreen() { return m_isFullScreen; }
	void SetFullScreen(bool _b) { m_isFullScreen = _b; }

	std::string GetCurrLevel() { return mCurrLevel; }
	void SetCurrLevel(const std::string& _level) { mCurrLevel = _level; }

	void SetNextLevel(const std::string& _level) { mNextLevel = _level; }

	// tmp for fps
	float GetDT() { return m_dt; }

private:
	// tmp for fps
	float m_dt;

	// Tracks all the systems the game uses
	std::vector<std::shared_ptr<ISystem>> m_systems;
	// The last time the game was updated
	float m_lastTime;
	// Is the game running (true) or being shut down (false)?
	bool m_gameActive;
	// Is Level Editor running
	bool mIsEditorActive;
	// Is Game paused
	bool mIsGamePaused;
	// current level of the game
	std::string mCurrLevel;
	// next level of the game
	std::string mNextLevel;

	bool m_isFullScreen;

	// set status of vsync on/off
	void SetVSync(bool _sync);
};

// Message telling the game to quit
class MessageQuit : public Message
{
public:
	MessageQuit() : Message(MessageId::Quit) {};
};

//A global pointer to the instance of the core
extern std::unique_ptr<CoreEngine> CORE;
