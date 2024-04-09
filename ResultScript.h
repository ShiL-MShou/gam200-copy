#pragma once
#include "Script.h"
#include "Score.h"
#include "SpriteComponent.h"

class ResultScript : public Script
{
	std::weak_ptr<SpriteComponent>	gradeAPtr;
	std::weak_ptr<SpriteComponent>	gradeCPtr;
	std::weak_ptr<SpriteComponent>	gradeFPtr;
	std::weak_ptr<SpriteComponent>	backgroundAPtr;
	std::weak_ptr<SpriteComponent>	backgroundCPtr;
	std::weak_ptr<SpriteComponent>	backgroundFPtr;

	int score;
	bool picLoaded;
	bool playSound;
	bool bgFLoaded;

public:
	ResultScript();
	~ResultScript();

	virtual void Start() override;
	virtual void Init() override;
	virtual void Update(float _dt) override;
	virtual void Exit() override;
	void LoadResource() override;
};