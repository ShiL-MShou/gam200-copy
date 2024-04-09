/******************************************************************************/
/*
	File Name: CHASE_STATE.cpp
	Project Name: WhereGotLate
	Author(s): Lee Shi Lin [100%]

	Brief:

	Copyright Information:
	All content © 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "stdafx.h"
#include "CHASE_STATE.h"
#include "PathFind.h"

#include "DebugDraw.h"


CHASE_STATE::CHASE_STATE()
	: path_to_player{}, _playerPos{}, _unitPos{}, unitGridIndex{}, playerGridIndex{}, _currPt{ 0 }, _nextPt{ 0 }, speed{100.0f}
{
	iState::_name = "Chase";
}

CHASE_STATE::~CHASE_STATE()
{
}

void CHASE_STATE::Update(float _dt)
{
	//std::cout << "Chase State" << std::endl;


	UNREFERENCED_PARAMETER(_dt);
	// change animation to run once the alerted animation is done.

	// update animation
	//GetOwner()->has(SpriteComponent)->ChangeGenericAnimation("ALERTED");
	std::shared_ptr<AnimationComponent> animationCompPtr = GetOwner()->has(AnimationComponent);

	int animationID = animationCompPtr->animationList.at(animationCompPtr->currAnimation);
	std::shared_ptr<Animation> animationPtr = GRAPHICS->GetAnimation(animationID);

	std::string animationName = GRAPHICS->GetAnimationName(animationID);

	MyDebugDraw.DrawRect(_playerPos, VEC2(5.0f, 5.0f));

	//std::cout << animationName.c_str() << std::endl;
	if (animationName.find("ALERTED") != std::string::npos)
	{
		//std::cout << animationCompPtr->currFrame  << "| " << animationPtr->lastFrame << std::endl;
		if (animationCompPtr->currFrame == animationPtr->lastFrame && animationCompPtr->currFrameTime <= 0.1f)
		{
			GetOwner()->has(SpriteComponent)->ChangeGenericAnimation("RUN");
			_ownerSM->updatedSprite = true;

			// check if the animation is changed.
			animationID = animationCompPtr->animationList.at(animationCompPtr->currAnimation);
			animationPtr = GRAPHICS->GetAnimation(animationID);

			animationName = GRAPHICS->GetAnimationName(animationID);

			if (animationName.find("ALERTED") != std::string::npos)
			{
				GetOwner()->has(SpriteComponent)->ChangeGenericAnimation("WALKING");
				_ownerSM->updatedSprite = true;
			}
		}
		else
			return;
	}

	//std::cout << "Chase State" << std::endl;
	if (IsUnableToChase() == true)
	{
		DisableAllForces(GetOwner()->has(Body));
		return;
	}

	//float step = speed * _dt;
	// Update current position of Unit and Player
	//GetUnitPos();
	//GetPlayerPos();

	// Update waypoints

	//if (path_to_player.size() == 0)
	{
		FindTarget();

		for (auto& tmpVec : path_to_player)
		{
			MyDebugDraw.DrawDebug(tmpVec, tmpVec + VEC2(10, 10));
		}
	}



	/*std::shared_ptr<SpriteComponent> sprPtr = GetOwner()->has(SpriteComponent);

	if (sprPtr != nullptr)
	{
		sprPtr->ChangeGenericAnimation("RUN");
		_ownerSM->updatedSprite = true;
	}*/
	

	//====================
	// Move towards player
	//====================
	if (path_to_player.size() != 0)
	{
		// if not end of waypoints
		if (_unitPos != path_to_player[path_to_player.size() - 1])
		{
			GetUnitPos();
			//std::cout << "Chase : " << _unitPos.x << " | " << _unitPos.y << "    " << path_to_player[_currPt].x << " | " << path_to_player[_currPt].y
			//			<< std::endl;
			MoveToPos(GetOwner()->has(Body), _unitPos, path_to_player[_currPt]);

			//ChaseAfter(step);
			SetNextPoint();
		}
		// else -> do nothing
	}
}

void CHASE_STATE::OnEnter()
{
	//Get position
	GetUnitPos();
	//GetPlayerPos();

	GetOwner()->has(SpriteComponent)->ChangeGenericAnimation("ALERTED");
	//_ownerSM->updatedSprite = true;
}

