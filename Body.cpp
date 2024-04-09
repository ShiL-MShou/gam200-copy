/*****************************************************************************/
/*
File Name:		Body.cpp
Project Name:	WhereGotLate
Author(s):		Ong Cheng Kai Marcus [90%]
				Kok Zhe Yan			 [10%]

Copyright information:
All content copyright 2020 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*****************************************************************************/
#include "stdafx.h"
#include "Body.h"
#include "DebugDraw.h"
#include "imgui.h"
#include "physics.h"
#include "imgui.h"
#include "ForceManager.h"

float EPSILON = 300.0f; // if you increase the magnitude of forces, this also needs to increase.
						//at time of writing: mag = 20000, epsilon = 250

Body::Body() :
	m_position{},
	m_prevPosition{},
	m_velocity{ VEC2{0, 0} },
	m_acceleration{},
	m_mass{ 2.0f },
	m_inverseMass{ 1.0f / m_mass },
	transform{},
	m_sumOfForces{},
	m_maxSpeed{ 200.0f}
{
	//forcemap = MyForceManager.ForceMap;
}

Body::~Body()
{
	forcemap.clear();
	//BodyForceVector.clear();
}

void Body::DebugDraw()
{
	MyDebugDraw.DrawLine(m_position, m_position + m_velocity);
}

void Body::Initialize()
{
	transform = GetBase()->has(Transform);
	m_position = transform->GetPosition();

    PHYSICS->AddBody(GetID());
}

std::shared_ptr<Component> Body::Clone()
{
	std::shared_ptr<Body> newComponent = std::make_shared<Body>();

	newComponent->m_velocity = m_velocity;
	newComponent->SetName(GetName());

	return newComponent;

}

void Body::Inspect()
{
	//ImGui::SliderFloat2("Velocity", m_position.vec, 0.0f, 300.0f);
	float test = VEC2SquaredLength(m_velocity);

	ImGui::SliderFloat("VEC2SquaredLength", &test, -50.0f, 50.0f);
	ImGui::SliderFloat("x sum of forces", &m_sumOfForces.x, -50.0f, 50.0f);
	ImGui::SliderFloat("y sum of forces", &m_sumOfForces.y, -50.0f, 50.0f);
	ImGui::SliderFloat("x vel", &m_velocity.x, -50.0f, 50.0f);
	ImGui::SliderFloat("y vel", &m_velocity.y, -50.0f, 50.0f);
	ImGui::SliderFloat("Drag life", &MyForceManager.GetMap()["drag"]->m_lifetime, -1.0f, 1.0f);
	ImGui::SliderFloat("Drag life", &MyForceManager.GetMap()["drag"]->m_age, -1.0f, 1.0f);
	//ImGui::SliderFloat("Linear mag", &BodyForceVector.at(0)->m_magnitude, -50.0f, 50.0f);
	//ImGui::SliderFloat("Drag mag", &BodyForceVector.at(1)->m_magnitude, -50.0f, 50.0f);

	ImGui::SliderFloat2("veloc", m_velocity.vec, -1.0f, 1.0f);
}

void Body::Serialize(ISerialization& _stream)
{
	StreamRead(_stream, m_velocity.x);
	StreamRead(_stream, m_velocity.y);
}

void Body::Serialize(JsonReader& _jr)
{
	std::string compName = "Body";
	_jr.ReadFloatMember(compName, "VelocityX", m_velocity.x);
	_jr.ReadFloatMember(compName, "VelocityX", m_velocity.y);
	_jr.ReadFloatMember(compName, "MaxSpeed", m_maxSpeed);

}

void Body::DeSerialize(JsonWriter& _jw)
{
	_jw.CreateKey(GetName());
	_jw.prettyWriterBuffer.StartObject();
	_jw.CreateFloatMember("VelocityX", m_velocity.x);
	_jw.CreateFloatMember("VelocityY", m_velocity.y);
	_jw.CreateFloatMember("MaxSpeed", m_maxSpeed);
	_jw.prettyWriterBuffer.EndObject();
}

void Body::Destroy()
{
	PHYSICS->RemoveBody(GetID());
}

void Body::SetPosition(VEC2 _pos)
{
	m_position = _pos;
}

VEC2 Body::GetPosition() const
{
	return m_position;
}

VEC2 Body::GetVelocity()
{
	return m_velocity;
}

