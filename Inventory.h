/************************************************************************************ /
/*
File Name:		Inventory.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin	[50%]
				Kok Zhe Yan			[50%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

#include "Composition.h"
#include <array>
#include <memory>

#include "Factory.h"

#include "SpriteComponent.h"


#define INVENTORYSIZE 1
#define NOTESPERSTAGE 2

class Inventory
{
	int currSelectedSlot;

	std::weak_ptr<GOC> selectedPtr;	
	std::weak_ptr<SpriteComponent> inventoryBar;
	std::weak_ptr<SpriteComponent> noteBar;

	std::array<std::weak_ptr<GOC>, INVENTORYSIZE> itemList;
	std::array<std::weak_ptr<GOC>, NOTESPERSTAGE> noteList;
	std::array<VEC2, INVENTORYSIZE> invPosList;
	std::array<VEC2, NOTESPERSTAGE> notePosList;
public:
	Inventory();
	~Inventory();

	void Initialize();

	bool AddItem(std::shared_ptr<GOC> _item);

	bool AddNote(std::shared_ptr<GOC> _item);

	bool isItemListFull();

	bool isNoteListFull();

	bool isNoteListEmpty();

	void RemoveItem(int _index);

	bool UseCurrItem(const VEC2& pos, int frame);

	void Clear();

	// Set and getters
	int GetCurrSelectedSlot() { return currSelectedSlot; }
	void SetCurrSelectedSlot(int _selected);// { currSelectedSlot = _selected; }
	const char* GetCurrSelectedSlotGOCName();

	void SetInvenBarHalfTransparent();
	void SetInvenBarLessTransparent();
	
	void SetNoteBarHalfTransparent();
	void SetNoteBarLessTransparent();

	std::weak_ptr<GOC> GetItem(std::string _name);
};