void CHASE_STATE::OnExit()
{
	_currPt = 0;
	path_to_player.clear();
	GetOwner()->has(Body)->DeactivateForces("playerRight");
	GetOwner()->has(Body)->DeactivateForces("playerLeft");
	GetOwner()->has(Body)->DeactivateForces("playerUp");
	GetOwner()->has(Body)->DeactivateForces("playerDown");
	GetOwner()->has(Body)->DeactivateForces("drag");

	GetOwner()->has(Body)->SetVelocity(VEC2(0, 0));
}

void CHASE_STATE::ResetState()
{
	path_to_player.clear();
}

// Update waypoints to player position
void CHASE_STATE::FindTarget()
{
	int oldPX = static_cast<int>(playerGridIndex.getx()); 
	int oldPY = static_cast<int>(playerGridIndex.gety());
	//int oldUX = static_cast<int>(unitGridIndex.getx()); 
	//int oldUY = static_cast<int>(unitGridIndex.gety());

	GetUnitPos();
	GetPlayerPos();

	//if unit & player grid change -> update way point
	if (oldPX != static_cast<int>(playerGridIndex.getx()) || 
		oldPY != static_cast<int>(playerGridIndex.gety()) ||
		path_to_player.empty() ) //|| 
		//oldUX != static_cast<int>(unitGridIndex.getx())   || 
		//oldUY != static_cast<int>(unitGridIndex.gety()) )
	{
		// if player position not blocked
		if (TestGridStat(_playerPos))
		{
			// Get waypoints to player
			std::vector<VEC2> g = FindPath(LOGIC->GetAImap(), _unitPos, _playerPos);

			if (g.empty() == false)
			{
				path_to_player.clear();
				SetPathtoPlayer(g);
			}
			else
				return;
		}
		else // if player position blocked 
		{
			// Find nearest walkable grid to player
			VEC2 nearest;
			int currX = static_cast<int>(playerGridIndex.getx());
			int currY = static_cast<int>(playerGridIndex.gety());

			if ((currY + 1) < MapHeight && TestGridStat(currX, currY + 1)) // UP
			{
				nearest = GetPos_index(currX, currY + 1);
				//std::cout << "FindPath 1" << std::endl;
			}
			else if ((currX + 1) < MapWidth && TestGridStat(currX + 1, currY)) // RIGHT
			{
				nearest = GetPos_index(currX + 1, currY);
				//std::cout << "FindPath 2" << std::endl;
			}
			else if ((currY - 1) > -1 && TestGridStat(currX, currY - 1)) // DOWN
			{
				nearest = GetPos_index(currX, currY - 1);
				//std::cout << "FindPath 3" << std::endl;
			}
			else if ((currX - 1) > -1 && TestGridStat(currX - 1, currY)) // LEFT
			{
				nearest = GetPos_index(currX - 1, currY);
				//std::cout << "FindPath 4" << std::endl;
			}
			//else if ((currX - 1) > -1 && TestGridStat(currX - 1, currY)) // LEFT
			//{
			//}
			else
				return;

			// Get waypoints to grid nearest to player
			std::vector<VEC2> g = FindPath(LOGIC->GetAImap(), _unitPos, nearest);

			if (g.empty() == false)
			{
				path_to_player.clear();
				SetPathtoPlayer(g);
				//std::cout << "Path Found" << path_to_player.size() << " | " << g.size() << std::endl;
			}
			else
				return;

		}

		// reset currPt & nextPt
		ResetCurrPoint();
	}
	// else -> do nothing
}

