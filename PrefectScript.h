#pragma once
#include "NPCScript.h"

#include <memory>
#include "StateMachine.h"
#include "Body.h"
#include "SpriteComponent.h"

#include "List_StateLinker.h"
#include "PATROL_STATE.h"
#include "IDLE_STATE.h"
#include "DISTRACTED_STATE.h"
#include "CHASE_STATE.h"
#include "StateEnum.h"


class PrefectScript : public NPCScript
{
	bool isRight;
	bool isUp;

	int dir = RIGHT;

	std::shared_ptr<Body>	bodyPtr;
	std::shared_ptr<SpriteComponent> spritePtr;



	// Create States
	std::shared_ptr<IDLE_STATE> idle = std::make_shared<IDLE_STATE>();
	std::shared_ptr<PATROL_STATE> patrol = std::make_shared<PATROL_STATE>();
	std::shared_ptr<DISTRACTED_STATE> distract = std::make_shared<DISTRACTED_STATE>();
	std::shared_ptr<CHASE_STATE> chase		    = std::make_shared<CHASE_STATE>();
	
	// create condition
	std::shared_ptr<TimerTransition> clock = std::make_shared<TimerTransition>(Patrol);
	std::shared_ptr<Rest> toRest = std::make_shared<Rest>(Idle);
	std::shared_ptr<ReachItem> RI = std::make_shared<ReachItem>(Patrol);
	std::shared_ptr<Distracted> Found1 = std::make_shared<Distracted>(Distract);
	std::shared_ptr<Distracted> Found2 = std::make_shared<Distracted>(Distract);
	std::shared_ptr<GiveUpChasing> stop = std::make_shared<GiveUpChasing>(Patrol);
	std::shared_ptr<DetectPlayer> seePlayI = std::make_shared<DetectPlayer>(Chase);
	std::shared_ptr<DetectPlayer> seePlayP = std::make_shared<DetectPlayer>(Chase);
	std::shared_ptr<DetectPlayer> seePlayD = std::make_shared<DetectPlayer>(Chase);

	//// idle:0, patrol:1, distract:2, chase:3
	std::shared_ptr<StateMachine> sm;// = std::make_shared<StateMachine>();

public:
	PrefectScript();
	~PrefectScript();

	// delete
	PrefectScript(const PrefectScript&) = delete;
	PrefectScript& operator=(const PrefectScript&) = delete;

	//int dir = RIGHT;
	
	void Start() override;
	//
	//void Update(float _dt) override;
	//
	//virtual void Exit() override;
	//
	//void SetOwner(std::shared_ptr<GOC> _gameObjPtr);
	//
	//void CollisionResponse(std::weak_ptr<GOC> _obj, CollideTypes _type, AABB _aabb, float _dt);
};

