/******************************************************************************/
/*
	File Name: NPCScript.h
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Set for Teacher NPC behaviour

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "NPCScript.h"
#include "CoreEngine.h"
#include "PathFind.h"
#include "ForceManager.h"
#include "FModSoundManager.h"

#include "Graphics.h"


NPCScript::NPCScript()
	: isRight{ true }, isUp{ true }, dir{ 0 }, bodyPtr{ nullptr }, spritePtr{ nullptr }, alerted{}, sm{ nullptr }, isShadowCreated{ false }, isStunned{ false }
{
}

NPCScript::~NPCScript()
{
	Exit();
}

void NPCScript::Start()
{
	//// Add Transition to States
	//idle->AddTransition(clock);
	//idle->AddTransition(Found1);
	//patrol->AddTransition(toRest);
	//patrol->AddTransition(Found2);
	//distract->AddTransition(RI);

	//// Add Patrol points
	//patrol->AddPatrolPoint(VEC2(-100, 80));
	//patrol->AddPatrolPoint(VEC2(160, 80));

	//// teacher movement force
	//MyForceManager.CreateMovementForces(20000.0f, 0.018f, "teacher");
	////bodyPtr->SetMaxSpeed();

	//// Population to state machine
	//sm->AddState(idle);     // 0
	//sm->AddState(patrol);   // 1
	//sm->AddState(distract); // 2

	//sm->SetStartState(0);

	//// Add State Machine to Logic System
	//LOGIC->AddStateMachine("NPC1", sm);
}

void NPCScript::Init()
{
	/*if (sm)
		sm->SetStartState(0);*/
}