void Body::SetVelocity(const VEC2& _vec)
{
	m_velocity = _vec;
}

void Body::SetVelocityX(float _x)
{
	m_velocity.x = _x;
}

void Body::SetVelocityY(float _y)
{
	m_velocity.y = _y;
}

float Body::GetMaxSpeed()
{
	return m_maxSpeed;
}

void Body::SetMaxSpeed(float _speed)
{
	m_maxSpeed = _speed;
}

std::shared_ptr<Transform> Body::GetTransform() const
{
	return transform;
}

void Body::SetPosX(float _x)
{
	m_position.x = _x;
}

void Body::SetPosY(float _y)
{
	m_position.y = _y;
}

//void Body::AddForce(std::shared_ptr<Force> _force)
//{
//	BodyForceVector.push_back(_force);
//}


//if for specific force instead of all forces, takes bool and enum of the force(?)
////////////////////////////////////////////////////////////////////////////////////
//No need to comment/uncomment
void Body::ActivateForces(std::string _key)
{
	//auto itr = forcemap.find(_key);

	// does not exist in forcemap
	if (forcemap.find(_key) == std::end(forcemap))
	{
		forcemap[_key] = std::make_shared<Force>(*MyForceManager.GetForceSPtr(_key));	
	}

	forcemap[_key]->m_isActive = true;
	forcemap[_key]->m_age = 0.0f;

	//MyForceManager.ForceMap[_key]->m_isActive = true;
	//MyForceManager.ForceMap[_key]->m_age = 0.0f;
}
//void Body::ActivateForces(ForceVecMember _key)
//{
//	BodyForceVector[_key]->m_isActive = true;
//	BodyForceVector[_key]->m_age = 0.0f;
//}
///////////////////////////////////////////////////////////////////////////////////

void Body::DeactivateForces(std::string _key)
{
	auto itr = forcemap.find(_key);

	// does not exist in forcemap
	if (itr != std::end(forcemap))
		forcemap[_key]->m_isActive = false;
	//forcemap[_key]->m_age = 0.0f;
}

//Using Map
////////////////////////////////////////////////////////////////////////////////////////////
void Body::ForceLifetimeUpdate(float _dt)
{
	for (auto& [key, value] : forcemap)
	{
		if (value->m_isActive)
		{
			if (value->m_forceType == FORCE_TYPE::LINEAR)
				EPSILON = value->m_epsilon;

			value->m_age += _dt;
						
			//std::cout << "Velocity : " << VEC2SquaredLength(m_velocity) << std::endl;

			if ((value->m_forceType == FORCE_TYPE::LINEAR) && (value->m_age >= value->m_lifetime))
			{
				value->m_isActive = false;
				value->m_age = 0.0f;
			}
			if (value->m_forceType == FORCE_TYPE::DRAG)
			{
				//std::cout << "drag age: " << value->m_age << std::endl;
				if (VEC2SquaredLength(m_velocity) < EPSILON)
				{
					value->m_isActive = false;
					value->m_age = 0.0f;
					//m_velocity = VEC2(0.0f, 0.0f);
				}
			}
		}

	}
}

////instead of always adding forces, add force in by default and change direction in logic
void Body::ApplyActiveForces()
{
	m_sumOfForces = { 0.0f, 0.0f };

	if (VEC2SquaredLength(m_velocity) < EPSILON)
	{
		m_velocity = { 0.0f, 0.0f };
	}

	//float dragX = 0.0f;
	
	for (auto& [key, value] : forcemap)
	{
		if (value->m_isActive)//is Linear force (enum)
		{
			//std::cout << "active forces count " << std::endl;
			if (value->m_forceType == FORCE_TYPE::DRAG)
			{
				//normalise vector of the velocity (get the direction)
				VEC2 unitVec;
				VEC2Normalize(unitVec, m_velocity);

				//m_direction = -(normalized vector)

				value->m_direction = -unitVec;

				//dragX = value->m_direction.x;

				//if mag is bigger will go negative
				// if mag is smaller okay
				//m_velocity.printVEC2();
				//std::cout << VEC2Length(m_velocity) << std::endl;

				if (VEC2Length(m_velocity) > value->m_magnitude)
				{
					//std::cout << "velo >= mag" << std::endl;
					m_sumOfForces += value->m_direction * value->m_magnitude;
				}

				else
				{
					//std::cout << "velo < mag" << std::endl;
					//VEC2 tmpVelo = value->m_direction * VEC2Length(m_velocity);
					//
					//std::cout << m_velocity.x << m_velocity.y << std::endl;
					//std::cout << tmpVelo.x << tmpVelo.y << std::endl;

					//m_sumOfForces += tmpVelo;
					// reset velocity
					m_velocity = { 0,0 };// -value->m_direction * 0.1f;
					//forcemap[key]->m_isActive = false;
				}

				//now we have the m_direction for the drag, so the below statement will calculate that
			}
			else
			{
				m_sumOfForces += value->m_direction * value->m_magnitude;
			}
		}
	}

	//if (m_sumOfForces.x < 0 && m_direction.x < 0)
	//	m_sumOfForces.x = 0;
	//else

}
//////////////////////////////////////////////////////////////////////////////////////////////////


