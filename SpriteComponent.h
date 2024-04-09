/************************************************************************************ /
/*
File Name:		SpriteComponent.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Sprite component.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

#include "Composition.h"

#include <memory>

#include "Texture.h"
#include "Shader.h"

#include "Transform.h"

#include "GraphicsVerticesType.h"


enum struct SpriteType
{
	SINGLE = 0,
	ANIMATED
};

enum struct SpriteMovementType
{
	FIXED = 0,
	FREE
};

class SpriteComponent : public Component
{
	int									mSpriteID;
	SpriteType							m_type;
	SpriteMovementType					movementType;

	int									currFrame = 0;
	float								depth;
	VEC2								m_size;

	VEC2								offset;

	std::shared_ptr<Transform>			m_transform;

	bool								isActive;
	
	float								alpha;

	// --- Single Sprite ---
	std::string							currSpriteListName;

	// ---------------------

	// --- optional? ---
	std::string							currTextureName;
	// -----------------


	// --- Fixed Movement ---
	VEC2								fixedPos;
	// ----------------------

	// --- utility? ---
	bool								isFliped;
	// ----------------


	// ------------------------------------------------------------------------------------

	//int									m_layerNum;

	std::vector<std::string>			m_subTextureList;

	// ----------------------------------------------
	int									startFrame;
	int									maxFrame;

	std::string							currSubTextureName;
	// ----------------------------------------------

	

public:
	SpriteComponent();
	~SpriteComponent();

	SpriteComponent(const SpriteComponent& _rhs) = delete;
	SpriteComponent& operator=(const SpriteComponent& _rhs) = delete;

	void Initialize() override;

	void Inspect() override;

	std::shared_ptr<Component> Clone() override;

	void Update(float _dt);

	void Serialize(JsonReader& _jr) override;

	void DeSerialize(JsonWriter& _jw) override;

	void Destroy() override;

	void AddTexture(std::string _textureName);
	void AddSubTexture(std::string _subTextureName);

	// setters and getters
	std::shared_ptr<Transform> GetTransform() { return m_transform; }

	void SetCurrTextureName(std::string _textureName);
	std::string GetCurrTextureName() const;

	std::string GetCurrSubTextureName();

	void SetSize(VEC2 _size);

	float GetAlpha();
	void SetAlpha(float _alpha);
	float GetDepth();
	void SetDepth(float _depth);
	void SetCurrFrame(unsigned int _currFrame);
	int GetID() { return mSpriteID; }
	void SetID(unsigned int _id);
	int GetCurrFrameNum();
	int GetMaxFrame();

	void SetCurrSubTextureName(std::string _subTextureName);
	void SetStartFrame(int _startFrame);
	void SetMaxFrame(int _maxFrame);

	std::string GetCurrSpriteListName();

	int GetStartFrame();
	void SetCurrentFrame(int _num);

	VEC2 GetFixedPosition();
	void SetFixedPosition(VEC2 _fixedPos);
	void SetFixedPositionX(float _x) { fixedPos.x = _x; }

	SpriteType GetType();
	SpriteMovementType GetMovementType();
	void SetMovementType(SpriteMovementType _movementType);

	void ChangeSubTexture(std::string _subTextureName);
	void ChangeAnimation(std::string _animationName);
	void ChangeAnimation(int _index);

	void ChangeGenericAnimation(std::string _animationName);

	VEC2 GetOffset();

	void FlipSprite();

	bool IsActive();

	void SetActive(bool _b = true);

	int GetSubTextureListSize();

	std::vector<std::string>& GetSubTextureList();

	//void AddSubTexture(std::string& _name);
};

