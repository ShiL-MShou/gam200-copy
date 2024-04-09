/*************************************************************************************/
/*
File Name:		Logic.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"
#include "Logic.h"
#include "CoreEngine.h"
#include "WindowsSystem.h"
#include "ComponentCreator.h"	
#include "TextSerialization.h"
#include "RapidjsonSerialization.h"
#include "Input.h"

#include "LogicComponent.h"

#include "Composition.h"

#include "Script.h"

#include "imgui.h"
#include "MyGuiManager.h"

// scripts
//#include "NPCScript.h"
#include "TeacherScript.h"
#include "PrefectScript.h"

#include "PrefectScript1.h"
#include "PrefectScript2.h"
#include "PrefectScript3.h"
#include "PrefectScript4.h"
#include "PrefectScript5.h"
#include "PrefectScript6.h"
#include "PrefectScript7.h"

#include "DISTRACTED_STATE.h"
#include "List_StateLinker.h"
#include "Vector2.h"
#include "FireAlarmScript.h"
#include "DoorScript.h"
#include "HomeWorkScript.h"
#include "ScriptButton.h"
#include "MainMenuScript.h"
#include "TextScript.h"
#include "LoseScript.h"
#include "WinScript.h"

#include "RubbishBin.h"
#include "AlarmClockScript.h"
#include "BananaScript.h"
#include "TutorialScript1.h"
#include "MouseCursorScript.h"
#include "PauseMenuScript.h"
#include "SettingsMenuScript.h"
#include "ConfirmQuitScript.h"
#include "WinConditionScript.h"

#include "ScriptButtonStart.h"
#include "ScriptButtonHowToPlay.h"
#include "ScriptButtonSettings.h"
#include "ScriptButtonCredits.h"
#include "ScriptButtonResume.h"
#include "ScriptButtonQuit.h"
#include "ScriptButtonRestart.h"
#include "ScriptButtonMainMenu.h"
#include "ScriptButtonNextStage.h"
#include "ScriptButtonBack.h"
#include "ScriptButtonNo.h"
#include "ScriptButtonYes.h"
#include "WaterCoolerScript.h"
#include "WaterScript.h"

#include "ScriptCheckBoxFullScreen.h"
#include "ScriptCheckBoxMute.h"
#include "VolumeDownScript.h"
#include "VolumeUpScript.h"

#include "FModSoundManager.h"
#include "FModSound.h"

#include "Graphics.h"
#include "DebugDraw.h"

#include "Score.h"
#include "ResultScript.h"

#include "LoadingScreenScript.h"
#include "TransitionScript.h"
#include "ComicScript.h"


std::shared_ptr<Logic> LOGIC;

int MapWidth = 0;
int MapHeight = 0;

Logic::Logic() 
	: gameState{GameState::GAMEPLAY}
{
	cheat = false;
	//test = nullptr;
}

Logic::~Logic()
{
	stateMachineList.clear();
	scriptList.clear();

	delete _oa;
	//UnloadAlarmSound();
}

//void Logic::InitAlarmSound()
//{
//
//}

void Logic::Initialize()
{
	RegisterComponent(Transform);
	RegisterComponent(LogicComponent);

	LoadScripts("");

	for (auto [name, scriptPtr] : scriptList)
	{
		scriptPtr->Start();
	}

	OAConfig config;
	_oa = new ObjectAllocator(sizeof(Node), config);
	//InitAlarmSound();
}
//
//FModSound* Logic::getAlarmSoundptr() const
//{
//	return alarmSound;
//}

//void Logic::UnloadAlarmSound() 
//{
//	/*alarmSound->unloadFile();
//	if (alarmSound)
//	{
//		delete alarmSound;
//	}
//	alarmSound = nullptr;*/
//}

