/************************************************************************************ /
/*
File Name:		Inventory.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin	[50%]
				Kok Zhe Yan			[50%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "Inventory.h"

// to spawn object
#include "Logic.h"
#include "FModSoundManager.h"
#include "Graphics.h"


#define ISPACING 80

Inventory::Inventory() : currSelectedSlot{ 0 }
{
	for (int i = 0; i < INVENTORYSIZE; ++i)
	{
		//invPosList[i] = VEC2(407 + static_cast<float>(ISPACING * i), 59);
		invPosList[i] = VEC2(125.0, 110.0);
	}
}

Inventory::~Inventory()
{
}

void Inventory::Initialize()
{
	FACTORY->CreateArcheType("InventoryItem");
	FACTORY->CreateArcheType("Banana");
	FACTORY->CreateArcheType("AlarmClock");
	FACTORY->CreateArcheType("Notes");
	FACTORY->CreateArcheType("NotesBar");
	FACTORY->CreateArcheType("InventoryBar");

	inventoryBar = LOGIC->CreateObjectAt(VEC2(0,0),0,"InventoryBar","invenbar", false)->has(SpriteComponent);
	inventoryBar.lock()->SetActive(true);

	noteBar = LOGIC->CreateObjectAt(VEC2(0,0),0,"NotesBar","nBar", false)->has(SpriteComponent);
	noteBar.lock()->SetActive(true);
	
	// selected slot
	currSelectedSlot = 0;

	for (int i = 0; i < NOTESPERSTAGE; ++i)
	{
		notePosList[i] = VEC2(1150.0f + static_cast<float>(ISPACING * i), 670.0f);
	}

}

bool Inventory::AddItem(std::shared_ptr<GOC> _item)
{
	for (int i = 0; i < INVENTORYSIZE; ++i)
	{
		if (itemList[i].lock() == nullptr)
		{
			//std::cout << "item added" << std::endl;

			itemList[i] = LOGIC->CreateObjectAt(invPosList[i], 0, _item->GetType(), _item->GetName());
			itemList[i].lock()->has(BoxCollider)->SetCanCollide(false);
			std::shared_ptr<SpriteComponent> sprPtr = itemList[i].lock()->has(SpriteComponent);
			MySpriteManager.normalToFixed(sprPtr->GetID());
			sprPtr->SetCurrFrame(_item->has(SpriteComponent)->GetCurrFrameNum());
			sprPtr->SetFixedPosition(invPosList[i]);
			itemList[i].lock()->SetInventory(true);
			_item->Destroy();
			return true;
		}
	}

	return false;
}

bool Inventory::AddNote(std::shared_ptr<GOC> _item)
{
	for (int i = 0; i < NOTESPERSTAGE; ++i)
	{
		if (noteList[i].lock() == nullptr)
		{
			//SetNoteBarLessTransparent();
			//std::cout << "item added" << std::endl;
			SOUNDMGR->PlaySoundFromMap("getNote");
			noteList[i] = LOGIC->CreateObjectAt(notePosList[i], 0, _item->GetType(), _item->GetName());
			noteList[i].lock()->has(BoxCollider)->SetCanCollide(false);
			std::shared_ptr<SpriteComponent> sprPtr = noteList[i].lock()->has(SpriteComponent);
			MySpriteManager.normalToFixed(sprPtr->GetID());
			sprPtr->SetCurrFrame(_item->has(SpriteComponent)->GetCurrFrameNum());
			sprPtr->SetFixedPosition(notePosList[i]);
			_item->Destroy();
			return true;
		}
	}
	return false;
}

bool Inventory::isItemListFull()
{
	for (int i = 0; i < INVENTORYSIZE; ++i)
	{
		if (itemList[i].lock() == nullptr)
			return false;
	}

	return true;
}

bool Inventory::isNoteListFull()
{
	for (int i = 0; i < NOTESPERSTAGE; ++i)
	{
		if (noteList[i].lock() == nullptr)
			return false;
	}

	return true;
}

bool Inventory::isNoteListEmpty()
{
	for (int i = 0; i < NOTESPERSTAGE; ++i)
	{
		if (noteList[i].lock() != nullptr)
			return false;
	}

	return true;
}


void Inventory::RemoveItem(int _index)
{
	if (std::shared_ptr<GOC> item = itemList[_index].lock())
		item->Destroy();

}

bool Inventory::UseCurrItem(const VEC2& pos, int frame)
{
	bool isUsed = false;

	if (std::shared_ptr<GOC> curritem = itemList[currSelectedSlot].lock())
	{
		std::shared_ptr<Script> script = LOGIC->GetScript(curritem->has(LogicComponent)->GetScriptName());
		script->SetOwner(curritem);
		script->UseItem(pos, frame);

		RemoveItem(currSelectedSlot);
		isUsed = true;
	}

	return isUsed;
}

void Inventory::Clear()
{
	selectedPtr.reset();
}

void Inventory::SetCurrSelectedSlot(int _selected)
{
	// safety check
	if (selectedPtr.lock() == nullptr)
		return;


	std::shared_ptr<SpriteComponent> spritePtr = selectedPtr.lock()->has(SpriteComponent);

	// safety check
	if (spritePtr == nullptr)
		return;

	spritePtr->SetFixedPosition(invPosList[_selected]);

	currSelectedSlot = _selected;
}

const char* Inventory::GetCurrSelectedSlotGOCName()
{
	if (std::shared_ptr<GOC> curritem = itemList[currSelectedSlot].lock())
	{
		return curritem->GetName();
	}
	else
		return "INVALID";
}

void Inventory::SetInvenBarHalfTransparent()
{
	inventoryBar.lock()->SetAlpha(0.5f);
}

void Inventory::SetInvenBarLessTransparent()
{
	inventoryBar.lock()->SetAlpha(0.8f);

}

void Inventory::SetNoteBarHalfTransparent()
{
	if (noteBar.lock())
		noteBar.lock()->SetAlpha(0.5f);
}

void Inventory::SetNoteBarLessTransparent()
{
	if (noteBar.lock())
		noteBar.lock()->SetAlpha(0.8f);
}

std::weak_ptr<GOC> Inventory::GetItem(std::string _name)
{
	//std::weak_ptr<GOC>, INVENTORYSIZE
	for (auto& weakPtr : itemList)
	{
		if (weakPtr.lock()->GetName() == _name)
		{
			return weakPtr;
		}
	}

	return std::weak_ptr<GOC>();
}
