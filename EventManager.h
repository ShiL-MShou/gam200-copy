#pragma once

#include "Singleton.h"

//#include "Logic.h"
//#include "CoreEngine.h"
//
//#include "Engine.h"

#include "TextScript.h"

#include "SpriteComponent.h"

class EventManager : public Singleton<EventManager>
{
public:
	bool inConfirmQuit;
	bool inGameStage;

	void Initialize();
	void LoadResource();
	void Update(float _dt);
	void Clear();

	void AddEvent(const std::string& _event);

	void ActivateDistraction(const VEC2& _loc, const std::string& _event);

	void ActivateInteractable();

	void ActivateChase(const VEC2& _loc, const std::string& _npc);

	void DeActivateChase();

	void DeActivateAlarmVFX();

	enum class NPCEvent
	{
		NONE,
		DISTRACTED,
		CHASE,
		NUMEVENT
	};


	std::weak_ptr<SpriteComponent> textPtr;
	std::weak_ptr<SpriteComponent> boyPtr;
	std::weak_ptr<SpriteComponent> teacherPtr;
	std::weak_ptr<SpriteComponent> tintPtr;
	std::weak_ptr<TextScript> textscriptPtr;

	std::weak_ptr<SpriteComponent> alarmVFXPtr;

private:

	NPCEvent currevent;

	EventManager();

	~EventManager();

	friend class Singleton<EventManager>;

	std::map<std::string, bool> events;

	bool inDialog;

	int pageCount;

	std::vector<std::string> npcchase;

public:
	NPCEvent GetCurrEvent();
};

#define MyEvent (EventManager::GetInstance())