void Logic::Update(float _dt)
{
	if (WINDOWS->outoffocus)
		return;

	UpdateKeyBoardControls();

	if (CORE->GetEditorStatus() && MyGuiManager::isGamePaused)
		return;

	// reset
	pathFindCalled = false;

	for (auto id : logicComponentList)
	{
		std::shared_ptr<LogicComponent> logicCompPtr = MyResource.GetComponentWithID<LogicComponent>(id);

		// --- scripts ---
		std::string name = logicCompPtr->GetScriptName();

		if (name != "NONE")
		{
			// safety checks
			if (scriptList.find(name) != std::end(scriptList))
			{
				scriptList[name]->SetOwner(logicCompPtr->GetBase());
				scriptList[name]->Update(_dt);
			}
			else
			{
				assert(0 && "Script not found!");
			}
		}

		
		/*if (CORE->GetPausedStatus())
			continue;*/

		//// --- State machine ---
		//std::string smName = logicCompPtr->GetSMName();
		//
		//if (smName != "no")
		//{
		//	// safety checks
		//	if(stateMachineList.find(smName) != stateMachineList.end())
		//	{ 
		//		stateMachineList[smName]->SetOwner(logicCompPtr->GetBase());
		//		stateMachineList[smName]->Update(CORE->GetDT());
		//	}
		//	else
		//	{
		//		assert(0 && "StateMachine not found!");
		//	}
		//}

	}

	MyEvent.Update(_dt);

	//MyDebugDraw.DrawLine(VEC2(0, 0), VEC2(64, 64));

	// drawing of the grid
	if (GRAPHICS->isDebug)
	{
		std::ifstream file(CORE->mPath + "AI_Grid_Map/" + CORE->GetCurrLevel() + ".json");
		if (file.is_open())
		{
			file.close();
		}
		else
		{
			return;
		}
		for (size_t i = 0; i < MapHeight; i++)
		{
			for (size_t k = 0; k < MapWidth; k++)
			{
				MyDebugDraw.DrawRect(	VEC2(static_cast<float>(cWIDTH * k) + cWIDTH / 2.0f, cHEIGHT * i + cHEIGHT / 2.0f),
										VEC2(	cWIDTH, cHEIGHT));

				if (aiMap[i][k] == false)
					MyDebugDraw.DrawLine(	VEC2((float)(cWIDTH * k), static_cast<float>(cHEIGHT * i) ),
											VEC2((float)(cWIDTH * (k+1)), static_cast<float>(cHEIGHT * (i+1) )));
			}
		}
		
	}

}

void Logic::Clear()
{
	for (auto [name, scriptPtr] : scriptList)
	{
		scriptPtr->Exit();
	}
	for (auto [name, smPtr] : stateMachineList)
	{
		smPtr->ResetSM();
	}

	logicComponentList.clear();
	MyEvent.Clear();

	stateMachineList.clear();
	//scriptList.clear();
}

void Logic::SendMessage(Message& _message)
{
	auto iter = logicComponentList.begin();
	auto end = logicComponentList.end();
	switch (_message.m_Mid)
	{
	

	case MessageId::NPCDistract:
	{
		NPCDISTRACT distract = dynamic_cast<NPCDISTRACT&>(_message);

		for (auto [smname, smptr] : stateMachineList)
		{
			if (strcmp(smname.c_str(), "PREFECT_1") == 0)
				continue;
			if (distract.attractall)
			{
				if (stateMachineList[smname]->GetCurrState()->_name != "Chase")
					smptr->SendDistractMsgSM(true, distract.ObjPos, smptr->GetCurrStateindex(), "distracted");
			}
		}
	}
		break;

	case MessageId::NPCCollide:
		// Find NPC that has collided, stop their SM
		for (auto id : logicComponentList)
		{
			std::shared_ptr<LogicComponent> logicCompPtr = MyResource.GetComponentWithID<LogicComponent>(id);
			std::string smName = logicCompPtr->GetSMName();
			
			if (smName == "NPC1")
			{
				stateMachineList[smName]->CollideWithItem();
			}
		}
		break;

	case MessageId::Win:
		if (gameState != GameState::WIN)
		{
			gameState = GameState::WIN;
			LoadLevel("Win");
		}
		break;
	case MessageId::Lose:

		if (gameState != GameState::LOSE)
		{
			gameState = GameState::LOSE;
			LoadLevel("Lose");
		}
		break;

	default:
		break;
	}
}

std::shared_ptr<GOC> Logic::CreateObjectAt(VEC2 _pos, float _rot, const std::string& _archetype, const std::string& _name, bool _tosave)
{
	// Create the object by serializing it with the file
	std::shared_ptr<GOC> object = FACTORY->CreateObject(_archetype);
	//Get the transform and adjust the position
	std::shared_ptr<Transform> transform = object->has(Transform);
	transform->SetPosition(_pos);
	transform->SetRotation(_rot);
	//transform->SetStartPosition(_pos);
	
	if (_tosave)
		FACTORY->IDObject(object);
	else
		FACTORY->IDTile(object);

	object->Initialize(object);

	object->SetName(_name);

	return object;
}

