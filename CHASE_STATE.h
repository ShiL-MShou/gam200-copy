/******************************************************************************/
/*
	File Name: CHASE_STATE.h
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief: 

	Copyright Information:
	All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "State.h"

class CHASE_STATE : public iState
{
	std::vector<VEC2> path_to_player;
	VEC2 _playerPos;
	VEC2 _unitPos;
	VEC2 unitGridIndex;
	VEC2 playerGridIndex;
	int _currPt;// = 0;
	int _nextPt;
	float speed;// = 100.0f;

public:
	CHASE_STATE();
	~CHASE_STATE();


	void Update(float _dt) override;

	void OnEnter() override;

	void OnExit() override;

	void ResetState() override;

	// Update waypoints to player position
	void FindTarget();

	// Move to player position
	void ChaseAfter(float step);
	
	// Set up currPt = 0, and nextPt
	void ResetCurrPoint();

	// Iterate through points
	void SetNextPoint();

	void SetPathtoPlayer(std::vector<VEC2> list);

	void SetSpeed(float s);

	void GetPlayerPos();
	void GetUnitPos();
};