/*****************************************************************************/
/*
File Name:		BoxColider.cpp
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [70%]
				Tay Qin Wen Alvin	 [30%]
Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#include "stdafx.h"
#include "BoxCollider.h"
#include "SpriteComponent.h"
#include "Physics.h"
#include "imgui.h"

#include "DebugDraw.h"


BoxCollider::BoxCollider()
	: m_centerCoords{ },
	m_halfLengths{ VEC2{} },
	m_velocity{  },
	m_canCollide{ false },
	m_offSet{},
	isCollided{ false }
{
}

/*
	Constructors: -All float parameters
				  -All VEC2 parameters
				  -BoxCollider copy constructor
*/
BoxCollider::BoxCollider(float _x, float _y, float _halfWidth, float _halfheight,
	float _vecX, float _vecY, bool _canCollide) :
	m_centerCoords{ _x, _y },
	m_halfLengths{ _halfWidth, _halfheight },
	m_velocity{ _vecX, _vecY },
	m_canCollide{ _canCollide }
{}

BoxCollider::BoxCollider(VEC2 _centerCoords, VEC2 _halflengths, VEC2 _velocity, bool _canCollide) :
	m_centerCoords{ _centerCoords },
	m_halfLengths{ _halflengths },
	m_velocity{ _velocity },
	m_canCollide{ _canCollide }
{}

BoxCollider::BoxCollider(BoxCollider& _bc) :
	m_centerCoords{ _bc.m_centerCoords },
	m_halfLengths{ _bc.m_halfLengths },
	m_velocity{ _bc.m_velocity },
	m_canCollide{ _bc.m_canCollide }
{}

void BoxCollider::Initialize()
{
	//if (myType == CollideTypes::INTERACTABLE)
	//	mID = PHYSICS->AddInteractableCollider(GetBase()->has(BoxCollider));
	//else
	//	mID = PHYSICS->AddBoxCollider(GetBase()->has(BoxCollider));

	PHYSICS->AddBoxColliderToArray(GetID(), myType);

	std::shared_ptr<Transform> m_transform2 = GetBase()->has(Transform);

	if (m_halfLengths.x == 0 && m_halfLengths.y == 0)
	{
		m_halfLengths.x = m_transform2->GetScale().x;
		m_halfLengths.y = m_transform2->GetScale().y;
	}
	//m_halfLengths.x = trf->GetScale().x ;
	//m_halfLengths.y = trf->GetScale().y ;

	//m_halfLengths = m_transform2->GetScale();

	//m_centerCoords.x = trf->GetPosition().x;
	//m_centerCoords.y = trf->GetPosition().y;

	//m_centerCoords = m_transform2->GetPosition();// +m_offSet;

	m_canCollide = true;

	m_transform = m_transform2;
}

void BoxCollider::Serialize(JsonReader& _jr)
{
	std::string name = "BoxCollider";

	//VEC2 offset;
	//_jr.stream.clear();

	std::string typeStr;

	_jr.ReadStringMember(name, "Type", typeStr);

	if (typeStr == "PLAYER")
		myType = CollideTypes::PLAYER;
	else if (typeStr == "COLLECTABLE")
		myType = CollideTypes::COLLECTABLE;
	else if (typeStr == "INTERACTABLE")
		myType = CollideTypes::INTERACTABLE;
	else if (typeStr == "NPC")
		myType = CollideTypes::NPC;
	else if (typeStr == "ENVIRONMENT")
		myType = CollideTypes::ENVIRONMENT;
	else if (typeStr == "DOOR")
		myType = CollideTypes::DOOR;
	else if (typeStr == "WINCOLLIDER")
		myType = CollideTypes::WINCOLLIDER;
	else if (typeStr == "BUTTON")
		myType = CollideTypes::BUTTON;
	else
		myType = CollideTypes::NONE;

	_jr.ReadFloatMember(name, "OffsetX", m_offSet.x);
	_jr.ReadFloatMember(name, "OffsetY", m_offSet.y);

	_jr.ReadFloatMember(name, "HalfLengthX", m_halfLengths.x);
	_jr.ReadFloatMember(name, "HalfLengthY", m_halfLengths.y);


	//_jr.ReadBoolMember(name, "isInteractable", mInteractable);
}

void BoxCollider::DeSerialize(JsonWriter& _jw)
{
	_jw.CreateKey(GetName());
	_jw.prettyWriterBuffer.StartObject();

	std::string typeStr;

	if (myType == CollideTypes::PLAYER)
		typeStr = "PLAYER";
	else if (myType == CollideTypes::COLLECTABLE)
		typeStr = "COLLECTABLE";
	else if (myType == CollideTypes::INTERACTABLE)
		typeStr = "INTERACTABLE";
	else if (myType == CollideTypes::NPC)
		typeStr = "NPC";
	else if (myType == CollideTypes::ENVIRONMENT)
		typeStr = "ENVIRONMENT";
	else if (myType == CollideTypes::DOOR)
		typeStr = "DOOR";
	else if (myType == CollideTypes::BUTTON)
		typeStr = "BUTTON";
	else// if myType == CollideTypes::NONE;
		typeStr = "NONE";

	_jw.CreateStringMember("Type", typeStr);
	_jw.CreateFloatMember("OffsetX", m_offSet.x);
	_jw.CreateFloatMember("OffsetY", m_offSet.y);
	  
	_jw.CreateFloatMember("HalfLengthX", m_halfLengths.x);
	_jw.CreateFloatMember("HalfLengthY", m_halfLengths.y);

	_jw.prettyWriterBuffer.EndObject();
}