//void Logic::CreateObjectAt(VEC2 _pos, float _rot, const std::string& _archetype, const std::string& _name, int)
//{
//	// Create the object by serializing it with the file
//	std::shared_ptr<GOC> object = FACTORY->CreateObject(_archetype, true);
//	//Get the transform and adjust the position
//	std::shared_ptr<Transform> transform = object->has(Transform);
//	transform->SetPosition(_pos);
//	transform->SetRotation(_rot);
//
//	object->SetName(_name);
//}

void Logic::LoadLevel(const std::string& _levelname)
{
	//gameState = GameState::GAMEPLAY;

	// loading scripts resources
	for (auto [ignore, scriptPtr] : scriptList)
	{
		scriptPtr->LoadResource();
	}

	std::string file = CORE->mPath + "Level Files/" + _levelname + ".json";

	JsonReader jr(file);
	bool isFileOpened = jr.IsFileOpened();
	assert(isFileOpened);

	jr.ParseStreamToDoc();
	jr.doc.Accept(jr.prettyReaderBuffer);

	if (!isFileOpened)
	{
		std::cout << "Error Loading level!" << std::endl;
		return;
	}
	else
	{
		//needed to reset the ifstream good() flag, otherwise for some reason the below jr.IsGood() will be false
		jr.stream.clear();
	}
	
	std::string jsonKey1 = "Object";

	std::string objectName, archeType, texture;
	VEC2 objectPos;
	float objectRot;

	std::string name;
	int i = 1;
	///
	std::string jsonKey2 = "Sound";
	std::string soundFile, soundKey;
	std::string soundName;
	int soundnum = 1;
	///
	while (jr.IsGood())
	{
		int startFrame = -1;
		int maxFrame = -1;
		texture = "NIL";
		float halflengthx = -1, halflengthy = -1;
		float offsetx = -1, offsety = -1;
		float fixedposx = -1, fixedposy = -1;

		name = jsonKey1 + std::to_string(i); //Object + 1 = Object1
		///
		soundName = jsonKey2 + std::to_string(soundnum);

		if (jr.doc.HasMember(soundName.c_str()))
		{
			jr.ReadStringMember(soundName, "Key", soundKey);
			jr.ReadStringMember(soundName, "Filename", soundFile);

			//check if there is no sound in the map, so we dont double dip.
			//if no sound, free to load in the stuff
			SOUNDMGR->LoadSound(soundKey, soundFile);
		}
		///
		if (jr.doc.HasMember(name.c_str()))
		{
			jr.ReadStringMember(name, "ObjectName", objectName);
			jr.ReadStringMember(name, "ArcheType", archeType);
			jr.ReadFloatMember(name, "PosX", objectPos.x);
			jr.ReadFloatMember(name, "PosY", objectPos.y);
			jr.ReadFloatMember(name, "Rotation", objectRot);
			jr.ReadStringMember(name, "TextureSubName", texture);
			jr.ReadIntMember(name, "StartFrame", startFrame);
			jr.ReadIntMember(name, "MaxFrame", maxFrame);
			jr.ReadFloatMember(name, "OffsetX", offsetx);
			jr.ReadFloatMember(name, "OffsetY", offsety);
			jr.ReadFloatMember(name, "HalfLengthX", halflengthx);
			jr.ReadFloatMember(name, "HalfLengthY", halflengthy);
			jr.ReadFloatMember(name, "FixedPosX", fixedposx);
			jr.ReadFloatMember(name, "FixedPosY", fixedposy);

			FACTORY->CreateArcheType(archeType);
			std::shared_ptr<GOC> object = CreateObjectAt(objectPos, objectRot, archeType, objectName);
			assert(object.get());

			std::shared_ptr<SpriteComponent> sprPtr = object->has(SpriteComponent);
			std::shared_ptr<AnimationComponent> animationCompPtr = object->has(AnimationComponent);

			
			if (fixedposx != -1)
				sprPtr->SetFixedPosition(VEC2{ fixedposx ,fixedposy });

			if (texture != "NIL")
				sprPtr->SetCurrSubTextureName(texture);
			if (startFrame != -1)
				sprPtr->SetStartFrame(startFrame);
			if (maxFrame != -1)
				sprPtr->SetMaxFrame(maxFrame);
			
			if (sprPtr)
			{
				std::string tmpSubTextureName = "";
				int subTextureCount = 0;

				jr.ReadIntMember(name, "SubTextureCount", subTextureCount);
				//jw.ReadInt("SubTextureCount", tmpSubTextureList.size());

				for (int k = 0; k < subTextureCount; ++k)
				{
					jr.ReadStringMember(name, "SubTextureName" + std::to_string(k + 1), tmpSubTextureName);

					//jw.CreateStringMember("SubTextureName" + std::to_string(i + 1), tmpSubTextureList[i]);
					sprPtr->AddSubTexture(tmpSubTextureName);
				}
			}
			
			if (animationCompPtr)
			{
				int tmp = -1;
				jr.ReadIntMember(name, "StartingAnimation", tmp);

				if (tmp != -1)
					animationCompPtr->currAnimation = tmp-1;

			}


			if (halflengthx != -1)
			{
				std::shared_ptr<BoxCollider> colliderPtr = object->has(BoxCollider);
				
				if (colliderPtr != nullptr)
				{
					colliderPtr->SetHalfLengths(VEC2(halflengthx, halflengthy));
					colliderPtr->SetOffSets(VEC2(offsetx, offsety));
				}
			}
		}
		else
			break;

		++i;
		++soundnum;
	}

	// loading graphics resources
	GRAPHICS->LoadLevel();
	MyEvent.LoadResource();

	GRAPHICS->InitLevelMap(CORE->mPath + "Tilemaps/" + CORE->GetCurrLevel() + "Wall.json");
	GRAPHICS->InitLevelMap(CORE->mPath + "Tilemaps/" + CORE->GetCurrLevel() + "Floor.json");

	// Load collider map
	std::ifstream mfile(CORE->mPath + "AI_Grid_Map/" + CORE->GetCurrLevel() + ".json");
	char c;

	for (auto& row : aiMap)
	{
		row.clear();
	}
	aiMap.clear();

	if (mfile.is_open())
	{
		int w = 0;
		int h = 0;

		std::vector<bool> row;

		while (mfile.get(c))
		{
			if (!(c == '\n')) 
			{
				row.push_back(c - '0');
				++w;
			}
			else
			{
				aiMap.push_back(row);
				row.clear();
				w = 0;
				++h;
			}
		}

		MapWidth = static_cast<int>(aiMap[0].size());
		MapHeight = static_cast<int>(aiMap.size());
		
		mfile.close();
	}
}

