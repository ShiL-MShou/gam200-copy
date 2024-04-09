#pragma once
#include "System.h"
#include "Singleton.h"
#include "Force.h"
#include "ForceTypes.h"

class ForceManager : public Singleton<ForceManager>
{
	ForceManager();
	virtual ~ForceManager();

	ForceManager(const ForceManager&) = delete;
	ForceManager& operator=(const ForceManager&) = delete;

	friend class Singleton<ForceManager>;
	friend class Force;

	std::map < std::string, VEC2 > DirectionMap;
	float drag;
	float dragLifetime;

public:
	std::map< std::string, std::shared_ptr<Force> > ForceMap;

	void Initialize();
	void Update(float _dt);
	void Clear();
	void ActivateForce(std::string _key);
	void CreateMovementForces(float _magnitude, float _lifetime, std::string _key);
	std::map<std::string, std::shared_ptr<Force>>& GetMap();
	std::shared_ptr<Force>& GetForceSPtr(std::string _key);
};

#define MyForceManager (ForceManager::GetInstance())