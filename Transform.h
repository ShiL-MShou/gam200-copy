/************************************************************************************ /
/*
File Name:		Transform.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Transform component.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

#include "Component.h"

// --- glm ---
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

// --- vector ---
#include "Vector2.h"

#include <array>


class Transform :public Component
{
	VEC2 m_position, m_scale, m_startpos;
	float m_rotation;

public:
	Transform();
	~Transform();

	Transform(const Transform& _rhs);
	Transform& operator=(const Transform& _rhs);

	void Initialize() override;

	std::shared_ptr<Component> Clone() override;

	void Inspect() override;

	void Serialize(ISerialization& _stream) override;
	void Serialize(JsonReader& _jr) override;

	void Destroy() override;

	// set and get
	void SetPosition(VEC2 _pos);
	void SetPosition(float _x, float _y);
	void SetPositionX(float _x);
	void SetPositionY(float _y);

	void SetStartPosition(VEC2 _pos);


	void SetScale(VEC2 _scale);
	void SetScale(float x, float y);

	void SetRotation(float _rotateRad);

	VEC2 GetPosition() const { return m_position; }
	VEC2 GetStartPosition() const;
	VEC2 GetScale();
	
	float GetRotation() const { return m_rotation; }

	glm::mat4 ConvertIntoMat4();


	std::array<float, 16> ConvertTest() const;
};

