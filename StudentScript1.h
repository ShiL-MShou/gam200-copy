#pragma once
#include "Script.h"


class StudentScript1 :	public Script
{ 
public:
	void Start() override;
	void Init() override;
	void Update(float _dt) override;
	void Exit() override;
	void LoadResource() override;
};

