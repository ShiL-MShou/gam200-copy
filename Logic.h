/*************************************************************************************/
/*
File Name:		Logic.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once

#include "CoreEngine.h"
#include "Physics.h"
//#include "Engine.h"
#include "Graphics.h"
#include "StateMachine.h"
#include "LogicComponent.h"
#include "FModSound.h"

#include "PathFind.h"

#include "ObjectAllocator.h"

class Logic : public ISystem
{
public:

	enum class GameState
	{
		WIN = 0,
		LOSE,
		GAMEPLAY
	};
private:
	//std::vector<std::shared_ptr<LogicComponent>> logicComponentList;

	bool pathFindCalled;

	std::set<CompId> logicComponentList;
	std::map<std::string, std::shared_ptr<StateMachine>> stateMachineList;
	std::map<std::string, std::shared_ptr<Script>> scriptList;

	//FModSound* alarmSound = nullptr;

	GameState gameState;
	
	std::vector <std::vector<bool>> aiMap;
	bool once = true;

	bool cheat;
	//bool aiMap[gCOLUMN][gROW];

public:

	ObjectAllocator* _oa;

	Logic();
	~Logic();

	virtual void Initialize();
	virtual void Update(float _dt) override;
	virtual void Clear() override;
	virtual std::string GetName() override { return "Logic"; }
	virtual void SendMessage(Message& _message);
	bool GetCheat() { return cheat; }

	std::shared_ptr<GOC> CreateObjectAt(VEC2 _pos, float _rot, const std::string& _archetype, const std::string& _name, bool _tosave = true);
	// overload of create object function so no obj pointer is returned
	//void CreateObjectAt(VEC2 _pos, float _rot, const std::string& _archetype, const std::string& _name, int);

	void LoadLevel(const std::string& _levelname);
	void ChangeLevel(const std::string& _levelname = CORE->GetCurrLevel());
	void SaveLevel(const std::string& _levelname);

	void UpdateKeyBoardControls();

	std::shared_ptr<StateMachine> GetStateMachine(std::string _name);
	void LoadScripts(const std::string& _path);

	void AddLogicComponent(CompId _id);
	void RemoveLogicComponent(CompId _id);
	void AddStateMachine(std::string _name, std::shared_ptr<StateMachine> _stateMachine);
	void AddScript(std::string _name, std::shared_ptr<Script> _scriptPtr);

	void ClearInventory();

	template <typename type = Script>
	std::shared_ptr<type> GetScript(std::string _name) { return std::static_pointer_cast<type>(scriptList[_name]); }

	GameState GetGameState() { return gameState; }

	//std::array <std::array<bool, gROW>, gCOLUMN>& GetAImap();
	std::vector<std::vector<bool>>& GetAImap();
	void CanDistract();

	int GetSMStateIndex(const std::string& smname);
	//void InitAlarmSound();
	//FModSound* getAlarmSoundptr() const;
	//void UnloadAlarmSound();

	//template<typename type>
	//inline std::shared_ptr<type> CreateScript()
	//{
	//	return std::make_shared<type>(); //td::static_pointer_cast<type>(GetComponent(_typeId));
	//}

	bool IsPathFindCalled();
	void PathFindCalled();

};

extern std::shared_ptr<Logic> LOGIC;

extern int MapWidth;
extern int MapHeight;