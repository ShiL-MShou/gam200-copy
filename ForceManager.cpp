#include "stdafx.h"
#include "ForceManager.h"

ForceManager::ForceManager() : drag{ 18400.0f }, dragLifetime{ 5000.0f }
{
}

ForceManager::~ForceManager()
{
	ForceMap.clear();
}

void ForceManager::Initialize()
{
	DirectionMap["center"] = VEC2{ 0.0f, 0.0f };
	DirectionMap["left"] = VEC2{ -1.0f, 0.0f };
	DirectionMap["right"] = VEC2{ 1.0f, 0.0f };
	DirectionMap["up"] = VEC2{ 0.0f, 1.0f };
	DirectionMap["down"] = VEC2{ 0.0f, -1.0f };

	float playerMagnitude = 20000.0f;
	float lifetime = 0.018f;
	

	//do we want:
	//1) one movement force that we just change directions for?
	//2) diff forces for diff directions? 
	//if 2nd, need an enum for the IDs and a very specific loading of the forces. 

	CreateMovementForces(playerMagnitude, lifetime, "player");
	CreateMovementForces(30000.0f, lifetime, "player2");
	//std::shared_ptr<Force> dragForce = std::make_shared<Force>(DirectionMap["center"], drag, dragLifetime, DRAG);

	std::shared_ptr<Force> moveDIAGONALUPRIGHT = std::make_shared<Force>(VEC2{ 0.5f, 0.5f }, playerMagnitude, 5.0f, FORCE_TYPE::LINEAR);
	std::shared_ptr<Force> moveDIAGONALUPLEFT = std::make_shared<Force>(VEC2{ -0.5f, 0.5f }, playerMagnitude, 5.0f, FORCE_TYPE::LINEAR);
	std::shared_ptr<Force> moveGRAVITY = std::make_shared<Force>(VEC2{ 0.0f, -1.0f }, playerMagnitude, 5.0f, FORCE_TYPE::LINEAR);

	ForceMap["drag"] = std::make_shared<Force>(DirectionMap["center"], drag, dragLifetime, FORCE_TYPE::DRAG);

	ForceMap["diagLeft"] = moveDIAGONALUPLEFT;
	ForceMap["diagRight"] = moveDIAGONALUPRIGHT;
	ForceMap["Gravity"] = moveGRAVITY;
}

void ForceManager::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void ForceManager::Clear()
{
	
}

//use this with the key for the force you want to activate eg. MyForceManager.ActivateForce(teacherLeft);
void ForceManager::ActivateForce(std::string _key)
{
	ForceMap[_key]->m_isActive = true;
	ForceMap[_key]->m_age = 0.0f;
}

//enter the magnitude, lifetime and the base "key" you want to set.
//you will get 4 new forces in the format "keyLeft, keyRight" etc
// note: because it is using std::map insert(), you will not be able to overwrite an existing key with this.
void ForceManager::CreateMovementForces(float _magnitude, float _lifetime, std::string _key)
{
	ForceMap.insert( std::pair(_key + "Left", std::make_shared<Force>(DirectionMap["left"], _magnitude, _lifetime, FORCE_TYPE::LINEAR)) );
	ForceMap.insert( std::pair(_key + "Right", std::make_shared<Force>(DirectionMap["right"], _magnitude, _lifetime, FORCE_TYPE::LINEAR)) );
	ForceMap.insert( std::pair(_key + "Up", std::make_shared<Force>(DirectionMap["up"], _magnitude, _lifetime, FORCE_TYPE::LINEAR)) );
	ForceMap.insert( std::pair(_key + "Down", std::make_shared<Force>(DirectionMap["down"], _magnitude, _lifetime, FORCE_TYPE::LINEAR)));

	return;
}

std::map<std::string, std::shared_ptr<Force>>& ForceManager::GetMap()
{
	return ForceMap;
}

std::shared_ptr<Force>& ForceManager::GetForceSPtr(std::string _key)
{
	return ForceMap[_key];
}