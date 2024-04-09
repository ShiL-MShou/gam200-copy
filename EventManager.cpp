#include "stdafx.h"

#include "EventManager.h"

#include "FModSoundManager.h"

#include "Logic.h"
#include "Score.h"

EventManager::EventManager() : inConfirmQuit{ false }, inGameStage{ false }, events{}, inDialog{ false }, pageCount{ 0 }, npcchase{}
{
}

EventManager::~EventManager()
{
}

EventManager::NPCEvent EventManager::GetCurrEvent()
{
	return currevent;
}

void EventManager::Initialize()
{
	textscriptPtr = LOGIC->GetScript<TextScript>("TEXT_SCRIPT");
	MyScore.Initialize();

}

void EventManager::LoadResource()
{
	FACTORY->CreateArcheType("PlayerSprite");
	FACTORY->CreateArcheType("TeacherSprite");
	FACTORY->CreateArcheType("Tint");
	FACTORY->CreateArcheType("Text");
	FACTORY->CreateArcheType("AlarmVFX");

	textPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Text", "Text", false)->has(SpriteComponent);
	textPtr.lock()->SetActive(false);
	tintPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Tint", "Tint", false)->has(SpriteComponent);
	tintPtr.lock()->SetActive(false);
	boyPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "PlayerSprite", "BoyDialog", false)->has(SpriteComponent);
	boyPtr.lock()->SetActive(false);
	teacherPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "TeacherSprite", "TeacherDialog", false)->has(SpriteComponent);
	teacherPtr.lock()->SetActive(false);

	
}

void EventManager::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	if (events["alarm"])
	{
		if (INPUTMGR->GetKeyRelease(MVK_RETURN) || INPUTMGR->GetKeyRelease(MVK_SPACE) || INPUTMGR->GetKeyRelease(MVK_LBUTTON))
		{
			if (pageCount == 5)
			{
				textscriptPtr.lock()->Serialize(CORE->mPath + "Text/FireAlarm/FAlarmBoi1.json");
				//create the boy's sprite for dialog
				SOUNDMGR->PlaySoundFromMap("boyVoice");
				boyPtr.lock()->SetActive(true);

				--pageCount;//becomes 4
			}
			else if (pageCount == 4)
			{
				SOUNDMGR->PlaySoundFromMap("boyYikes");

				textscriptPtr.lock()->Serialize(CORE->mPath + "Text/FireAlarm/FAlarmBoi2.json");
				--pageCount;//becomes 3
			}
			else if (pageCount == 3)
			{
				//SOUNDMGR->PlaySoundFromMap("boyYikes");
				boyPtr.lock()->SetActive(false);
				textscriptPtr.lock()->Serialize(CORE->mPath + "Text/FireAlarm/meanwhile.json");
				--pageCount;//becomes 2
			}
			else if (pageCount == 2)//teacher 1
			{
				SOUNDMGR->PlaySoundFromMap("teacherVoice");
				teacherPtr.lock()->SetActive(true);
				textscriptPtr.lock()->Serialize(CORE->mPath + "Text/FireAlarm/FAlarmXiao1.json");
				--pageCount;//becomes 1
			}
			else if (pageCount == 1)//teacher 2
			{
				SOUNDMGR->PlaySoundFromMap("teacherVoice");
				textscriptPtr.lock()->Serialize(CORE->mPath + "Text/FireAlarm/FAlarmXiao2.json");
				--pageCount;//becomes 0
			}
			else if (pageCount == 0)
			{
				CORE->SetPausedStatus(false);
				//SOUNDMGR->UnPauseSoundFromMap("BGM");
				//SOUNDMGR->PlaySoundLoopFromMap("BGM");

				tintPtr.lock()->SetActive(false);
				teacherPtr.lock()->SetActive(false);
				textPtr.lock()->SetActive(false);
				//inFireAlarmDialog = false;
				inDialog = false;
				events["alarm"] = false;
			}
		}
	}
}

void EventManager::Clear()
{
	events.clear();
	npcchase.clear();
	currevent = NPCEvent::NONE;
}

void EventManager::AddEvent(const std::string& _event)
{
	events[_event] = false;
}

void EventManager::ActivateDistraction(const VEC2& _loc, const std::string& _event)
{
	currevent = NPCEvent::DISTRACTED;

	events[_event] = true;

	SOUNDMGR->PlaySoundLoopFromMap(_event);
	SOUNDMGR->PauseSoundFromMap("BGM");
	SOUNDMGR->setForcePause("BGM", true);
	//std::cout << CORE->GetCurrLevel() << std::endl;

	alarmVFXPtr = LOGIC->CreateObjectAt(VEC2(_loc.x, _loc.y + 150.0f), 0, "AlarmVFX", "avfx", false)->has(SpriteComponent);
	alarmVFXPtr.lock()->SetActive(true);

	if (events["alarm"] && strcmp(CORE->GetCurrLevel().c_str(), "Stage2v2") == 0) 
	{
		CORE->SetPausedStatus(true);
		inDialog = true;

		pageCount = 6;

		textPtr.lock()->SetActive(true);
		tintPtr.lock()->SetActive(true);

		textscriptPtr.lock()->Serialize(CORE->mPath + "Text/FireAlarm/AlarmPulled.json");
		--pageCount; //becomes 5
	}

	NPCDISTRACT message(_loc, events["alarm"]);
	LOGIC->SendMessage(message);
}

void EventManager::ActivateChase(const VEC2& _loc, const std::string& _npc)
{
	currevent = NPCEvent::CHASE;

	npcchase.push_back(_npc);

	events["chase"] = true;

	//SOUNDMGR->PlaySoundLoopFromMap("chase");
	//SOUNDMGR->PauseSoundFromMap("BGM");

	UNREFERENCED_PARAMETER(_loc);
}

void EventManager::DeActivateChase()
{
	npcchase.pop_back();

	if (npcchase.empty())
	{
		currevent = NPCEvent::NONE;
		events["chase"] = false;
	}
}

void EventManager::DeActivateAlarmVFX()
{
	alarmVFXPtr.lock()->SetActive(false);
	SOUNDMGR->setForcePause("alarm", true);
}


