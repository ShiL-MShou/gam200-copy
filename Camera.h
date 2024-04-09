/************************************************************************************ /
/*
File Name:		Camera.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#pragma once


// to use glm or own vec/ matrix class?

#include "glm.hpp"
#include "Component.h"

// to use glm or own vec/ matrix class?

// --- glm ---
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

// --- transform ---
#include "Transform.h"

// --- vectors ---
#include "Vector2.h"

#include <queue>
#include <tuple>

//spritecomponent
#include "SpriteComponent.h"

#define CAMERA_VERTICAL_TOLORANCE 50
#define CAMERA_HORIZONTAL_TOLORANCE 100
#define MAX_SHAKE_COUNTER 6

#define SHAKE_OFFSET_X 10
#define SHAKE_OFFSET_Y 50

#define DEFAULT_MOVE_SPEED 1.0f
#define MAX_MOVE_SPEED 2.0f


enum class CAMERA_DIRECTION
{
	DEFAULT = -1,
	NEUTRAL,
	LEFT,
	RIGHT,
	NEUTRAL_X,
	UP,
	DOWN,
	NEUTRAL_Y
};

class Camera : public Component
{
	std::shared_ptr<Transform> m_transformPtr;

	std::shared_ptr<Transform> m_followPtr;

	float screenWidth, screenHeight;

	VEC2 followTolorance;
	VEC2 followSize;

	VEC2 limit;

	bool isMoving;
	VEC2 movingToPos;
	VEC2 movingDiff;

	CAMERA_DIRECTION	alignX;
	CAMERA_DIRECTION	alignY;


	VEC2 currAlignDiff;
	VEC2 alignDiff;

	// <position, timer, speed>
	std::queue<std::tuple<VEC2, float, float>> moveList;

	float timer;


	// shake
	VEC2 shakePos;
	int shakeCounter;
	bool isShaking;

	//black bars
	std::weak_ptr<SpriteComponent> barTopPtr;
	std::weak_ptr<SpriteComponent> barBotPtr;

public:
	Camera();
	~Camera();

	void Initialize() override;
	void Update();
	std::shared_ptr<Component> Clone() override;
	void Inspect() override;
	void Destroy() override;

	void ResetPosition();

	void MoveX(int _x);
	void MoveY(int _y);
	void Move(VEC2 _vec);

	void AdjustZoom(float _zoom);

	VEC2 GetPosition();
	VEC2 GetScale();

	float GetScreenWidth();
	float GetScreenHeight();

	// setter and getters
	glm::mat4 GetMatrix();

	void SetFollowPtr(std::shared_ptr<Transform> _followptr);
	void SetFollowSize(VEC2 _size);
	void SetLimit(VEC2 _limit);

	float MyRound(float _val);

	void MoveTo(std::tuple<VEC2, float, float> _pair);

	void SetAlign(CAMERA_DIRECTION _align);

	void AddToMoveList(VEC2 _pos, float _timer = 0, float _speed = DEFAULT_MOVE_SPEED);

	void AddFollowPos(float speed = DEFAULT_MOVE_SPEED);

	void UpdateAlignment();

	void Shake();
	// to call loop, it will be activated if the bool is true
	void ShakeHelper();

	size_t GetMoveListSize() { return moveList.size();  };

	void BarUpdate();

	void MoveBarDown(std::weak_ptr<SpriteComponent> barPtr, float speed);

	void MoveBarUp(std::weak_ptr<SpriteComponent> barPtr, float speed);
};

