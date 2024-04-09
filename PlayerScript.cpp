/*****************************************************************************/
/*
File Name:		PlayerScript.cpp
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [65%]
				Alvin Tay Qin Wen	 [25%]
				Lee Shi Lin			 [10%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#include "stdafx.h"
#include "PlayerScript.h"
#include "graphics.h"
#include "TextScript.h"
#include "CoreEngine.h"
#include "Logic.h"
#include "TutorialScript1.h" // extern IntroDialogDone
#include "FireAlarmScript.h" // extern inFireAlarmDialog
#include "WinScript.h" //extern inWinDialog
#include "FModSoundManager.h"
#include "WindowsSystem.h"
#include <vector>
#include "Score.h"

int scoreNum = 0;

bool inDialog = false;

bool IsBeingChased = false;

bool isInventoryFull = false;


PlayerScript::PlayerScript()
	: inputPtr{ INPUTMGR }, isFacingRight{ true },
	isFacingUp{ false },
	firstBananaDrop{ false }, playBananaDropText{ false },
	firstGotBanana{ false }, gotBanana{ false },
	firstClockDrop{ false }, gotClock{ false }, maxScore{ 100 },
	scoreRendererPtr{ nullptr },
	scoreStartPt{ 1100, 655 },
	 isShadowCreated{ false },
	trashFrame{0}, smokeDelay{0.0f}
{
	scoreText = "Score:  ";
}

PlayerScript::~PlayerScript()
{
	Exit();
}

void PlayerScript::Start()
{
	//inventory.Initialize();
}

void PlayerScript::Init()
{
	inventory.Initialize();
	SOUNDMGR->PauseAllSounds();
	SOUNDMGR->GetMap().erase("menuMusic");
	SOUNDMGR->PlaySoundLoopFromMap("BGM");
	SOUNDMGR->setForcePause("BGM", false);
	scoreRendererPtr = GRAPHICS->GetTextRenderer();
	isInventoryFull = false;
	sweatPtr = LOGIC->CreateObjectAt(VEC2(0,0), 0, "SweatVFX", "svfx", false)->has(SpriteComponent);
	sweatPtr.lock()->SetActive(false);
}

void PlayerScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	
	//scoreRendererPtr->AddText(scoreText + std::to_string(scoreNum), scoreStartPt);
	//TrashVFXUpdate();


	std::string currSubTexture = spritePtr->GetCurrSubTextureName();

	if (!inventory.isItemListFull())
		inventory.SetInvenBarHalfTransparent();
	else
		inventory.SetInvenBarLessTransparent();
	
	if (inventory.isNoteListEmpty())
		inventory.SetNoteBarHalfTransparent();
	else
		inventory.SetNoteBarLessTransparent();

	// reset
	bool isMoving = false;

	std::shared_ptr<Camera> camPtr = GRAPHICS->GetMainCameraPtr();
	
	if (isShadowCreated == false)
	{
		std::cout << "Create player shadow emitter" << std::endl;
		GRAPHICS->CreateEmitter(EmitterType::DYNAMIC,	// type
			transformPtr,			// transform ptr
			VEC2(0.0f, -50.0f),		// offset
			0.0f,					// startDelay
			EMITTER_INFINITE_LIFETIME,					// lifetime
			EMITTER_INFINITE_LIFETIME, 					// particle lifetime
			2.0f, 					// fade speed
			EMITTER_INFINITE_LIFETIME, 					// emit interval
			1.0f, 					// emit num
			VEC2(0.0f, 0.0f),		// emit area
			0.0f,					// angle
			0.0f,					// movementspeed
			"PlayerShadow");		// archetype

		isShadowCreated = true;
	}

	if (!CORE->GetPausedStatus())
	{
		// movement
		if (!inDialog && !GRAPHICS->GetMainCameraPtr()->GetMoveListSize())
		{
			if (INPUTMGR->GetKeyHold(MVK_UP) || INPUTMGR->GetKeyHold(MVK_W))
			{
				//Using Map
				bodyPtr->ActivateForces("player2Up");
				bodyPtr->ActivateForces("drag");

				camPtr->SetAlign(CAMERA_DIRECTION::UP);
				//Using Vector
				//bodyPtr->ActivateForces(UP);
				//bodyPtr->ActivateForces(DRAGFORCE);
				isFacingUp = true;
				isMoving = true;
			}

			else if (INPUTMGR->GetKeyHold(MVK_DOWN) || INPUTMGR->GetKeyHold(MVK_S))
			{
				//Using Map
				bodyPtr->ActivateForces("player2Down");
				bodyPtr->ActivateForces("drag");

				camPtr->SetAlign(CAMERA_DIRECTION::DOWN);

				//Using Vector
				//bodyPtr->ActivateForces(DOWN);
				//bodyPtr->ActivateForces(DRAGFORCE);
				isFacingUp = false;
				isMoving = true;
			}
			else 
			{
				isFacingUp = false;
				//camPtr->SetAlign(CAMERA_DIRECTION::NEUTRAL_Y);
			}

			if (INPUTMGR->GetKeyHold(MVK_LEFT) || INPUTMGR->GetKeyHold(MVK_A))
			{
				//std::cout << "Left" << std::endl;
				//Using Map
				bodyPtr->ActivateForces("player2Left");
				bodyPtr->ActivateForces("drag");

				camPtr->SetAlign(CAMERA_DIRECTION::RIGHT);

				//Using Vector
				//bodyPtr->ActivateForces(LEFT);
				//bodyPtr->ActivateForces(DRAGFORCE);

				if (isFacingRight)
					isFacingRight = false;

				isMoving = true;
			}

			else if (INPUTMGR->GetKeyHold(MVK_RIGHT) || INPUTMGR->GetKeyHold(MVK_D))
			{
				//std::cout << "Right" << std::endl;
				//Using Map
				bodyPtr->ActivateForces("player2Right");
				bodyPtr->ActivateForces("drag");

				camPtr->SetAlign(CAMERA_DIRECTION::LEFT);

				//Using Vector
				//bodyPtr->ActivateForces(RIGHT);
				//bodyPtr->ActivateForces(DRAGFORCE);

				if (!isFacingRight)
					isFacingRight = true;

				isMoving = true;
			}
			else
			{
				//camPtr->SetAlign(CAMERA_DIRECTION::NEUTRAL_X);
			}

			/*if (isMoving)
			{
				walkSoundCounter += _dt;
				if (walkSoundCounter > 1.0)
				{
					SOUNDMGR->PlaySoundFromMap("pickUp");
					walkSoundCounter = 0;
				}
			}*/
			if (isMoving == false)
			{
				bodyPtr->DeactivateForces("playerRight");
				bodyPtr->DeactivateForces("playerLeft");
				bodyPtr->DeactivateForces("playerUp");
				bodyPtr->DeactivateForces("playerDown");
				bodyPtr->DeactivateForces("drag");

				bodyPtr->SetVelocity(VEC2(0, 0));
			}


		}
		

		//std::cout << "Player Velo: " << bodyPtr->GetVelocity().x << " | " << bodyPtr->GetVelocity().y << std::endl;
		
		// walking smoke (NEEDS TO BE UPDATED)
		if (INPUTMGR->GetKeyHold(MVK_UP) || INPUTMGR->GetKeyHold(MVK_W) || INPUTMGR->GetKeyHold(MVK_DOWN) || INPUTMGR->GetKeyHold(MVK_S) ||
			INPUTMGR->GetKeyHold(MVK_LEFT) || INPUTMGR->GetKeyHold(MVK_A) || INPUTMGR->GetKeyHold(MVK_RIGHT) || INPUTMGR->GetKeyHold(MVK_D))
		{
			smokeDelay += _dt;
			if (smokeDelay > 0.5f)
			{
				if (isFacingRight)
				{
					GRAPHICS->CreateEmitter(EmitterType::DYNAMIC,	// type
						transformPtr,			// transform ptr
						VEC2(-20.0f, -50.0f),		// offset
						0.0f,					// startDelay
						0.5f,					// lifetime
						0.3f, 					// particle lifetime
						2.0f, 					// fade speed
						0.5f, 					// emit interval
						1.0f, 					// emit num
						VEC2(0.0f, 0.0f),		// emit area
						0.0f,					// angle
						0.0f,					// movementspeed
						"FeetSmokeVFX");		// archetype
				}
				else
				{
					GRAPHICS->CreateEmitter(EmitterType::DYNAMIC,	// type
						transformPtr,			// transform ptr
						VEC2(20.0f, -50.0f),		// offset
						0.0f,					// startDelay
						0.5f,					// lifetime
						0.3f, 					// particle lifetime
						2.0f, 					// fade speed
						0.5f, 					// emit interval
						1.0f, 					// emit num
						VEC2(0.0f, 0.0f),		// emit area
						0.0f,					// angle
						0.0f,					// movementspeed
						"FeetSmokeVFX");		// archetype
				}
				smokeDelay = 0;
			}
			
		}
		



	}

	// collision check
	if (colliderPtr && colliderPtr->IsCollided())
	{
		auto collidedlist = colliderPtr->GetCollidedList();
		for (auto [id, aabb] : collidedlist)
		{
			std::shared_ptr<GOC> obj = MyResource.GetObjectWithID(id);
			CollisionResponse(obj, obj->has(BoxCollider)->GetMyType(), aabb);
		}
		colliderPtr->ResetIsCollide();
	}
	else
	{
 		interactableFeedback.Destroy();
	}
	
 	// --- updating animation ---
	if (bodyPtr->GetVelocity().x == 0.0f && bodyPtr->GetVelocity().y == 0.0f)
	{
		isMoving = false;
	}
	else
		isMoving = true;

	if (!isMoving)
	{	
		// --- IDLE ---
		if (!isFacingRight)
		{
			spritePtr->ChangeAnimation("PLAYER_IDLE_LEFT");
		}
		else
		{
			spritePtr->ChangeAnimation("PLAYER_IDLE_RIGHT");
		}
	}
	else
	{
		// --- WALKING --- 
		if (!isFacingRight)
		{
			if (isFacingUp)
			{
				spritePtr->ChangeAnimation("PLAYER_WALKUP_LEFT");
			}
			else
			{
				spritePtr->ChangeAnimation("PLAYER_WALKING_LEFT");
			}
		}
		else
		{
			if (isFacingUp)
			{
				spritePtr->ChangeAnimation("PLAYER_WALKUP_RIGHT");
			}
			else
			{
				spritePtr->ChangeAnimation("PLAYER_WALKING_RIGHT");
			}
		}
		// running
		if (MyEvent.GetCurrEvent() == EventManager::NPCEvent::CHASE)
		{
			sweatPtr.lock()->GetBase()->has(Transform)->SetPosition(VEC2(transformPtr->GetPosition().getx(), transformPtr->GetPosition().gety() + 20.0f));
			sweatPtr.lock()->SetActive(true);
		}
		else if (MyEvent.GetCurrEvent() == EventManager::NPCEvent::NONE)
		{
			sweatPtr.lock()->SetActive(false);
		}
	}

	// rigg
	


	// --- Use Item ---
	if (INPUTMGR->GetKeyRelease(MVK_Q) && !inDialog && IntroDialogDone && !inWinDialog) // use item
	{
		SOUNDMGR->PlaySoundFromMap("drop");
		// create smoke when item is placed.
		if (inventory.UseCurrItem(VEC2(transformPtr->GetPosition().x, transformPtr->GetPosition().y - 50.0f), trashFrame))
		{
			smokeFeedback.Create(VEC2(transformPtr->GetPosition().x, transformPtr->GetPosition().y - 50.0f));
		}
		if (strcmp(inventory.GetCurrSelectedSlotGOCName(), "Banana") == 0)
		{
			if (strcmp(CORE->GetCurrLevel().c_str(), "Stage2v2") == 0)
			{
				isInventoryFull = false;
				if (firstBananaDrop == false) //if press q and havent dropped banana yet
				{
					playBananaDropText = true; //enable the drop banana bool to let the text play
					firstBananaDrop = true;	//set the initial bool to true; will never go into this loop again
				}

				//if we're dropping the banana
				if (playBananaDropText) //this is now true, do the following
				{
					CORE->SetPausedStatus(true);
					//play banana squishy sound?
					MyEvent.textPtr.lock()->SetActive(true);
					MyEvent.boyPtr.lock()->SetActive(true);
					MyEvent.tintPtr.lock()->SetActive(true);

					//sound
					SOUNDMGR->PlaySoundFromMap("boyVoice");

					MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/BoyComments/DropBanana.json");
					inDialog = true;
				}
			}
			
		}
		else if (strcmp(inventory.GetCurrSelectedSlotGOCName(), "Clock") == 0)
		{
			if (firstClockDrop == false) //if press q and havent dropped banana yet
			{
				playClockDropText = true; //enable the drop banana bool to let the text play
				firstClockDrop = true;	//set the initial bool to true; will never go into this loop again
			}

			//if we're dropping the clock
			if (playClockDropText) //this is now true, do the following
			{
				CORE->TogglePausedStatus();
				//play banana squishy sound?
				/*textObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Text", "Banana Drop Text");
				boySpritePtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "PlayerSprite", "DialogPicture");
				tintPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Tint", "DialogPicture");*/
				MyEvent.textPtr.lock()->SetActive(true);
				MyEvent.boyPtr.lock()->SetActive(true);
				MyEvent.tintPtr.lock()->SetActive(true);


				//sound
				SOUNDMGR->PlaySoundFromMap("boyVoice");

				MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/BoyComments/DropClock.json");
				inDialog = true;
			}
		}
	}

	if (INPUTMGR->GetKeyPressed(MVK_LBUTTON))
	{
		SOUNDMGR->PlaySoundFromMap("click");
	}

	if (INPUTMGR->GetKeyPressed(MVK_M))
	{
		std::cout << "Total notes: " << MyScore.getTotalNotesCollected() << std::endl;
		std::cout << "Stage notes: " << MyScore.getStageNotesCollected() << std::endl;
		std::cout << "Total stage clears: " << MyScore.getTotalStageClears() << std::endl;
		std::cout << "Stage clear: " << MyScore.getStageCleared() << std::endl;
	}

	{//we're pressing enter/lmb to get out of the dialog
		if ( inDialog && (INPUTMGR->GetKeyPressed(MVK_RETURN) || INPUTMGR->GetKeyPressed(MVK_SPACE) || INPUTMGR->GetKeyPressed(MVK_LBUTTON) ) )
		{
			CORE->SetPausedStatus(false);

			//text->Destroy();

			/*if (std::shared_ptr<GOC> boySpr = boySpritePtr.lock())
				boySpr->Destroy();

			if (std::shared_ptr<GOC> tint = tintPtr.lock())
				tint->Destroy();*/

			MyEvent.textPtr.lock()->SetActive(false);
			MyEvent.boyPtr.lock()->SetActive(false);
			MyEvent.tintPtr.lock()->SetActive(false);

			if (playBananaDropText)
				playBananaDropText = false;
			else if (gotBanana)
				gotBanana = false;
			else if (gotClock)
				gotClock = false;
			else if (playClockDropText)
				playClockDropText = false;

			inDialog = false; //no longer in dialog
		}
	}

	// update smoke
	smokeFeedback.Update(_dt);
	
}