void NPCScript::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	//patrol.UpdatePlayerpos( get owner position )
	// toidle.rest(getpts passed)

	if (CORE->GetPausedStatus() || !MyEvent.inGameStage)
		return;

	// creating shadow
	if (isShadowCreated == false)
	{
		//std::cout << "offset" << gameObjPtr.lock()->has(SpriteComponent)->GetOffset().x << " | " << gameObjPtr.lock()->has(SpriteComponent)->GetOffset().y << std::endl;

		GRAPHICS->CreateEmitter(EmitterType::DYNAMIC,				// type
								gameObjPtr.lock()->has(Transform),	// transform ptr
								VEC2(0.0f, -35.0f) + gameObjPtr.lock()->has(SpriteComponent)->GetOffset() / 2.0f,					// offset
								0.0f,								// startDelay
								EMITTER_INFINITE_LIFETIME,			// lifetime
								EMITTER_INFINITE_LIFETIME, 			// particle lifetime
								2.0f, 								// fade speed
								EMITTER_INFINITE_LIFETIME, 			// emit interval
								1.0f, 								// emit num
								VEC2(0.0f, 0.0f),					// emit area
								0.0f,								// angle
								0.0f,								// movementspeed
								"PlayerShadow");					// archetype

		isShadowCreated = true;
	}



	//std::cout << sm->GetCurrState()->_name << std::endl;

	// ---check surrounding
	/**/
	VEC2 Npos = gameObjPtr.lock()->has(Transform)->GetPosition();
	
	/* if return true, activte link to chase state */
	if (bodyPtr->GetVelocity().x > 0)
	{
		dir = RIGHT;
	}
	else if (bodyPtr->GetVelocity().x < 0)
	{
		dir = LEFT;
	}
	else if (bodyPtr->GetVelocity().y > 0)
	{
		dir = UP;
	}
	else if (bodyPtr->GetVelocity().y < 0)
	{
		dir = DOWN;
	}
	// if current state not chasing, check dir
	
	//CheckAllSide(Npos, 2);
	if (isStunned == false && sm->GetCurrState()->_name != "Chase" && !LOGIC->GetCheat() && DetectedPlayer(Npos, dir, 3.0f * 64.0f))//CheckOneSide(Npos, 2, dir))
	{
		/*std::cout << "GAME OVER" << std::endl;
		Message msg(MessageId::Lose);
		LOGIC->SendMessage(msg);*/
		DetectPlayer* DP;

		if (DP = dynamic_cast<DetectPlayer*>(sm->GetCurrState()->GetRawTransitionIndex("chase")))
		{
			DP->FoundPlayer();
		}
		
		MyEvent.ActivateChase(Npos, gameObjPtr.lock()->GetName());

		if (alerted.find(gameObjPtr.lock()->GetName()) == std::end(alerted))
			alerted[gameObjPtr.lock()->GetName()] = false;

		if (alerted[gameObjPtr.lock()->GetName()] == false)
		{
			if (strcmp(gameObjPtr.lock()->GetType(), "NPC") != 0)
			{
				GRAPHICS->CreateEmitter(EmitterType::DYNAMIC,	// type
					gameObjPtr.lock()->has(Transform),			// transform ptr
					VEC2(0.0f, 100.0f),		// offset
					0.0f,					// startDelay
					1.0f,					// lifetime
					0.8f, 					// particle lifetime
					2.0f, 					// fade speed
					2.0f, 					// emit interval
					1.0f, 					// emit num
					VEC2(0.0f, 0.0f),		// emit area
					0.0f,					// angle
					0.0f,					// movementspeed
					"AlertFeedback");		// archetype
			}
			else
			{
				GRAPHICS->CreateEmitter(EmitterType::DYNAMIC,	// type
					gameObjPtr.lock()->has(Transform),			// transform ptr
					VEC2(0.0f, 150.0f),		// offset
					0.0f,					// startDelay
					1.0f,					// lifetime
					0.8f, 					// particle lifetime
					2.0f, 					// fade speed
					2.0f, 					// emit interval
					1.0f, 					// emit num
					VEC2(0.0f, 0.0f),		// emit area
					0.0f,					// angle
					0.0f,					// movementspeed
					"AlertFeedback");		// archetype
			}

			alerted[gameObjPtr.lock()->GetName()] = true;
		}
	}
	// ---end

	// check for collision
	std::shared_ptr<BoxCollider> colliderptr = gameObjPtr.lock()->has(BoxCollider);
	if (colliderptr && colliderptr->IsCollided())
	{
		//std::cout << "collided" << " | " << colliderptr->GetCollidedObj()->GetName() <<  std::endl;
		CollisionResponse(colliderptr->GetCollidedObj(), colliderptr->GetCollidedType(), colliderptr->GetCollidedAABB(), _dt);
		colliderptr->ResetIsCollide();
	}

	// -------------

	bool IsCurrRight = gameObjPtr.lock()->has(Transform)->GetScale().x > 0;

	// Update State Machine
	//std::cout << gameObjPtr.lock()->GetName() << ", " << sm->GetCurrStateindex() << " | "  << gameObjPtr.lock()->has(Transform)->GetScale().x << std::endl;
	sm->Update(_dt);
	//std::cout << gameObjPtr.lock()->GetName() << ", " << sm->GetCurrStateindex() << std::endl;

	if (sm->updatedSprite == true)
	{
		// reset
		sm->updatedSprite = false;

		if (IsCurrRight == false)
		{
			spritePtr->FlipSprite();
			//IsCurrRight = true;
		}
	}
	//std::cout << "bp2: " << gameObjPtr.lock()->GetName() << ", " << IsCurrRight << " | " << gameObjPtr.lock()->has(Transform)->GetScale().x << std::endl;

	// update animation
	std::shared_ptr<AnimationComponent> animationCompPtr = gameObjPtr.lock()->has(AnimationComponent);
	
	int animationID = animationCompPtr->animationList.at(animationCompPtr->currAnimation);
	std::shared_ptr<Animation> animationPtr = GRAPHICS->GetAnimation(animationID);

	std::string animationName = GRAPHICS->GetAnimationName(animationID);
	//std::cout << animationName << std::endl;

	if (animationName.find("FALLING") != std::string::npos)
	{
		alerted[gameObjPtr.lock()->GetName()] = false;

		if (animationCompPtr->currFrame == animationPtr->lastFrame)
		{
			spritePtr->ChangeGenericAnimation("STUNNED");
			// off collider
			gameObjPtr.lock()->has(BoxCollider)->SetCanCollide(false);
			isStunned = true;
			bodyPtr->SetVelocity(VEC2{ 0.0f, 0.0f });
			if (MyEvent.GetCurrEvent() == EventManager::NPCEvent::CHASE)
				MyEvent.DeActivateChase();
		}
		
	}
	else if (animationName.find("STUNNED") != std::string::npos)
	{
		//std::shared_ptr<BoxCollider> boxColliderCompPtr = ;

		;// gameObjPtr.lock()->has(BoxCollider)->SetCanCollide(false);

		//if (animationCompPtr->currFrame == animationPtr->lastFrame)
		//{
		//	gameObjPtr.lock()->has(SpriteComponent)->ChangeGenericAnimation("GETTING_UP");
		//}
	}
	else if (animationName.find("GETTING_UP") != std::string::npos)
	{
		if (animationCompPtr->currFrame == animationPtr->lastFrame)
		{
			gameObjPtr.lock()->has(SpriteComponent)->ChangeGenericAnimation("IDLE");

			sm->GetCurrState()->UpdateAnimation();
			sm->ActiveSM(true); // unpause SM
			
			if (sm->GetCurrState()->_name == "Chase")
			{
				GiveUpChasing* GC;
				if (GC = dynamic_cast<GiveUpChasing*>(sm->GetCurrState()->GetRawTransitionIndex("giveup")))
				{
					GC->Giveup();
				}
			}
		}
	}
	

	//std::cout << " 1 :" << gameObjPtr.lock()->has(Transform)->GetScale().x << std::endl;

	// updating directiopn of the sprite

	IsCurrRight = gameObjPtr.lock()->has(Transform)->GetScale().x > 0;
	//std::cout << "NPC Velo: " << bodyPtr->GetVelocity().x << " | " << bodyPtr->GetVelocity().y << std::endl;

	// moving left
	if (IsCurrRight && bodyPtr->GetVelocity().x < 0)
	{
		//std::cout << " flip to left" << std::endl;
		isRight = false;
		spritePtr->FlipSprite();
	}
	// moving right
	else if (!IsCurrRight && bodyPtr->GetVelocity().x > 0)
	{
		//std::cout << " flip to right" << std::endl;
		isRight = true;
		spritePtr->FlipSprite();
	}
	//std::cout << "bp3: " << gameObjPtr.lock()->GetName() << ", " << IsCurrRight << " | " << gameObjPtr.lock()->has(Transform)->GetScale().x << std::endl;
	//std::cout << " 2 :" << gameObjPtr.lock()->has(Transform)->GetScale().x << std::endl;

}

