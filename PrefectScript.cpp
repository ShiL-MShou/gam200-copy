#include "stdafx.h"
#include "PrefectScript.h"

#include "Logic.h"
#include "CoreEngine.h"
#include "ForceManager.h"


PrefectScript::PrefectScript()
	: sm{ std::make_shared<StateMachine>() }
{
}

PrefectScript::~PrefectScript()
{
}


void PrefectScript::Start()
{
	// Add Transition to States
	/*/
	idle->AddTransition(clock);
	idle->AddTransition(Found1);
	patrol->AddTransition(toRest);
	patrol->AddTransition(Found2);
	distract->AddTransition(RI);
	/*/
	//idle->AddTransition("walk", clock);
	//idle->AddTransition("distracted", Found1);
	//idle->AddTransition("chase", seePlayI);

	//patrol->AddTransition("idling", toRest);
	//patrol->AddTransition("distracted", Found2);
	//patrol->AddTransition("chase", seePlayP);

	//distract->AddTransition("stopDistract", RI);
	//distract->AddTransition("chase", seePlayD);

	//chase->AddTransition("giveup", stop);

	//// Add Patrol points
	//patrol->AddPatrolPoint(VEC2(2000, 700));
	//patrol->AddPatrolPoint(VEC2(3000, 700));

	//// teacher movement force
	//MyForceManager.CreateMovementForces(20000.0f, 0.018f, "teacher");
	////bodyPtr->SetMaxSpeed();

	//// Population to state machine
	//sm->AddState(idle);     // 0
	//sm->AddState(patrol);   // 1
	//sm->AddState(distract); // 2

	//sm->SetStartState(0);

	//// Add State Machine to Logic System
	//LOGIC->AddStateMachine("PREFECT1", sm);
}
//
//void PrefectScript::Update(float _dt)
//{
//	UNREFERENCED_PARAMETER(_dt);
//
//	if (CORE->GetPausedStatus())
//		return;
//
//	// updating directiopn of the sprite
//	if (isRight && bodyPtr->GetVelocity().x < 0)
//	{
//		spritePtr->FlipSprite();
//	}
//	else if (!isRight && bodyPtr->GetVelocity().x > 0)
//	{
//		spritePtr->FlipSprite();
//	}
//
//	// ---check surrounding
//	/* if return true, activte link to chase state */
//	if (isRight && bodyPtr->GetVelocity().x < 0)
//	{
//		dir = RIGHT;
//	}
//	else if (bodyPtr->GetVelocity().x > 0)
//	{
//		dir = LEFT;
//	}
//	else if (bodyPtr->GetVelocity().y < 0)
//	{
//		dir = UP;
//	}
//	else if (bodyPtr->GetVelocity().y > 0)
//	{
//		dir = DOWN;
//	}
//
//	// to update?
//	VEC2 Npos = gameObjPtr.lock()->has(Transform)->GetPosition();
//	CheckAllSide(Npos, 2);
//
//	std::shared_ptr<BoxCollider> colliderptr = gameObjPtr.lock()->has(BoxCollider);
//	if (colliderptr && colliderptr->IsCollided())
//	{
//		CollisionResponse(colliderptr->GetCollidedObj(), colliderptr->GetCollidedType(), colliderptr->GetCollidedAABB(), _dt);
//		colliderptr->ResetIsCollide();
//	}
//
//	// -------------
//	std::shared_ptr<AnimationComponent> animationCompPtr = gameObjPtr.lock()->has(AnimationComponent);
//
//	int animationID = animationCompPtr->animationList.at(animationCompPtr->currAnimation);
//	std::shared_ptr<Animation> animationPtr = GRAPHICS->GetAnimation(animationID);
//
//	std::string animationName = GRAPHICS->GetAnimationName(animationID);
//	if (animationName == "FALLING")
//	{
//		if (animationCompPtr->currFrame == animationPtr->lastFrame)
//		{
//			sm->GetCurrState()->UpdateAnimation();
//			sm->ActiveSM(true); // unpause SM
//		}
//
//	}
//
//}
//
//void PrefectScript::Exit()
//{
//
//}
//
//void PrefectScript::SetOwner(std::shared_ptr<GOC> _gameObjPtr)
//{
//	gameObjPtr	= _gameObjPtr;
//
//	bodyPtr		= _gameObjPtr->has(Body);
//	spritePtr	= _gameObjPtr->has(SpriteComponent);
//
//}
//
//void PrefectScript::CollisionResponse(std::weak_ptr<GOC> _obj, CollideTypes _type, AABB _aabb, float _dt)
//{
//	UNREFERENCED_PARAMETER(_aabb);
//	UNREFERENCED_PARAMETER(_dt);
//
//	if (_type == CollideTypes::COLLECTABLE)
//	{
//		std::shared_ptr<GOC> obj = _obj.lock();
//		const char* name = obj->GetName();
//		
//		if (strcmp(name, "Banana") == 0)
//		{
//			NPCCOLLIDE message;
//			LOGIC->SendMessage(message);
//
//			if (scoreNum + 25 < 100)
//				scoreNum += 25;
//
//			// setting the falling animation
//			bool tmpIsRight = gameObjPtr.lock()->has(Transform)->GetScale().x > 0;
//			std::shared_ptr<SpriteComponent> sprPtr = gameObjPtr.lock()->has(SpriteComponent);
//			sprPtr->ChangeAnimation("PREFECT_FALLING");
//
//			if (!tmpIsRight)
//				sprPtr->FlipSprite();
//
//			// falling animation
//			std::shared_ptr<AnimationComponent> animationCompPtr = gameObjPtr.lock()->has(AnimationComponent);
//
//			int animationID = animationCompPtr->animationList.at(animationCompPtr->currAnimation);
//			std::shared_ptr<Animation> animationPtr = GRAPHICS->GetAnimation(animationID);
//			std::string animationName = GRAPHICS->GetAnimationName(animationID);
//
//			// update then the animation has reached its last frame
//			if (animationCompPtr->currFrame == animationPtr->lastFrame)
//			{
//				bool isRight = gameObjPtr.lock()->has(Transform)->GetScale().x > 0;
//				//std::shared_ptr<SpriteComponent> spritePtr = gameObjPtr.lock()->has(SpriteComponent);
//
//				if (animationName == "PREFECT_FALLING")
//				{
//					spritePtr->ChangeAnimation("PREFECT_STUNNED");
//				}
//				else if (animationName == "PREFECT_STUNNED")
//				{
//					gameObjPtr.lock()->has(SpriteComponent)->ChangeAnimation("PREFECT_GETUP");
//				}
//				else
//				{
//					sm->GetCurrState()->UpdateAnimation();
//					sm->ActiveSM(true); // unpause SM
//				}
//
//				if (!isRight)
//					spritePtr->FlipSprite();
//
//			}
//		}
//
//		if (strcmp(name, "Alarm Clock") == 0)
//		{
//			NPCCOLLIDE message;
//			LOGIC->SendMessage(message);
//
//			if (scoreNum + 25 < 100)
//				scoreNum += 25;
//
//			std::cout << "NPC collide with alarm clock" << std::endl;
//			//npc fiddle with clock
//		}
//
//	}
//}
