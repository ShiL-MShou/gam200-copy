#pragma once
#include "Script.h"

class DoorScript : public Script
{

public:
	DoorScript();
	~DoorScript();

	void Start() override;

	void Init() override;

	void Update(float _dt) override;

	void Exit() override;

};