#include "stdafx.h"
#include "Node.h"

/* start & end Node */
Node::Node(int _index, VEC2 v, int mycost, int xCollide, int yCollide) :
	vID{ _index }, location{ VEC2(v) }, actualCost{ mycost }, totalCost{ mycost }, parent{ nullptr }
{
	//Start from 0
	worldGridX = static_cast<int>(v.getx() / xCollide); // column
	worldGridY = static_cast<int>(v.gety() / yCollide); // row
}

/* Path Node */
Node::Node(Node* myparent, int myCost, int wgridX, int wgridY, int xCollide, int yCollide) :
	actualCost{ myCost }, 
	totalCost{ myparent->totalCost + actualCost },
	worldGridX{ wgridX }, worldGridY{ wgridY }
{
	parent = myparent;
	// Find location
	float x = static_cast<float>((worldGridX * xCollide) + (xCollide / 2.0f));
	float y = static_cast<float>((worldGridY * yCollide) + (yCollide / 2.0f));
	location = VEC2(x, y);
}

Node::Node(const Node& _rhs)
	:	vID{ _rhs.vID }, location{ _rhs.location }, worldGridX{ _rhs.worldGridX }, worldGridY{ _rhs.worldGridY }, parent{ _rhs.parent },
	canTravel{ _rhs.canTravel }, actualCost{ _rhs.actualCost }, totalCost{ _rhs.totalCost }
{
}

Node& Node::operator=(const Node& _rhs)
{
	vID = _rhs.vID;
	location = _rhs.location;
	worldGridX = _rhs.worldGridX;
	worldGridY = _rhs.worldGridY;
	parent = _rhs.parent;
	canTravel = _rhs.canTravel;
	actualCost = _rhs.actualCost;
	totalCost = _rhs.totalCost;  // = actualCost + estimatedCost


	return *this;
}

bool Node::operator==(const Node& rhs)
{
	bool x = (this->worldGridX == rhs.worldGridX);
	bool y = (this->worldGridY == rhs.worldGridY);
	if (x && y)
	{
		return true;
	}
	else
		return false;
}

bool Node::operator==(const Node& rhs) const
{
	bool x = (this->worldGridX == rhs.worldGridX);
	bool y = (this->worldGridY == rhs.worldGridY);
	if (x && y)
	{
		return true;
	}
	else
		return false;
}

bool Node::operator>(const Node& rhs)
{
	return this->totalCost > rhs.totalCost;
}

bool Node::operator<(const Node& rhs)
{
	return this->totalCost < rhs.totalCost;
}

int Node::getTotalCost()
{
	return totalCost;
}

Node* Node::GetParent()
{
	return parent;
}

Node* Node::GetParentPtr()
{
	return parent;
}

void Node::PrintNode()
{
	std::cout << vID << ": " << "x = " << location.getx() << ", y = " << location.gety() /*<< std::endl*/
		<< "     gx = " << worldGridX
		<< ", gy = " << worldGridY << " Total Cost: " << totalCost
		<< std::endl;
	if (parent)
	{
		std::cout << "Parent: gx = " << parent->worldGridX
			<< ", gy = " << parent->worldGridY << std::endl;
	}

}

int Node::GetvID()
{
	return vID;
}

void Node::SetTrue()
{
	canTravel = true;
}

void Node::SetFalse()
{
	canTravel = false;
}