void PlayerScript::Exit()
{
	inventory.Clear();
	inDialog = false;
	firstBananaDrop = false;
	playBananaDropText = false;
	firstGotBanana = false;
	gotBanana = false;
	firstClockDrop = false;
	playClockDropText = false;
	firstGotClock = false;
	gotClock = false;
	IntroDialogDone = false;
	scoreNum = 0;
	isShadowCreated = false;
	trashFrame = 0;
	//walkSoundCounter = 0;
	noteCheck.clear();
	smokeDelay = 0;
}

void PlayerScript::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
{
	gameObjPtr = _gameObjPtr;
	bodyPtr = _gameObjPtr->has(Body);
	transformPtr = _gameObjPtr->has(Transform);
	spritePtr = _gameObjPtr->has(SpriteComponent);
	colliderPtr = _gameObjPtr->has(BoxCollider);
	GRAPHICS->GetMainCameraPtr()->SetFollowPtr(transformPtr);
}


void PlayerScript::CollisionResponse(std::shared_ptr<GOC> _obj, CollideTypes _type, AABB _aabb)
{

	if (_type == CollideTypes::COLLECTABLE)
	{
		// interactable feedbacks
		if (strcmp(_obj->GetType(), "Notes") != 0) {
			if (strcmp(_obj->GetType(), "Dustbin") == 0)
				interactableFeedback.Create(_obj->has(Transform)->GetPosition() + VEC2(0.0f, (_obj->has(Transform)->GetScale().y) / 2));
			else
				interactableFeedback.Create(_obj->has(Transform)->GetPosition() + VEC2(0.0f, 105.0f));
			interactableFeedback.Update(0.0f);
		}

		//picking up something
		if (INPUTMGR->GetKeyRelease(MVK_E) && !inDialog)
		{
			
			//std::cout << i << std::endl;

			SOUNDMGR->PlaySoundFromMap("pickUp");
			//if we pickin up a banana
			if (strcmp(_obj->GetType(), "Dustbin") == 0)
			{
				//// Check if boy have pick up from the same dustbin
				//std::vector<GOCId>::iterator it;
				trashFrame = rand() % 6;
				//trashvfx
				//trashVFXPtr = LOGIC->CreateObjectAt(VEC2(_obj->has(Transform)->GetPosition().getx(), _obj->has(Transform)->GetPosition().gety()), 0, "TrashVFX", "tvfx", false)->has(AnimationComponent);
				GRAPHICS->CreateEmitter(EmitterType::DYNAMIC,	// type
					_obj->has(Transform),			// transform ptr
					VEC2(0.0f, 0.0f),		// offset
					0.0f,					// startDelay
					0.8f,					// lifetime
					0.8f, 					// particle lifetime
					2.0f, 					// fade speed
					2.0f, 					// emit interval
					1.0f, 					// emit num
					VEC2(0.0f, 0.0f),		// emit area
					0.0f,					// angle
					0.0f,					// movementspeed
					"TrashVFX");		// archetype
				//it = find(bananaCheck.begin(), bananaCheck.end(), _obj.lock()->GetId());
				//if (it == bananaCheck.end())
				//	notInsideVec = true;

				//if (notInsideVec) 
				if(strcmp(CORE->GetCurrLevel().c_str(), "Stage2v2") == 0)
				{

					if (!firstGotBanana)//if its the first time, this is true. subsequent presses it wont go in this
					{
						firstGotBanana = true;
						gotBanana = true;
					}

					if (gotBanana) //this is now true, do the following
					{
						CORE->SetPausedStatus(true);

						//dialog box, player sprite, tint
						/*textObjPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Text", "Banana Drop Text");
						boySpritePtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "PlayerSprite", "DialogPicture");
						tintPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Tint", "DialogPicture");*/
						MyEvent.textPtr.lock()->SetActive(true);
						MyEvent.boyPtr.lock()->SetActive(true);
						MyEvent.tintPtr.lock()->SetActive(true);

						//sound
						SOUNDMGR->PlaySoundFromMap("boyVoice");
						MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/BoyComments/GetBanana.json");
						inDialog = true;
					}

					//_obj.lock()->Destroy();
					
				}
				if (!inventory.isItemListFull())
				{
					
					
					std::shared_ptr<GOC> tmpObj = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Banana", "Banana");
					tmpObj->has(SpriteComponent)->SetCurrFrame(trashFrame);
					//std::cout << "current frame is: " << i << std::endl;
					inventory.AddItem(tmpObj);
					
					
					//isInventoryFull = true;
					//insert to vector
					//bananaCheck.push_back(_obj.lock()->GetId());
				}
			}
			//if we pickin up a clock
			else if (strcmp(_obj->GetType(), "AlarmClockGround") == 0)
			{
				if (!firstGotClock)//goes in first time, otherwise wont
				{
					firstGotClock = true;
					gotClock = true;
				}

				if (gotClock) //this is now true, do the following
				{
					CORE->SetPausedStatus(true);
					//play banana squishy sound?
					MyEvent.textPtr.lock()->SetActive(true);
					MyEvent.boyPtr.lock()->SetActive(true);
					MyEvent.tintPtr.lock()->SetActive(true);

					//sound
					SOUNDMGR->PlaySoundFromMap("boyVoice");
					MyEvent.textscriptPtr.lock()->Serialize(CORE->mPath + "Text/BoyComments/GetClock.json");
					inDialog = true;
				}
				_obj->Destroy();
				std::shared_ptr<GOC> tmpObj = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "AlarmClock", "Clock");
				tmpObj->has(SpriteComponent)->SetCurrFrame(8);
				inventory.AddItem(tmpObj);
			}
			else if (strcmp(_obj->GetName(), "WaterCooler") == 0)
			{
				/* ---------------------------------
					Interacting with the water cooler will "add more water" into the bottle if there is.
				--------------------------------- */

				//// search inventory for water bottle
				//std::weak_ptr<GOC> waterBottlePtr = inventory.GetItem("WaterBottle");

				//// if there is a water bottle in the inventory
				//if (waterBottlePtr.lock() != nullptr)
				//{
				//	std::shared_ptr<SpriteComponent> spriteCompPtr = waterBottlePtr.lock()->has(SpriteComponent);

				//	// check if it is already at max
				//	if (spriteCompPtr->GetCurrFrameNum() + 1 < spriteCompPtr->GetMaxFrame())
				//		spriteCompPtr->SetCurrentFrame(spriteCompPtr->GetCurrFrameNum() + 1);
				//}
			}
			else if(strcmp(_obj->GetType(), "AlarmClockRinging")==0)
			{

			}
			else if (static_cast<std::string>(_obj->GetType()).find("WaterPuddle") != std::string::npos)
			{

			}
			/*else if(strcmp(_obj.lock()->GetType(), "Notes") == 0)
			{
				if (!inventory.isNoteListFull())
				{
					_obj.lock()->Destroy();
					std::shared_ptr<GOC> tmpObj = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Notes", "Note");
					tmpObj->has(SpriteComponent)->SetCurrFrame(10);
					inventory.AddNote(tmpObj);
				}
			} */
			else if (strcmp(_obj->GetType(), "OpenLocker") == 0)
			{
					if (!inventory.isNoteListFull())
					{
						std::vector<GOCId>::iterator it = find(noteCheck.begin(), noteCheck.end(), _obj->GetId());
						//std::cout << (bool)(it == noteCheck.end()) << std::endl;
						if (it == noteCheck.end())
						{
							GRAPHICS->CreateEmitter(EmitterType::DYNAMIC,	// type
								transformPtr,			// transform ptr
								VEC2(20.0f, 60.0f),		// offset
								0.0f,					// startDelay
								0.5f,					// lifetime
								0.4f, 					// particle lifetime
								2.0f, 					// fade speed
								2.0f, 					// emit interval
								1.0f, 					// emit num
								VEC2(0.0f, 0.0f),		// emit area
								0.0f,					// angle
								0.0f,					// movementspeed
								"BlueStarVFX");		// archetype
							std::shared_ptr<GOC> tmpObj = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Notes", "Note");
							tmpObj->has(SpriteComponent)->SetCurrFrame(10);
							inventory.AddNote(tmpObj);
							MyScore.incrementNotes();

							noteCheck.push_back(_obj->GetId());
							_obj->has(BoxCollider)->SetCanCollide(false);
							_obj->has(BoxCollider)->ResetIsCollide();
						}

					}
			}
			else
				inventory.AddItem(_obj);
		}

		// collide with item
		if (strcmp(_obj->GetType(), "Notes") == 0)
		{
			if (!inventory.isNoteListFull())
			{
				GRAPHICS->CreateEmitter(EmitterType::DYNAMIC,	// type
					transformPtr,			// transform ptr
					VEC2(20.0f, 60.0f),		// offset
					0.0f,					// startDelay
					0.5f,					// lifetime
					0.4f, 					// particle lifetime
					2.0f, 					// fade speed
					2.0f, 					// emit interval
					1.0f, 					// emit num
					VEC2(0.0f, 0.0f),		// emit area
					0.0f,					// angle
					0.0f,					// movementspeed
					"BlueStarVFX");		// archetype

				_obj->Destroy();
				std::shared_ptr<GOC> tmpObj = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Notes", "Note");
				tmpObj->has(SpriteComponent)->SetCurrFrame(10);
				inventory.AddNote(tmpObj);
				MyScore.incrementNotes();
			}
		}
		/*
		if (INPUTMGR->GetKeyRelease(MVK_E))
		{
			if (strcmp(_obj->GetType(), "OpenLocker") == 0)
			{
				if (!inventory.isNoteListFull())
				{
					it = find(noteCheck.begin(), noteCheck.end(), _obj->GetId());
					if (it != noteCheck.end())
					{
						std::shared_ptr<GOC> tmpObj = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "Notes", "Note");
						tmpObj->has(SpriteComponent)->SetCurrFrame(10);
						inventory.AddNote(tmpObj);
						MyScore.incrementNotes();
					}

				}
			}
			
		}*/
	}
	else if (_type == CollideTypes::ENVIRONMENT || _type == CollideTypes::DOOR)
	{
		VEC2 center = colliderPtr->GetCenterCoords();
		VEC2 halflengths = colliderPtr->GetHalfLengths();
		VEC2 aabbMin1(center - halflengths);
		VEC2 aabbMax1(center + halflengths);

		VEC2 velo = bodyPtr->GetVelocity();

		VEC2 finalVec = bodyPtr->GetPos();

		//if ((aabbMin1.y > _aabb.minPt.y && aabbMax1.y < _aabb.maxPt.y))
		if (velo.x != 0.0f)
		{
			// left collide
			if (aabbMin1.x < _aabb.maxPt.x && aabbMax1.x > _aabb.maxPt.x)
			{
				if (velo.x <= 0.0f)
				{
					finalVec.x = _aabb.maxPt.x + halflengths.x + 1.0f;
					bodyPtr->SetVelocityX(0.0f);
				}
			}

			// right collide
			else if (aabbMax1.x > _aabb.minPt.x && aabbMin1.x < _aabb.minPt.x)
			{
				if (velo.x >= 0.0f)
				{
					finalVec.x = _aabb.minPt.x - halflengths.x - 1.0f;
					bodyPtr->SetVelocityX(0.0f);
				}
			}
		}

		//if ((aabbMin1.x > _aabb.minPt.x && aabbMax1.x < _aabb.maxPt.x))
		if (velo.y != 0.0f)
		{
			// bottom collide
			if (aabbMin1.y < _aabb.maxPt.y && aabbMax1.y > _aabb.maxPt.y)
			{
				if (velo.y <= 0.0f)
				{
					finalVec.y = _aabb.maxPt.y + halflengths.y + 1.0f - colliderPtr->GetOffSets().y;
					bodyPtr->SetVelocityY(0.0f);
				}
			}

			// top collide
			else if (aabbMax1.y > _aabb.minPt.y && aabbMin1.y < _aabb.minPt.y)
			{
				if (velo.y >= 0.0f)
				{
					finalVec.y = _aabb.minPt.y - halflengths.y - 1.0f - colliderPtr->GetOffSets().y;
					bodyPtr->SetVelocityY(0.0f);
				}
			}
		}

		transformPtr->SetPosition(finalVec);
		//colliderPtr->m_centerCoords = finalVec - colliderPtr->GetOffSets();
		bodyPtr->SetPosition(finalVec);
	}
	else if (_type == CollideTypes::NPC)
	{
		// GG - Set to lose screen

		scoreNum = 0;
		Message msg(MessageId::Lose);
		LOGIC->SendMessageW(msg);
	}
	else if (_type == CollideTypes::INTERACTABLE)
	{
		// interactable feedbacks
		interactableFeedback.Create(_obj->has(Transform)->GetPosition() + VEC2(0.0f, 45.0f));
		interactableFeedback.Update(0.0f);
	}
	
	if (_type != CollideTypes::COLLECTABLE && _type != CollideTypes::INTERACTABLE)
		interactableFeedback.Destroy();

	//colliderPtr->ResetIsCollide();
}

void PlayerScript::CollectItem()
{
	//inventory.AddItem();
}

void PlayerScript::LoadResource()
{
	interactableFeedback.LoadResource();
	smokeFeedback.LoadResource();
	FACTORY->CreateArcheType("AlertFeedback");
	FACTORY->CreateArcheType("PlayerSprite");
	FACTORY->CreateArcheType("WalkingSmoke");
	FACTORY->CreateArcheType("Tint");
	FACTORY->CreateArcheType("PlayerShadow");
	FACTORY->CreateArcheType("Notes");
	FACTORY->CreateArcheType("BackgroundC");
	FACTORY->CreateArcheType("BackgroundF");
	FACTORY->CreateArcheType("SweatVFX");
	FACTORY->CreateArcheType("TrashVFX");
	FACTORY->CreateArcheType("FeetSmokeVFX");
	FACTORY->CreateArcheType("BlueStarVFX");
}
