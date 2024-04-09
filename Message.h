/*************************************************************************************/
/*
File Name:		Message.h
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#pragma once
#include "Vector2.h"

enum class MessageId
{
	None,
	Quit,
	Win,
	Lose,
	//Collide,
	//ToggleDebugInfo,
	CharacterKey,
	//MouseButton,
	MouseMove,
	FileDrop,
	NPCDistract,
	NPCCollide
};

// Base message class. New message types are defined by deriving from this
class Message
{
public:
	Message(MessageId _id) : m_Mid(_id) {};
	virtual ~Message() {};

	MessageId m_Mid;
};

// Message 
class NPCDISTRACT : public Message
{
public:
	NPCDISTRACT(VEC2 position, bool b) : Message(MessageId::NPCDistract), ObjPos(position), attractall(b) {};
	VEC2 ObjPos;
	bool attractall;
};

class NPCCOLLIDE : public Message
{
public:
	NPCCOLLIDE() : Message(MessageId::NPCCollide) {};
};