void Logic::ChangeLevel(const std::string& _levelname)
{
	/*CORE->Clear();
	if (strcmp(_levelname.c_str(), "AutoSave"))
		CORE->SetCurrLevel(_levelname);
	LoadLevel(_levelname);*/
	//std::cout << _levelname << std::endl;

	if (strcmp(CORE->GetCurrLevel().c_str(), _levelname.c_str()) == 0)// || strcmp("Restart", _levelname.c_str()) == 0)
	{
		//CORE->SetNextLevel(CORE->GetCurrLevel());
		CORE->SetCurrLevel("RESTART");
	}

	CORE->SetNextLevel(_levelname);
	
	MyScore.clearStageVars();

	// reset
	gameState = GameState::GAMEPLAY;
}

void Logic::SaveLevel(const std::string& _levelname)
{
	std::string file = CORE->mPath + "Level Files/" + _levelname + ".json";

	JsonWriter jw;

	jw.prettyWriterBuffer.StartObject();


	std::string jsonKey1 = "Object";
	int count = 1;

	std::string jsonKey2 = "Sound";
	std::string soundFile, soundKey;
	std::string soundName;
	int soundnum = 1;

	auto objMap = MyResource.GetObjMap();

	for (auto& soundPtr : SOUNDMGR->GetMap())
	{
		if (soundPtr.second == nullptr)
			continue;
		jw.CreateKey(jsonKey2 + std::to_string(soundnum));
		jw.prettyWriterBuffer.StartObject();
		jw.CreateStringMember("Key", soundPtr.first);
		jw.CreateStringMember("Filename",  soundPtr.second->GetFilepath().c_str());
		jw.prettyWriterBuffer.EndObject();
		++soundnum;
	}

	for (auto& objPtr : objMap)
	{
		if (objPtr == nullptr || objPtr->isInventory())
			continue;

		std::shared_ptr<Transform> tmpTrans = objPtr->has(Transform);
		std::shared_ptr<SpriteComponent> tmpSpr = objPtr->has(SpriteComponent);
		std::shared_ptr<BoxCollider> tmpCollider = objPtr->has(BoxCollider);
		std::shared_ptr<AnimationComponent> tmpAnimationComp = objPtr->has(AnimationComponent);

		if (tmpTrans)
		{
			jw.CreateKey(jsonKey1 + std::to_string(count));
			jw.prettyWriterBuffer.StartObject();
			jw.CreateStringMember("ObjectName", objPtr->GetName());
			jw.CreateStringMember("ArcheType", objPtr->GetType());

			jw.CreateFloatMember("PosX", tmpTrans->GetStartPosition().x);
			jw.CreateFloatMember("PosY", tmpTrans->GetStartPosition().y);
			jw.CreateFloatMember("Rotation", tmpTrans->GetRotation());


			if (tmpSpr != nullptr)
			{
				if (tmpSpr->GetType() == SpriteType::SINGLE)
				{
					// remove duplicates
					auto last = std::unique(tmpSpr->GetSubTextureList().begin(), tmpSpr->GetSubTextureList().end());
					// v now holds {1 2 3 4 5 6 7 x x x x x x}, where 'x' is indeterminate
					tmpSpr->GetSubTextureList().erase(last, tmpSpr->GetSubTextureList().end());



					jw.CreateIntMember("StartFrame", tmpSpr->GetStartFrame());
					jw.CreateStringMember("TextureSubName", tmpSpr->GetCurrSubTextureName());
					jw.CreateIntMember("MaxFrame", tmpSpr->GetMaxFrame());

					// saving subTextures
					auto& tmpSubTextureList = tmpSpr->GetSubTextureList();
					jw.CreateIntMember("SubTextureCount", static_cast<int>(tmpSubTextureList.size()));

					for (int i = 0; i < tmpSubTextureList.size(); ++i)
					{
						jw.CreateStringMember("SubTextureName" + std::to_string(i + 1), tmpSubTextureList[i]);
					}
				}
				if (tmpSpr->GetMovementType() == SpriteMovementType::FIXED)
				{
					jw.CreateFloatMember("FixedPosX", tmpSpr->GetFixedPosition().x);
					jw.CreateFloatMember("FixedPosY", tmpSpr->GetFixedPosition().y);
				}

			}
			if (tmpCollider)
			{
				jw.CreateFloatMember("OffsetX", tmpCollider->GetOffSets().x);
				jw.CreateFloatMember("OffsetY", tmpCollider->GetOffSets().y);
				jw.CreateFloatMember("HalfLengthX", tmpCollider->GetLengths().x);
				jw.CreateFloatMember("HalfLengthY", tmpCollider->GetLengths().y);
			}
			if (tmpAnimationComp)
			{
				jw.CreateIntMember("StartingAnimation", tmpAnimationComp->startingAnimation);
			}


			jw.prettyWriterBuffer.EndObject();
		}
		else
		{
			assert(0);
		}

		++count;
		
	}

	jw.prettyWriterBuffer.EndObject();

	jw.WriteToJsonFile(file);
}

