#include "stdafx.h"
//#include "Animator.h"
//#include "imgui.h"
//
#include "Sprite.h"
//
//Animator::Animator() 
//	: currFrame{ 1 }, maxFrame{ 1 }, updateInterval{ 0.5 }, timeTillNextFrame{ updateInterval }
//{
//	//currFrame;
//	//prevUpdateTime;
//	//updateInterval;
//	//data[20];
//}
//
//Animator::~Animator()
//{
//}
//
//void Animator::Initialize()
//{
//	spritePtr = GetBase()->has(Sprite);
//
//	if (spritePtr)
//		maxFrame = spritePtr->GetMaxFrame();
//	else // sprite not found
//		assert(0);
//}
//
//void Animator::Inspect()
//{
//	ImGui::SliderFloat("UpdateInterval", &updateInterval, 0.0f, 5.0f);
//}
//
//std::shared_ptr<Component> Animator::Clone()
//{
//	std::shared_ptr<Animator> newComponent = std::make_shared<Animator>();
//
//	newComponent->updateInterval = updateInterval;
//
//	return newComponent;
//}
//
//void Animator::Update(float _dt)
//{
//	UNREFERENCED_PARAMETER(_dt);
//
//	timeTillNextFrame -= _dt;
//
//	if (timeTillNextFrame <= 0)
//	{
//		// update and reset if it exceeds
//		if (++currFrame >= maxFrame)
//			currFrame = 0;
//
//		if (GetBase()->has(Sprite))
//			GetBase()->has(Sprite)->SetCurrFrame(currFrame);
//
//		// reset
//		timeTillNextFrame = updateInterval;
//	}
//}
//
//void Animator::Serialize(ISerialization& stream)
//{
//	// ---------------------------
//									//	currFrame
//	//	updateInterval
//	// ---------------------------
//
//	//StreamRead(stream, currFrame);
//	StreamRead(stream, updateInterval);
//}
//
//void Animator::Serialize(JsonReader& _jr)
//{
//	std::string compName = "Animator";
//
//	_jr.ReadFloatMember(compName, "UpdateInterval", updateInterval);
//}
//
//void Animator::DeSerialize(JsonWriter& _jw)
//{
//	_jw.CreateKey(GetName());
//	_jw.prettyWriterBuffer.StartObject();
//	_jw.CreateFloatMember("UpdateInterval", updateInterval);
//	_jw.prettyWriterBuffer.EndObject();
//}
//
//void Animator::ChangeAnimation(unsigned int _maxFrame)
//{
//	maxFrame = _maxFrame;
//}
//
//void Animator::SetMaxFrame(unsigned int _maxFrame)
//{
//	maxFrame = _maxFrame;
//}
//
//void Animator::Destroy()
//{
//}
//
//bool Animator::IsAnimationEnd()
//{
//	return (maxFrame-1) == currFrame;
//}