void BoxCollider::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	//std::shared_ptr<Transform> m_transform2 = GetBase()->has(Transform);

	//m_centerCoords = m_transform2->GetPosition() + m_offSet;

	//DebugDraw::Instance.DrawRect(m_centerCoords, m_halfLengths);
}

std::shared_ptr<Component> BoxCollider::Clone()
{
	std::shared_ptr<BoxCollider> newComponent = std::make_shared<BoxCollider>();

	newComponent->SetName(GetName());

	newComponent->SetCenterCoords(m_centerCoords);
	newComponent->SetHalfLengths(m_halfLengths);

	newComponent->myType = myType;

	newComponent->m_offSet = m_offSet;

	//newComponent->mInteractable = mInteractable;


	return newComponent;
}

void BoxCollider::Inspect()
{
	int it = static_cast<std::underlying_type_t<CollideTypes>>(myType);

	ImGui::Text(std::to_string(it).c_str());

	//ImGui::SliderFloat2("Position", m_centerCoords.vec, 0.0f, static_cast<float>(WinWidth));
	/*ImGui::SliderFloat2("Scale", m_halfLengths.vec, 0.0f, 500.0f);
	ImGui::SliderFloat2("Velocity", m_velocity.vec, 0.0f, 300.0f);*/
	ImGui::Checkbox("Collidable", &m_canCollide);
	//ImGui::Checkbox("Interactable", &mInteractable);

	ImGui::SliderFloat2("HalfLength", m_halfLengths.vec, 0.0f, static_cast<float>(WinWidth) / 2);
	ImGui::SliderFloat2("Offset", m_offSet.vec, -100.0f, static_cast<float>(WinWidth) / 2);

	int test = static_cast<int>(myType);

	ImGui::SliderInt("Offset", &test, 0, 6);
}



void BoxCollider::Serialize(ISerialization& _stream)
{
	UNREFERENCED_PARAMETER(_stream);
}

void BoxCollider::Destroy()
{
	PHYSICS->RemoveBoxCollider(GetID(), myType);
	//if (myType == CollideTypes::INTERACTABLE)
	//	PHYSICS->RemoveInteractableCollider(mID);
	//else
	//	PHYSICS->RemoveBoxCollider(mID);
}

/*
	Getters and Setters for the private members found below
	-
*/
void BoxCollider::SetCenterCoords(VEC2 _v)
{
	m_centerCoords = _v;
	//UNREFERENCED_PARAMETER(_v);
}

VEC2 BoxCollider::GetCenterCoords() const
{
	//return m_transform->GetPosition();

	return m_centerCoords;
}

void BoxCollider::SetHalfLengths(VEC2 _v)
{
	m_halfLengths = _v;
}

VEC2 BoxCollider::GetHalfLengths() const
{
	return m_halfLengths / 2.0f; //m_transform->GetScale() / 2.0f;	
}

void BoxCollider::SetVelocity(VEC2 _v)
{
	m_velocity = _v;
}

VEC2 BoxCollider::GetVelocity()
{
	std::shared_ptr<Body> body = GetBase()->has(Body);
	//std::cout << GetBase()->GetName() << std::endl;
	if (body)
		return body->GetVelocity();
	else
		return VEC2{ 0.0f, 0.0f };
}

void BoxCollider::SetOffSets(VEC2 _v)
{
	m_offSet = _v;
}

VEC2 BoxCollider::GetOffSets()
{
	return m_offSet;
}

void BoxCollider::SetCanCollide(bool _b)
{
	m_canCollide = _b;
}

bool BoxCollider::GetCanCollide() const
{
	return m_canCollide;
}

bool BoxCollider::IsCollided()
{
	return isCollided;
}

void BoxCollider::ResetIsCollide()
{
	isCollided = false;
	collidedlist.clear();
}

AABB BoxCollider::GetCollidedAABB()
{
	if (!collidedlist.empty())
		return std::get<1>(collidedlist[0]);
	else
		return AABB{};
}

CollideTypes BoxCollider::GetCollidedType()
{
	if (!collidedlist.empty())
		return MyResource.GetObjectWithID(std::get<0>(collidedlist[0]))->has(BoxCollider)->GetMyType();
	else
		return CollideTypes::NONE;
	//if (std::shared_ptr<GOC> obj = collidedWith.lock())
	//	return obj->has(BoxCollider)->GetMyType();
	//else
	//	return CollideTypes::NONE;
}

void BoxCollider::ClearCollideList()
{
	collidedlist.clear();
}

std::shared_ptr<GOC> BoxCollider::GetCollidedObj()
{
	if (!collidedlist.empty())
		return MyResource.GetObjectWithID(std::get<0>(collidedlist[0]));
	else
		return nullptr;
}

std::vector<std::tuple<GOCId, AABB>> BoxCollider::GetCollidedList()
{
	return collidedlist;
}

void BoxCollider::SetCollided(GOCId _id, AABB _aabb)
{
	//collidedWith = _obj;
	//collidedAABB = _aabb;
	collidedlist.push_back({ _id, _aabb });
	isCollided = true;
}

CollideTypes BoxCollider::GetMyType()
{
	return myType;
}

BoxCollider::~BoxCollider()
{
}

bool AABB::CheckCollided(AABB const* _rhs)
{
	//std::cout	<< (minPt.x > _rhs.maxPt.x) << "|" << (maxPt.x < _rhs.minPt.x) << "|"
	//			<< (minPt.y > _rhs.maxPt.y) << "|" << (maxPt.y < _rhs.minPt.y) << std::endl;

	// not collided
	if (minPt.x > _rhs->maxPt.x || maxPt.x < _rhs->minPt.x ||
		minPt.y > _rhs->maxPt.y || maxPt.y < _rhs->minPt.y)
	{
		return false;
	}

	return true;
}
