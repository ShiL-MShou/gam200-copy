/************************************************************************************ /
/*
File Name:		SpriteComponent.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Sprite component.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "SpriteComponent.h"
#include "Graphics.h"
#include "imgui.h"

#include "Animator.h"

// tmp for testing only
#include "BoxCollider.h"

#include "AnimationComponent.h"


SpriteComponent::SpriteComponent()
	:	 m_type{ SpriteType::SINGLE }, movementType{ SpriteMovementType::FREE }, m_size{ 1, 1 }, startFrame{ 0 }, 
	offset{}, isActive{ true }, alpha{1.0f}
{
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::Inspect()
{
	if (m_type == SpriteType::ANIMATED)
		ImGui::Text("Sprite Type: ANIMATED");
	else
		ImGui::Text("Sprite Type: FREE");

	if (movementType == SpriteMovementType::FIXED)
		ImGui::Text("Movement Type: FIXED");
	else
		ImGui::Text("Movement Type: FREE");

	ImGui::SliderFloat2("FixedPos", fixedPos.vec, 0.0f, static_cast<float>(WinWidth));

	ImGui::SliderFloat("Alpha", &alpha, 0.0f, 1.0f);

	if (m_type == SpriteType::SINGLE)
	{

		// --- subTexture selection ---
		ImGui::BeginChild("subtextures", ImVec2(150, 50), true);
		{
			for (auto& name : m_subTextureList)
			{
				// selection
				bool isSelected = (currSubTextureName == name);
				if (ImGui::Selectable(name.c_str(), isSelected))
				{
					// update selection
					currSubTextureName = name.c_str();
					ChangeSubTexture(currSubTextureName);

					maxFrame = GRAPHICS->GetSpriteList(currSubTextureName)->GetLastFrame();
				}
			}
		}
		ImGui::EndChild();


		if (ImGui::Button("Add"))
		{
			ImGui::OpenPopup("Animation List");
		}
		bool open = true;
		if (ImGui::BeginPopupModal("Animation List", &open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// --- texture selection ---
			ImGui::BeginChild("Textures", ImVec2(150, 50), true);
			{
				static const char* currtexture = currSubTextureName.c_str();

				for (auto& spriteListPtr : GRAPHICS->GetSpriteList())
				//for (auto& [name, graphicsFormat] : GRAPHICS->GetGraphicsMap())
				{
					// selection
					bool isSelected = (currTextureName == spriteListPtr->name);
					if (ImGui::Selectable(spriteListPtr->name.c_str(), isSelected))
					{
						// update current texture
						currTextureName = spriteListPtr->name.c_str();

						//auto [gname, va, vb, ib, texture] = graphicsFormat;

						// update subTextureList
						m_subTextureList.clear();

						//for (auto& [subtexturename, ignore] : texture->subTextureLst)
						if (std::find(std::begin(m_subTextureList), std::end(m_subTextureList), "spriteListPtr->name") == std::end(m_subTextureList))
						{
							m_subTextureList.push_back(spriteListPtr->name);
						}

						// update current subtexture
						currSubTextureName = spriteListPtr->name;// std::begin(texture->subTextureLst)->first;
						ChangeSubTexture(currSubTextureName);

						maxFrame = spriteListPtr->GetLastFrame();
					}
				}
			}
			ImGui::EndChild();

			if (ImGui::Button("Add"))
			{
				// add to list
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}


		//currSubTextureName = currtexture;

		ImGui::SliderFloat("Depth", &depth, -2.0f, 2.0f);

		// tmp fix
		maxFrame = GRAPHICS->GetSpriteList(currSubTextureName)->GetLastFrame();

		//TMP
		//maxFrame = 8;

		// change to selected startFrame and maxFrame
		ImGui::SliderInt("Start Frame", &startFrame, 0, maxFrame - 1);
		ImGui::SliderInt("Current Frame", &currFrame, 0, maxFrame - 1);
	}


	if (ImGui::Button("Deactivate"))
	{
		isActive = false;
		ImGui::CloseCurrentPopup();
	}

	if (ImGui::Button("Activate"))
	{
		isActive = true;
		ImGui::CloseCurrentPopup();
	}


}



void SpriteComponent::Initialize()
{
	assert(GetBase()->has(Transform));
	m_transform = GetBase()->has(Transform);

	MySpriteManager.AddSprite(GetBase()->has(SpriteComponent));


	if (m_type == SpriteType::SINGLE)
	{
		std::shared_ptr<Texture> tmpTexture = GRAPHICS->GetTexture(currTextureName);// ->GetNumCol();

		VEC2 tmpSize = GRAPHICS->GetSpriteList(currSubTextureName)->size;

		m_transform->SetScale(	tmpSize.x * m_size.x,
								tmpSize.y * m_size.y);
	}
	else if (m_type == SpriteType::ANIMATED)
	{
		std::shared_ptr<AnimationComponent> animationCompPtr = GetBase()->has(AnimationComponent);

		// safety checks
		if (animationCompPtr)
		{
			VEC2 tmpSize = GRAPHICS->GetAnimation(animationCompPtr->animationList.at(animationCompPtr->currAnimation))->size;

			m_transform->SetScale(tmpSize.x * m_size.x,
				tmpSize.y * m_size.y);
		}
		else
		{
			std::cout << "Animation component not found!" << std::endl;
		}
	}
	else
		assert(0 && "Sprite type is not recognised!");
}

std::shared_ptr<Component> SpriteComponent::Clone()
{
	std::shared_ptr<SpriteComponent> newComponent = std::make_shared<SpriteComponent>();

	newComponent->m_type = m_type;
	newComponent->movementType = movementType;
	newComponent->m_size = m_size;
	newComponent->depth = depth;
	//newComponent->m_layerNum = m_layerNum;
	newComponent->SetName(GetName());
	newComponent->startFrame = startFrame;
	newComponent->fixedPos = fixedPos;
	newComponent->offset = offset;
	newComponent->isActive = isActive;

	if (m_type == SpriteType::SINGLE)
	{
		newComponent->currFrame = currFrame;
		newComponent->maxFrame = maxFrame;
		newComponent->currTextureName = currTextureName;
		newComponent->currSubTextureName = currSubTextureName;
	}

	for (auto& texture : m_subTextureList)
	{
		if (std::find(std::begin(newComponent->m_subTextureList), std::end(newComponent->m_subTextureList), texture) 
			== std::end(newComponent->m_subTextureList))
			newComponent->m_subTextureList.push_back(texture);
	}

	return newComponent;
}

void SpriteComponent::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);
}

void SpriteComponent::Serialize(JsonReader& _jr)
{
	std::string type, tmpMovementType;

	//StreamRead(stream, m_name);

	std::string compName = "SpriteComponent";

	int textureCount = 0;
	int subTextureCount = 0;
	std::string textureName = "";
	std::string subTextureName = "";


	_jr.ReadStringMember(compName, "Type", type);
	_jr.ReadStringMember(compName, "MovementType", tmpMovementType);
	_jr.ReadFloatMember(compName, "Depth", depth);
	_jr.ReadFloatMember(compName, "ScaleX", m_size.x);
	_jr.ReadFloatMember(compName, "ScaleY", m_size.y);
	//_jr.ReadIntMember(compName, "LayerNum", m_layerNum);
	_jr.ReadIntMember(compName, "StartFrame", startFrame);


	// --- sprite type ---
	if (strcmp(type.c_str(), "SINGLE") == 0)
	{
		m_type = SpriteType::SINGLE;

		_jr.ReadIntMember(compName, "TextureCount", textureCount);

		{
			_jr.ReadStringMember(compName, "TextureName1", textureName);
			currTextureName = textureName;
		}

		_jr.ReadIntMember(compName, "SubTextureCount", subTextureCount);

		for (int i = 1; i <= subTextureCount; ++i)
		{
			_jr.ReadStringMember(compName, "SubTextureName" + std::to_string(i), subTextureName);

			AddSubTexture(subTextureName);
		}

		currFrame = startFrame;

		currSubTextureName = m_subTextureList.at(0);

		//maxFrame = GRAPHICS->GetTextureFrameMax(currTextureName, currSubTextureName);
	}
	else if (type == "ANIMATED")
		m_type = SpriteType::ANIMATED;

	// --- movement type ---
	if (tmpMovementType == "FIXED")
	{
		movementType = SpriteMovementType::FIXED;
		_jr.ReadFloatMember(compName, "FixedPosX", fixedPos.x);
		_jr.ReadFloatMember(compName, "FixedPosY", fixedPos.y);
	}
	else if (tmpMovementType == "FREE")
		movementType = SpriteMovementType::FREE;


	_jr.ReadFloatMember(compName, "OffsetX", offset.x);
	_jr.ReadFloatMember(compName, "OffsetY", offset.y);
}

// NOT DONE FOR THE NEW ANIMATION
void SpriteComponent::DeSerialize(JsonWriter& _jw)
{
	std::string type, tmpMovementType;

	if (m_type == SpriteType::SINGLE)
		type = "SINGLE";
	else if (m_type == SpriteType::ANIMATED)
		type = "ANIMATED";
	else
		type = "UNKNOWN";

	if (movementType == SpriteMovementType::FIXED)
		tmpMovementType = "FIXED";
	else
		tmpMovementType = "FREE";


	_jw.CreateKey(GetName());
	_jw.prettyWriterBuffer.StartObject();

	_jw.CreateStringMember("Type", type);
	_jw.CreateStringMember("MovementType", tmpMovementType);
	_jw.CreateFloatMember("Depth", depth);
	_jw.CreateFloatMember("ScaleX", m_size.x);
	_jw.CreateFloatMember("ScaleY", m_size.y);
	//_jw.CreateIntMember("LayerNum", m_layerNum);
	_jw.CreateIntMember("StartFrame", startFrame);

	if (m_type == SpriteType::SINGLE)
	{
		_jw.CreateIntMember("TextureCount", 1);
		_jw.CreateStringMember("TextureName1", currTextureName);

		_jw.CreateIntMember("SubTextureCount", static_cast<int>(m_subTextureList.size()));

		for (int i = 1; i <= m_subTextureList.size(); ++i)
		{
			_jw.CreateStringMember("SubTextureName" + std::to_string(i), m_subTextureList.at(i));
		}
	}

	// --- movement type ---
	if (movementType == SpriteMovementType::FIXED)
	{
		_jw.CreateFloatMember("FixedPosX", fixedPos.x);
		_jw.CreateFloatMember("FixedPosY", fixedPos.y);
	}

	_jw.prettyWriterBuffer.EndObject();
}

void SpriteComponent::Destroy()
{
	MySpriteManager.RemoveSprite(mSpriteID);
}


// TO REMOVE
void SpriteComponent::AddTexture(std::string _textureName)
{
	UNREFERENCED_PARAMETER(_textureName);
}

void SpriteComponent::AddSubTexture(std::string _subTextureName)
{
	if (std::find(std::begin(m_subTextureList), std::end(m_subTextureList), _subTextureName) == std::end(m_subTextureList))
		m_subTextureList.push_back(_subTextureName);
}


float SpriteComponent::GetAlpha()
{
	return alpha;
}

void SpriteComponent::SetAlpha(float _alpha)
{
	alpha = _alpha;
}

float SpriteComponent::GetDepth()
{
	return depth;
}

void SpriteComponent::SetDepth(float _depth)
{
	depth = _depth;
}

int SpriteComponent::GetCurrFrameNum()
{
	return currFrame;
}

int SpriteComponent::GetMaxFrame()
{
	return maxFrame;
}

void SpriteComponent::SetCurrSubTextureName(std::string _subTextureName)
{
	currSubTextureName = _subTextureName;
}

void SpriteComponent::SetStartFrame(int _startFrame)
{
	startFrame = _startFrame;
	currFrame = startFrame;
}

void SpriteComponent::SetMaxFrame(int _maxFrame)
{
	maxFrame = _maxFrame;

	//std::shared_ptr<Animator> animatorPtr = GetBase()->has(Animator);
	//if (animatorPtr != nullptr)
	//	animatorPtr->Initialize();

}

std::string SpriteComponent::GetCurrSpriteListName()
{
	return currSpriteListName;
}

int SpriteComponent::GetStartFrame()
{
	return startFrame;
}

void SpriteComponent::SetCurrentFrame(int _num)
{
	currFrame = _num;
}

VEC2 SpriteComponent::GetFixedPosition()
{
	return fixedPos;
}

void SpriteComponent::SetFixedPosition(VEC2 _fixedPos)
{
	fixedPos = _fixedPos;
}

SpriteType SpriteComponent::GetType()
{
	return m_type;
}

SpriteMovementType SpriteComponent::GetMovementType()
{
	return movementType;
}

void SpriteComponent::SetMovementType(SpriteMovementType _movementType)
{
	movementType = _movementType;
}

void SpriteComponent::ChangeSubTexture(std::string _subTextureName)
{
	auto itr = std::find(std::begin(m_subTextureList), std::end(m_subTextureList), _subTextureName);

	if (itr == std::end(m_subTextureList))
		assert(0 && "subTexture not found");

	currSubTextureName = _subTextureName;

	currTextureName = GRAPHICS->GetSpriteList(currSubTextureName)->textureName;

	//maxFrame = GRAPHICS->GetTextureFrameMax(currTextureName, _subTextureName);
	//if (currFrame >= maxFrame)
		currFrame = 0;

	// reset size
	//std::shared_ptr<Texture> tmpTexture = GRAPHICS->GetTexture(currTextureName);

	VEC2 tmpSize = GRAPHICS->GetSpriteList(currSubTextureName)->size;
	// = tmpTexture->GetSize(currSubTextureName);

	m_transform->SetScale(	tmpSize.x * m_size.x,
							tmpSize.y * m_size.y);


	//if (GetBase()->has(Animator))
	//	GetBase()->has(Animator)->Initialize();
}


// TO MOVE TO ANIMATION
void SpriteComponent::ChangeAnimation(std::string _animationName)
{
	std::shared_ptr<AnimationComponent> animationCompPtr = GetBase()->has(AnimationComponent);

	// safety checks
	if (animationCompPtr)
	{
		// curr animation is the same. IE no need to change animation.
		if (GRAPHICS->GetAnimationName(animationCompPtr->animationList.at(animationCompPtr->currAnimation)) == _animationName)
			return;

		for (int i = 0; i < animationCompPtr->animationList.size(); ++i)
		{
			if (GRAPHICS->GetAnimationName(animationCompPtr->animationList.at(i)) == _animationName)
			{
				std::shared_ptr<Animation> animationPtr = GRAPHICS->GetAnimation(animationCompPtr->animationList.at(i));

				// setting size
				VEC2 tmpSize = GRAPHICS->GetAnimation(GRAPHICS->GetAnimationID(_animationName))->size;

				m_transform->SetScale(tmpSize.x * m_size.x,
					tmpSize.y * m_size.y);


				// reset current frame if it is out of range
				if (animationCompPtr->currFrame >= animationPtr->lastFrame)
					animationCompPtr->currFrame = 0;

				animationCompPtr->currAnimation = i;
				return;
			}

		}

		// add animation into animation list
		animationCompPtr->animationList.push_back(GRAPHICS->GetAnimationID(_animationName));
		ChangeAnimation(_animationName);

		return;
	}

	assert(0 && "Animation Component not found!");
}

void SpriteComponent::ChangeAnimation(int _index)
{
	std::shared_ptr<AnimationComponent> animationCompPtr = GetBase()->has(AnimationComponent);

	// safety checks
	if (animationCompPtr)
	{
		ChangeAnimation(GRAPHICS->GetAnimationName(animationCompPtr->animationList.at(_index)));
	}

}

void SpriteComponent::ChangeGenericAnimation(std::string _animationName)
{
	//std::cout << "animation updated" << _animationName << " : " << GetBase()->GetName() << std::endl;


	std::shared_ptr<AnimationComponent> animationCompPtr = GetBase()->has(AnimationComponent);
	
	// safety checks
	if (animationCompPtr)
	{
		// curr animation is the same. IE no need to change animation.
		if (GRAPHICS->GetAnimationName(animationCompPtr->animationList.at(animationCompPtr->currAnimation)) == _animationName)
			return;

		std::string tmpAnimationStr;

		for (int i = 0; i < animationCompPtr->animationList.size(); ++i)
		{
			//std::cout << "finding: " << animationCompPtr->animationList.at(i) << std::endl;

			tmpAnimationStr = GRAPHICS->GetAnimationName(animationCompPtr->animationList.at(i));

			if (tmpAnimationStr.find(_animationName) != std::string::npos)
			{
				std::shared_ptr<Animation> animationPtr = GRAPHICS->GetAnimation(animationCompPtr->animationList.at(i));

				// setting size
				VEC2 tmpSize = GRAPHICS->GetAnimation(GRAPHICS->GetAnimationID(tmpAnimationStr))->size;

				m_transform->SetScale(tmpSize.x * m_size.x,
					tmpSize.y * m_size.y);


				// reset current frame if it is out of range
				//if (animationCompPtr->currFrame >= animationPtr->lastFrame)
					animationCompPtr->currFrame = 0;

				animationCompPtr->currAnimation = i;
				animationCompPtr->currFrameTime = animationPtr->frames.at(animationCompPtr->currFrame).timeDelay;
				return;
			}

		}

		//// add animation into animation list
		//animationCompPtr->animationList.push_back(GRAPHICS->GetAnimationID(tmpAnimationStr));
		//ChangeAnimation(tmpAnimationStr);
		//
		//return;
	}

	std::cout << _animationName << "ANIMATION NOT FOUND!" << std::endl;
	//assert(0 && "Animation Component not found!");
}

VEC2 SpriteComponent::GetOffset()
{
	return offset;
}



void SpriteComponent::FlipSprite()
{
	//std::cout << "Flip called" << std::endl;

	m_transform->SetScale(m_transform->GetScale().x * -1, m_transform->GetScale().y);
}

bool SpriteComponent::IsActive()
{
	return isActive;
}

void SpriteComponent::SetActive(bool _b)
{
	isActive = _b;
}

int SpriteComponent::GetSubTextureListSize()
{
	return static_cast<int>(m_subTextureList.size());
}

std::vector<std::string>& SpriteComponent::GetSubTextureList()
{
	return m_subTextureList;
}

//void SpriteComponent::AddSubTexture(std::string& _name)
//{
//	m_subTextureList.push_back(_name);
//}

void SpriteComponent::SetCurrFrame(unsigned int _currFrame)
{
	currFrame = _currFrame;
}

void SpriteComponent::SetID(unsigned int _id)
{
	mSpriteID = _id;
}

void SpriteComponent::SetCurrTextureName(std::string _textureName)
{
	currTextureName = _textureName;
}

std::string SpriteComponent::GetCurrTextureName() const
{
	return currTextureName;
}

std::string SpriteComponent::GetCurrSubTextureName()
{
	return currSubTextureName;
}

void SpriteComponent::SetSize(VEC2 _size)
{
	m_size = _size;
}
