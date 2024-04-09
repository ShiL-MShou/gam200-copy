/*****************************************************************************/
/*
File Name:		PlayerScript.h
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [40%]
				Tay Qin Wen Alvin 	 [60%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#pragma once

#include "Script.h"

#include <memory>
#include "Input.h"
#include "Transform.h"
#include "SpriteComponent.h"
#include "Body.h"
#include "Composition.h"
#include "TextRenderer.h"
#include "Inventory.h"
#include "SmokeFeedbackScript.h"
#include "InteractablesFeedbackScript.h"
#include "HomeWorkScript.h" //extern bool HWcopied
#include "AnimationComponent.h"

extern bool IsBeingChased;// = true;


class PlayerScript : public Script
{
	std::shared_ptr<Input>			inputPtr;
	//std::weak_ptr<GOC>			gameObjPtr;
	std::shared_ptr<Transform>		transformPtr;
	std::shared_ptr<BoxCollider>	colliderPtr;
	std::shared_ptr<Body>			bodyPtr;

	//dialogs
	std::weak_ptr<GOC>				textObjPtr; // Obj pointer
	std::weak_ptr<GOC>				boySpritePtr;
	std::weak_ptr<GOC>				tintPtr;
	
	//Dialog Bools//////////////////////
	
	//bool IsBeingChased = true;


	bool firstBananaDrop;
	bool playBananaDropText;

	bool firstGotBanana;
	bool gotBanana;

	bool firstClockDrop;
	bool playClockDropText;

	bool firstGotClock;
	bool gotClock;
	////////////////////////////////////

	//float walkSoundCounter;

	//score stuff
	std::shared_ptr<TextRenderer> scoreRendererPtr;
	std::weak_ptr<GOC> scoreBoxPtr;

	std::weak_ptr<SpriteComponent> sweatPtr;
	std::shared_ptr<AnimationComponent> trashVFXPtr;

	VEC2 scoreStartPt;
	std::string scoreText;
	int maxScore;

	bool isFacingRight;
	bool isFacingUp;

	std::shared_ptr<SpriteComponent>		spritePtr;

	Inventory						inventory;

	SmokeFeedbackScript			smokeFeedback;

	InteractablesFeedbackScript interactableFeedback;

	bool isShadowCreated;

	int trashFrame;

	float smokeDelay;

	std::vector<GOCId> noteCheck;

public:
	PlayerScript();
	~PlayerScript();

	virtual void Start() override;
	virtual void Init() override;
	virtual void Update(float _dt) override;
	virtual void Exit() override;

	void SetOwner(std::shared_ptr<GOC> _gameObjPtr) override;
	//std::weak_ptr<GOC> GetGameObjPtr();

	virtual void CollisionResponse(std::shared_ptr<GOC> _obj, CollideTypes _type, AABB _aabb);

	void CollectItem();

	void LoadResource() override;
};

extern int scoreNum;

extern bool inDialog;

extern bool isInventoryFull;