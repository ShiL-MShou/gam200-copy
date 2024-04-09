/************************************************************************************ /
/*
File Name:		Transform.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Transform component.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "Transform.h"
#include "imgui.h"
#include "SpriteComponent.h"


Transform::Transform()
	: m_position{ 0, 0 }, m_scale{ 1, 1 }, m_rotation{ 0 }, m_startpos{ 0, 0 }
{
}

Transform::~Transform()
{
}

Transform::Transform(const Transform& _rhs)
	: m_position{ _rhs.m_position }, m_scale{ _rhs.m_scale }, m_rotation{_rhs.m_rotation}, m_startpos{_rhs.m_startpos}
{
}

Transform& Transform::operator=(const Transform& _rhs)
{
	m_position = _rhs.m_position;
	m_scale = _rhs.m_scale;
	m_rotation = _rhs.m_rotation;
	m_startpos = _rhs.m_startpos;

	return *this;
}

void Transform::Initialize()
{
}

std::shared_ptr<Component> Transform::Clone()
{
	std::shared_ptr<Transform> newComponent = std::make_shared<Transform>();

	newComponent->m_position = m_position;
	newComponent->m_rotation = m_rotation;
	newComponent->m_scale = m_scale;
	newComponent->m_startpos = m_startpos;
	newComponent->SetName(GetName());

	return newComponent;
}

void Transform::Inspect()
{
	ImGui::SliderFloat2("Position", m_position.vec, -1000.0f, static_cast<float>(WinWidth));
	ImGui::SliderFloat2("Scale", m_scale.vec, 0.0f, 500.0f);
	ImGui::SliderFloat("Rotation", &m_rotation, 0.0f, 360.0f);
	ImGui::SliderFloat2("Start Pos", m_startpos.vec, -1000.0f, static_cast<float>(WinWidth));
}

void Transform::Serialize(ISerialization& _stream)
{
	StreamRead(_stream, m_position.x);
	StreamRead(_stream, m_position.y);
	StreamRead(_stream, m_rotation);
}

void Transform::Serialize(JsonReader& _jr)
{
	UNREFERENCED_PARAMETER(_jr);
}

void Transform::Destroy()
{
}

void Transform::SetPosition(VEC2 _pos)
{
	m_position = _pos;
}

void Transform::SetPosition(float _x, float _y)
{
	m_position.x = _x;
	m_position.y = _y;
}

void Transform::SetPositionX(float _x)
{
	m_position.x = _x;
}

void Transform::SetPositionY(float _y)
{
	m_position.y = _y;
}

void Transform::SetStartPosition(VEC2 _pos)
{
	m_startpos = _pos;
}

void Transform::SetScale(VEC2 _scale)
{
	m_scale = _scale;
}

void Transform::SetScale(float x, float y)
{
	m_scale.x = x;
	m_scale.y = y;
}

void Transform::SetRotation(float _rotate)
{
	m_rotation = _rotate;
}

VEC2 Transform::GetStartPosition() const
{
	if (m_startpos.x == 0 && m_startpos.y == 0)
		return m_position;
	else
		return m_startpos;
}

VEC2 Transform::GetScale()
{
	return m_scale;
}

glm::mat4 Transform::ConvertIntoMat4()
{
	glm::mat4 myTranslate;

	//const auto* map = &GetBase()->GetComponentMap();

	if (GetBase() && GetBase()->GetComponent("SpriteComponent") != nullptr)
	{
		std::shared_ptr<SpriteComponent> sprPtr = GetBase()->has(SpriteComponent);
		myTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(m_position.x, m_position.y, 0));
	}
	else
		myTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(m_position.x, m_position.y, 0));

	glm::mat4 myRotate = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));
	glm::mat4 myScale = glm::scale(glm::mat4(1.0f), glm::vec3(m_scale.x, m_scale.y, 1));

	return myTranslate * myRotate * myScale;

	//return glm::mat4();
}

std::array<float, 16> Transform::ConvertTest() const
{
	glm::mat4 myTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(m_position.x, m_position.y, 0));
	glm::mat4 myRotate = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));
	glm::mat4 myScale = glm::scale(glm::mat4(1.0f), glm::vec3(m_scale.x, m_scale.y, 1));

	glm::mat4 mat = myTranslate * myRotate * myScale;

	std::array<float, 16> tmpArr;


	const float* pSource = (const float*)glm::value_ptr(mat);
	for (int i = 0; i < 16; ++i)
		tmpArr[i] = pSource[i];


	return tmpArr;
}
