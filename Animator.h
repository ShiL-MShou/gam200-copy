#pragma once
//
//#include <memory>				// smart pointers
//
//#include "Composition.h"
//#include "Component.h"
//
//#include "Sprite.h"
//#include "Vector2.h"
//#include "Texture.h"
//#include "TextSerialization.h"	// ISerialization?
//
//
//
//class Animator : public Component
//{
//	//unsigned int	currFrame;
//	//unsigned int	maxFrame;
//	//unsigned int	numCol;
//	//unsigned int	numRow; // not used yet
//	//VEC2			textureCoordsDiff;
//
//
//	unsigned int	currFrame;
//	unsigned int	maxFrame;
//	float			updateInterval;
//	float			timeTillNextFrame;
//
//	std::shared_ptr<Sprite> spritePtr;
//	
//	//float			data[20];	// (x, y, z, u, v) * 4
//
//	//std::shared_ptr<Texture>	texturePtr;
//
//public:
//	Animator();
//	~Animator();
//
//	void Initialize() override;
//
//	void Inspect() override;
//
//	std::shared_ptr<Component> Clone() override;
//
//	void Update(float _dt);
//
//	void Serialize(ISerialization& stream) override;
//	void Serialize(JsonReader& _jr) override;
//	void DeSerialize(JsonWriter& _jw) override;
//	void Destroy() override;
//
//	// does the same as SetMaxFrame
//	void ChangeAnimation(unsigned int _maxFrame);
//
//	void SetMaxFrame(unsigned int _maxFrame);
//
//	bool IsAnimationEnd();
//
//};
//