//Using BodyForceVector
//////////////////////////////////////////////////////////////////////////////////////////////////
//void Body::ForceLifetimeUpdate(float _dt)
//{
//
//	for (auto& it : BodyForceVector)
//	{
//		if (it->m_isActive)
//		{
//			it->m_age += _dt;
//
//			std::cout << "Velocity : " << VEC2SquaredLength(m_velocity) << std::endl;
//
//			if ((it->m_forceType == LINEAR) && (it->m_age >= it->m_lifetime))
//			{
//				it->m_isActive = false;
//				it->m_age = 0.0f;
//
//			}
//			if (it->m_forceType == DRAG)
//			{
//				if (VEC2SquaredLength(m_velocity) < 250.0f)
//				{
//					it->m_isActive = false;
//					it->m_age = 0.0f;
//				}
//			}
//		}
//
//	}
//}

//Using BodyForceVector
//void Body::ApplyActiveForces()
//{
//	m_sumOfForces = { 0.0f, 0.0f };
//
//	if (VEC2SquaredLength(m_velocity) < EPSILON)
//	{
//		m_velocity = { 0.0f, 0.0f };
//	}
//
//	for (auto& it : BodyForceVector)
//	{
//		if (it->m_isActive)//is Linear force (enum)
//		{
//			if(it->m_forceType == DRAG)
//			{
//				//normalise vector of the velocity (get the direction)
//				VEC2 unitVec;
//				VEC2Normalize(unitVec, m_velocity);
//
//				//m_direction = -(normalized vector)
//				it->m_direction = -unitVec;
//
//				//now we have the m_direction for the drag, so the below statement will calculate that
//			}
//
//			m_sumOfForces += it->m_direction * it->m_magnitude;
//		}
//	}
//}
///////////////////////////////////////////////////////////////////////////////////////////////

void Body::Integrate(float _dt)
{
	//goes through the vector of forces and sums forces. removes force from vector if age > lifetime
	m_acceleration = m_sumOfForces * m_inverseMass;

	//std::cout << "Sum of Force" << m_sumOfForces.x << " |  " << m_sumOfForces.y << std::endl;
	//std::cout << m_sumOfForces.x << " |  " << m_sumOfForces.y << " : " << m_inverseMass << std::endl;


	//std::cout << "acel " << m_acceleration.x << " |  " << m_acceleration.y << std::endl;

	m_velocity = m_velocity + (m_acceleration * _dt);

	//std::cout << "velo  aft acel " << m_velocity.x << " |  " << m_velocity.y << std::endl;


	if (m_velocity.x > m_maxSpeed)
	{
		m_velocity.x = m_maxSpeed;
	}
	else if (m_velocity.x < -m_maxSpeed)
	{
		m_velocity.x = -m_maxSpeed;
	}

	if (m_velocity.y > m_maxSpeed)
	{
		m_velocity.y = m_maxSpeed;
	}
	else if (m_velocity.y < -m_maxSpeed)
	{
		m_velocity.y = -m_maxSpeed;
	}

	//if (m_velocity.x <= 1 && m_velocity.x >= -1)
	//{
	//	m_velocity.x = 0;
	//}
	//
	//if (m_velocity.y <= 1 && m_velocity.y >= -1)
	//{
	//	m_velocity.y = 0;
	//}

	//std::cout << m_velocity.x << " |  " << m_velocity.y << std::endl;
	//m_prevPosition = m_position;

	m_position = transform->GetPosition();

	m_position = m_position + (m_velocity * _dt);

	transform->SetPosition(m_position);
}