void CHASE_STATE::ChaseAfter(float step)
{
	UNREFERENCED_PARAMETER(step);
	GetUnitPos();
	//VEC2 n = VEC2MoveTowards(_unitPos, path_to_player[_currPt], step);

	//std::cout	<< "Chase : " << _unitPos.x << " | " << _unitPos.y << "    " << path_to_player[_currPt].x << " | " << path_to_player[_currPt].y
	//			<< std::endl;

	if (VEC2Magnitude(_unitPos, path_to_player[_currPt]) <= 1)
	{
		//std::cout << "Snap" << std::endl;
		GetOwner()->has(Transform)->SetPosition(path_to_player[_currPt]);

		GetOwner()->has(Body)->DeactivateForces("playerRight");
		GetOwner()->has(Body)->DeactivateForces("playerLeft");
		GetOwner()->has(Body)->DeactivateForces("playerUp");
		GetOwner()->has(Body)->DeactivateForces("playerDown");
		GetOwner()->has(Body)->DeactivateForces("drag");

		GetOwner()->has(Body)->SetVelocity(VEC2(0, 0));
		SetNextPoint();
		return;
	}

	if (_unitPos.x < path_to_player[_currPt].x) // move right
	{
		//std::cout << "right" << _unitPos.y << std::endl;
		GetOwner()->has(Body)->ActivateForces("playerRight");
		GetOwner()->has(Body)->ActivateForces("drag");
	}
	else if (_unitPos.x > path_to_player[_currPt].x) // move left
	{
		//std::cout << "left " << _unitPos.y << std::endl;
		GetOwner()->has(Body)->ActivateForces("playerLeft");
		GetOwner()->has(Body)->ActivateForces("drag");
	}

	if (_unitPos.y < path_to_player[_currPt].y) // move up
	{
		//std::cout << "Up " << _unitPos.y << std::endl;
		GetOwner()->has(Body)->ActivateForces("playerUp");
		GetOwner()->has(Body)->ActivateForces("drag");
	}
	else if (_unitPos.y > path_to_player[_currPt].y) // move down
	{
		//std::cout << "Down" << _unitPos.y << std::endl;
		GetOwner()->has(Body)->ActivateForces("playerDown");
		GetOwner()->has(Body)->ActivateForces("drag");
	}

	//if (_unitPos.x < n.x && GetOwner()->has(Transform)->GetScale().x < 0)
	//{
	//	GetOwner()->has(SpriteComponent)->FlipSprite();
	//}
	//else if (_unitPos.x > n.x&& GetOwner()->has(Transform)->GetScale().x > 0)
	//{
	//	GetOwner()->has(SpriteComponent)->FlipSprite();
	//}

	SetNextPoint();
}

// Set currPt = 0, and nextPt
void CHASE_STATE::ResetCurrPoint()
{
	if (path_to_player.size() == 0)
	{
		//std::cout << "No Points of Patrol" << std::endl;
	}
	else
	{
		_currPt = 0;

		if (path_to_player.size() == 1)
			_nextPt = _currPt;
		else
		{
			if (_currPt + 1 == path_to_player.size())
				_nextPt = 0;
			else
				_nextPt = _currPt + 1;
		}
	}
}

void CHASE_STATE::SetNextPoint()
{
	GetUnitPos();
	if ((int)(_unitPos.x) == (int)path_to_player[_currPt].x && (int)(_unitPos.y) == (int)(path_to_player[_currPt].y) )
	{
		_currPt = _nextPt;

		if (path_to_player.size() == 1)
			_nextPt = _currPt;
		else
		{
			if (_currPt + 1 == path_to_player.size())
				_nextPt = 0;
			else
				_nextPt = _currPt + 1;
		}
	}
}

void CHASE_STATE::SetPathtoPlayer(std::vector<VEC2> list)
{
	for (size_t i = 0; i < list.size(); i++)
	{
		path_to_player.push_back(list[i]);
	}
}

void CHASE_STATE::SetSpeed(float s)
{
	speed = s;
}

void CHASE_STATE::GetPlayerPos()
{
	_playerPos = PHYSICS->GetPlayerGOC().lock()->has(BoxCollider)->GetCenterCoords();
	playerGridIndex = VEC2(static_cast<float>(GetGridX_index(_playerPos)), static_cast<float>(GetGridY_index(_playerPos)));
}

void CHASE_STATE::GetUnitPos()
{
	_unitPos = GetOwner()->has(Transform)->GetPosition();
	unitGridIndex = VEC2(static_cast<float>(GetGridX_index(_unitPos)), static_cast<float>(GetGridY_index(_unitPos)));
}

// check within grid range
// same y or x with AI and Player
// check for blockage