void NPCScript::CollisionResponse(std::shared_ptr<GOC> _obj, CollideTypes _type, AABB _aabb, float _dt)
{
	UNREFERENCED_PARAMETER(_aabb);
	UNREFERENCED_PARAMETER(_dt);

	//std::cout << "Collided type: " << static_cast<std::underlying_type_t<CollideTypes>>(_type) << std::endl;
	//std::cout << _obj.lock()->GetName() << std::endl;

	if (_type == CollideTypes::COLLECTABLE)
	{
		const char* name = _obj->GetName();
		//std::cout << "knockknock" << std::endl;
		//std::cout << name << std::endl;

		if (strcmp(name, "Banana") == 0 || static_cast<std::string>(name).find("WaterPuddle") != std::string::npos)
		{
			//std::cout << gameObjPtr.lock()->GetName() << std::endl;
		/*	NPCCOLLIDE message;
			LOGIC->SendMessage(message);*/

			_obj->Destroy();

			if(scoreNum + 25 < 100)
				scoreNum += 25;

			// setting the falling animation
			isRight = gameObjPtr.lock()->has(Transform)->GetScale().x > 0;
			//std::shared_ptr<SpriteComponent> sprPtr = gameObjPtr.lock()->has(SpriteComponent);
			spritePtr->ChangeGenericAnimation("FALLING");
			SOUNDMGR->PlaySoundFromMap("prefectFall");

			sm->ActiveSM(false); // unpause SM

			//if (!isRight)
			//	sprPtr->FlipSprite();

			/*// falling animation
			std::shared_ptr<AnimationComponent> animationCompPtr = gameObjPtr.lock()->has(AnimationComponent);

			int animationID = animationCompPtr->animationList.at(animationCompPtr->currAnimation);
			std::shared_ptr<Animation> animationPtr = GRAPHICS->GetAnimation(animationID);
			std::string animationName = GRAPHICS->GetAnimationName(animationID);

			// update then the animation has reached its last frame
			if (animationCompPtr->currFrame == animationPtr->lastFrame)
			{
				isRight = gameObjPtr.lock()->has(Transform)->GetScale().x > 0;
				//std::shared_ptr<SpriteComponent> spritePtr = gameObjPtr.lock()->has(SpriteComponent);

				if (animationName.find("FALLING") != std::string::npos)
				{
					spritePtr->ChangeGenericAnimation("STUNNED");
				}
				else if (animationName.find("STUNNED") != std::string::npos)
				{
					gameObjPtr.lock()->has(SpriteComponent)->ChangeGenericAnimation("GETTING_UP");
				}
				else
				{
					sm->GetCurrState()->UpdateAnimation();
					sm->ActiveSM(true); // unpause SM
				}

				if (!isRight)
					spritePtr->FlipSprite();

			}*/
		}

		if (strcmp(name, "Alarm Clock") == 0)
		{
			_obj->Destroy();

			NPCCOLLIDE message;
			LOGIC->SendMessage(message);

			if (scoreNum + 25 < 100)
				scoreNum += 25;

			//std::cout << "NPC collide with alarm clock" << std::endl;
			//npc fiddle with clock
		}



		//std::shared_ptr<Animator> animatorPtr = gameObjPtr.lock()->has(Animator);
		//
		//if (animatorPtr->IsAnimationEnd())
		//{

		//	sprPtr->ChangeAnimation("WALKING");

		//	/*gameObjPtr.lock()->has(BoxCollider)->ResetIsCollide();
		//	for (auto obj : MyResource.GetObjMap())
		//	{
		//		if (obj == nullptr)
		//			continue;
		//		if (strcmp(obj->GetName(), "Banana") == 0)
		//			obj->Destroy();
		//	}*/
		//}

	}
	else if (_type == CollideTypes::DOOR && strcmp(gameObjPtr.lock()->GetName(), "Teacher") == 0)
	{
		_obj->Destroy();
		SOUNDMGR->PlaySoundFromMap("doorOpen");
	}
}

void NPCScript::UpdateAnimation(std::string _animationStr)
{
	UNREFERENCED_PARAMETER(_animationStr);
}

void NPCScript::LoadResource()
{
	FACTORY->CreateArcheType("PlayerShadow");
}

void NPCScript::Exit()
{
	alerted.clear();
	isShadowCreated = false;
	//if(MyEvent.GetCurrEvent() == EventManager::NPCEvent::CHASE)
	//	MyEvent.DeActivateChase();
	isStunned = false;

	if (sm)
		sm->ResetSM();
}

void NPCScript::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
{
	gameObjPtr =	_gameObjPtr;
	bodyPtr =		_gameObjPtr->has(Body);
	spritePtr =		_gameObjPtr->has(SpriteComponent);

	sm =			LOGIC->GetStateMachine(_gameObjPtr->has(LogicComponent)->GetSMName());
	if (sm)
		sm->SetOwner(_gameObjPtr);

}
