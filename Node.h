#pragma once
#include <iostream>
#include <vector>
#include "Vector2.h"

//#define uint unsigned int

struct Node {
	int vID = 2;
	VEC2 location;
	int worldGridX;
	int worldGridY;
	Node* parent;
	bool canTravel = true;
	int actualCost = 1;
	int totalCost = 0;  // = actualCost + estimatedCost

	// Create Start & End Node
	Node(int _index, VEC2 v, int mycost, int xCollide, int yCollide);
	// Create Path Node
	Node(Node* myparent, int myCost, int wgridX, int wgridY, int xCollide, int yCollide);

	Node(const Node& _rhs);
	Node& operator=(const Node& _rhs);





	// Comparison operator
	bool operator ==(const Node& rhs);
	bool operator ==(const Node& rhs) const;
	bool operator > (const Node& rhs);
	bool operator < (const Node& rhs);

	// Get Total Cost
	int getTotalCost();

	// Get Parent
	Node* GetParent();
	Node* GetParentPtr();

	void PrintNode();

	int GetvID();

	void SetTrue();
	void SetFalse();
};