void Logic::UpdateKeyBoardControls()
{
	// for debug purposes - move camera in game
	/*if (INPUTMGR->GetKeyHold(MVK_W))
		GRAPHICS->TestCameraY(-5.0f);
	else if (INPUTMGR->GetKeyHold(MVK_A))
		GRAPHICS->TestCameraX(5.0f);
	else if (INPUTMGR->GetKeyHold(MVK_S))
		GRAPHICS->TestCameraY(5.0f);
	else if (INPUTMGR->GetKeyHold(MVK_D))
		GRAPHICS->TestCameraX(-5.0f);*/

	// toggle debug
	if (INPUTMGR->GetKeyPressed(MVK_RBRACKET))
		GRAPHICS->ToggleDebugInfo();

	// camera zoom in/out
	/*if (INPUTMGR->GetKeyPressed(MVK_EQUAL))
		GRAPHICS->TestCameraZoom(0.1f);
	else if (INPUTMGR->GetKeyPressed(MVK_MINUS))
		GRAPHICS->TestCameraZoom(-0.1f);*/
	
	// toggle level editor
	if (INPUTMGR->GetKeyRelease(MVK_F3))
		CORE->ToggleEditorStatus();

	if (INPUTMGR->GetKeyRelease(MVK_F4))
		CORE->TogglePausedStatus();

	if (INPUTMGR->GetKeyRelease(MVK_P))
		cheat = !cheat;

	if (CORE->GetEditorStatus())
		return;

	if (INPUTMGR->GetKeyRelease(MVK_8))
	{
		Message msg(MessageId::Win);
		LOGIC->SendMessageW(msg);
	}

	if (INPUTMGR->GetKeyRelease(MVK_9))
	{
		Message msg(MessageId::Lose);
		LOGIC->SendMessageW(msg);
	}
		
	if (INPUTMGR->GetKeyRelease(MVK_1))
		ChangeLevel("Stage1v2");

	if (INPUTMGR->GetKeyRelease(MVK_2))
		ChangeLevel("Stage2v2");

	if (INPUTMGR->GetKeyRelease(MVK_3))
		ChangeLevel("Stage3v2");
	
	if (INPUTMGR->GetKeyRelease(MVK_4))
		ChangeLevel("Stage4");
}

