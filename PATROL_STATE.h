/******************************************************************************/
/*
	File Name: PATROL_STATE.h
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: Derived state for Teacher NPC

	Copyright Information:
	All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "State.h"
#include "Transition.h"
#include "Vector2.h"
#include "SpriteComponent.h"

class PATROL_STATE : public iState
{
	std::vector<VEC2> patrol_pts;
	std::vector<VEC2> path_to_pt;
	int _startPt;
	
	int _nextPt;
	int _currspt;
	int _nextspt;
	bool isMovingRight;
	VEC2 _UnitPos;
	VEC2 _distract_des;
	float speed;

	bool isFacingRight;
	//bool isFalling = false;


public:
	int _currPt = 0;

	PATROL_STATE();
	~PATROL_STATE();


	void Update(float _dt) override;

	void OnEnter() override;

	void OnExit() override;

	void ResetState() override;

	void UpdateAnimation() override;

	void AddPatrolPoint(VEC2 pt);

	void SetStartPoint(int index = 0);

	/* Update current and next point of patrol point*/
	void SetNextPoint();

	/* Getters */
	VEC2 GetCurrPt();
	VEC2 GetUnitPos();

	/* Setter and Getter for distraction point */
	void SetDistractDes(VEC2 t);
	VEC2 GetDistractDes();

	void MoveTo(float step);

	void SetPathtoPt(std::vector<VEC2> list);

	void SetCurrsmallPoint();
	/* Update current and next point of path_to_pt*/
	void GetNextsmallPoint();

	/* Detect change in direction */ // not used
	void ChangeDirection(const std::shared_ptr<SpriteComponent>& sprPtr);
};