std::shared_ptr<StateMachine> Logic::GetStateMachine(std::string _name)
{
	if (stateMachineList.find(_name) != stateMachineList.end())
		return stateMachineList[_name];

	return nullptr;
}


void Logic::LoadScripts(const std::string& _path)
{
	UNREFERENCED_PARAMETER(_path);

	 /*
	JsonReader jr(_path);
	assert(jr.IsFileOpened() && "Error Loading Scripts!");

	jr.ParseStreamToDoc();
	jr.doc.Accept(jr.prettyReaderBuffer);

	//needed to reset the ifstream good() flag, otherwise for some reason the below jr.IsGood() will be false
	jr.stream.clear();

	std::string scriptType, scriptName, name;

	std::string jsonKey1 = "Script";
	int i = 1;

	while (jr.IsGood())
	{
		name = jsonKey1 + std::to_string(i);

		if (jr.doc.HasMember(name.c_str()))
		{
			jr.ReadStringMember(name, "Name", scriptName);
			jr.ReadStringMember(name, "Type", scriptType);
		}
		else
			break;

		++i;
	}*/

	//AddScript("PlayerScript", std::make_shared<PlayerScript>());
	AddScript("PLAYER_SCRIPT",				std::make_shared<PlayerScript>());
	AddScript("TEACHER_SCRIPT",				std::make_shared<TeacherScript>());
	//AddScript("PREFECT_SCRIPT",				std::make_shared<PrefectScript>());

	AddScript("PREFECT_SCRIPT_1",			std::make_shared<PrefectScript1>());
	AddScript("PREFECT_SCRIPT_2",			std::make_shared<PrefectScript2>());
	AddScript("PREFECT_SCRIPT_3",			std::make_shared<PrefectScript3>());
	AddScript("PREFECT_SCRIPT_4",			std::make_shared<PrefectScript4>());
	AddScript("PREFECT_SCRIPT_5",			std::make_shared<PrefectScript5>());
	AddScript("PREFECT_SCRIPT_6",			std::make_shared<PrefectScript6>());
	AddScript("PREFECT_SCRIPT_7",			std::make_shared<PrefectScript7>());
	
	//AddScript("DOOR_SCRIPT",				std::make_shared<DoorScript>());
	AddScript("DUSTBIN_SCRIPT",             std::make_shared<RubbishBin>());
	AddScript("FIREALARM_SCRIPT",			std::make_shared<FireAlarmScript>());
	AddScript("HOMEWORK_SCRIPT",			std::make_shared<HomeWorkScript>());
	AddScript("MainMenu_SCRIPT",			std::make_shared<MainMenuScript>());
	AddScript("PAUSEMENU_SCRIPT",			std::make_shared<PauseMenuScript>());
	AddScript("SETTINGSMENU_SCRIPT",		std::make_shared<SettingsMenuScript>());
	
	AddScript("TEXT_SCRIPT",				std::make_shared<TextScript>());
	
	AddScript("Lose_SCRIPT",				std::make_shared<LoseScript>()); 
	AddScript("Win_SCRIPT",					std::make_shared<WinScript>());
	AddScript("ALARMCLOCK_SCRIPT",			std::make_shared<AlarmClockScript>());
	AddScript("BANANA_SCRIPT",              std::make_shared<BananaScript>());
	AddScript("WATERCOOLER_SCRIPT",			std::make_shared<WaterCoolerScript>());
	AddScript("WATER_SCRIPT",				std::make_shared<WaterScript>());
	AddScript("RESULT_SCRIPT",				std::make_shared<ResultScript>());

	AddScript("TUT1_SCRIPT",                std::make_shared<TutorialScript1>());

	AddScript("CURSOR_SCRIPT",              std::make_shared<MouseCursorScript>());
	
	AddScript("CONFIRMQUIT_SCRIPT",         std::make_shared<ConfirmQuitScript>());
	AddScript("WIN_CONDITION_SCRIPT",       std::make_shared<WinConditionScript>());

	// button scripts
	AddScript("BUTTON_SCRIPT",				std::make_shared<ScriptButton>());
	AddScript("BUTTON_START_SCRIPT",		std::make_shared<ScriptButtonStart>());
	AddScript("BUTTON_HOWTOPLAY_SCRIPT",	std::make_shared<ScriptButtonHowToPlay>());
	AddScript("BUTTON_SETTINGS_SCRIPT",		std::make_shared<ScriptButtonSettings>());
	AddScript("BUTTON_CREDITS_SCRIPT",		std::make_shared<ScriptButtonCredits>());
	AddScript("BUTTON_RESUME_SCRIPT",       std::make_shared<ScriptButtonResume>());
	AddScript("BUTTON_QUIT_SCRIPT",			std::make_shared<ScriptButtonQuit>());
	AddScript("BUTTON_RESTART_SCRIPT",		std::make_shared<ScriptButtonRestart>());
	AddScript("BUTTON_MAINMENU_SCRIPT",		std::make_shared<ScriptButtonMainMenu>());
	AddScript("BUTTON_NEXTSTAGE_SCRIPT",	std::make_shared<ScriptButtonNextStage>());
	AddScript("BUTTON_BACK_SCRIPT",			std::make_shared<ScriptButtonBack>());
	AddScript("BUTTON_YES_SCRIPT",          std::make_shared<ScriptButtonYes>());
	AddScript("BUTTON_NO_SCRIPT",           std::make_shared<ScriptButtonNo>());

	AddScript("CHECKBOX_FULLSCREEN_SCRIPT", std::make_shared<ScriptCheckBoxFullScreen>());
	AddScript("CHECKBOX_MUTE_SCRIPT",	    std::make_shared<ScriptCheckBoxMute>());
	AddScript("VOLUME_UP_SCRIPT",           std::make_shared<VolumeUpScript>());
	AddScript("VOLUME_DOWN_SCRIPT",         std::make_shared<VolumeDownScript>());

	AddScript("STARTINGSCREEN_SCRIPT",		std::make_shared<LoadingScreenScript>());
	AddScript("TRANSITION_SCRIPT",			std::make_shared<TransitionScript>());
	AddScript("COMIC_SCRIPT",				std::make_shared<ComicScript>());
}

void Logic::AddLogicComponent(CompId _id)
{
	logicComponentList.insert(_id);
}

void Logic::RemoveLogicComponent(CompId _id)
{
	logicComponentList.erase(_id);
}

void Logic::AddStateMachine(std::string _name, std::shared_ptr<StateMachine> _stateMachine)
{
	stateMachineList[_name] = _stateMachine;
}

void Logic::AddScript(std::string _name, std::shared_ptr<Script> _scriptPtr)
{
	scriptList[_name] = _scriptPtr;
}

void Logic::ClearInventory()
{
	if (scriptList.find("PLAYER_SCRIPT") != std::end(scriptList))
		std::dynamic_pointer_cast<PlayerScript>(scriptList["PLAYER_SCRIPT"])->Exit();
}

std::vector<std::vector<bool>>& Logic::GetAImap()
{
	return aiMap;
}

void Logic::CanDistract()
{
	once = true;
}

int Logic::GetSMStateIndex(const std::string& smname)
{
	return stateMachineList[smname]->GetCurrStateindex();
}

bool Logic::IsPathFindCalled()
{
	return pathFindCalled;
}

// called when path find is called.
void Logic::PathFindCalled()
{
	pathFindCalled = true;
	}
