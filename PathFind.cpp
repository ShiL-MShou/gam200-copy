#include "stdafx.h"
#include "PathFind.h"
#include "ForceTypes.h"
#include <queue>
#include "Physics.h"
#include <Logic.h>

// debug draw
#include "DebugDraw.h"

// memory allocator
#include "ObjectAllocator.h"

template<
	class T,
	class Container = std::vector<T>,
	class Compare = std::less<typename Container::value_type>
> class MyQueue : public std::priority_queue<T, Container, Compare>
{
public:
	typedef typename
		std::priority_queue<
		T,
		Container,
		Compare>::container_type::const_iterator const_iterator;

	const_iterator find(const T& val) const
	{
		auto first = this->c.cbegin();
		auto last = this->c.cend();
		while (first != last) {
			if (*(*first) == *val)
				return first;
			++first;
		}
		return last;
	}

	const_iterator cend() const
	{
		auto last = this->c.cend();
		return last;
	}

	void replace(const T& o, const T& n)
	{
		auto first = this->c.begin();
		auto last = this->c.end();
		while (first != last) {
			if (*first == o)
			{
				this->c.erase(first);
				this->c.insert(first, n);
				break;
				//*first = n;
			}
			++first;
		}
	}
};

struct CompareNodeCost /*: public std::binary_function<Node, Node, bool>*/
{
	bool operator() (Node* lhs, Node* rhs) const
	{
		if (lhs->totalCost == rhs->totalCost)
		{
			return false;
		}
		return lhs->totalCost > rhs->totalCost;
	}
};
Node* AllocateMem()
{
	return reinterpret_cast<Node*>(LOGIC->_oa->Allocate());;
}
/*/
// Not in use anymore
VEC2 GridtoWorld(VEC2 gridPos)
{
	float x = gridPos.getx() - (cWIDTH * MapWidth / 2);
	float y = gridPos.gety() - (cHEIGHT * MapHeight / 2);
	return VEC2(x, y);
}
VEC2 WorldtoGrid(VEC2 worldPos)
{
	float x = worldPos.getx() + (cWIDTH * MapWidth / 2);
	float y = worldPos.gety() + (cHEIGHT * MapHeight / 2);
	return VEC2(x, y);
}
/*/
bool TestGridStat(VEC2 v)
{
	std::vector <std::vector<bool>>& all = LOGIC->GetAImap();
	int wgx = static_cast<int>((v.getx()) / cWIDTH); // column
	int wgy = static_cast<int>((v.gety()) / cHEIGHT); // row
	return all[wgy][wgx];
}

bool TestGridStat(int x, int y)
{
	std::vector <std::vector<bool>>& all = LOGIC->GetAImap();
	return all[y][x];
}

void SetAllTrue(std::vector <std::vector<bool>>& all)
{
	for (size_t i = 0; i < MapHeight; i++)
	{
		for (size_t k = 0; k < MapWidth; k++)
			all[i][k] = true;
	}
}

void SetAllFalse(std::vector <std::vector<bool>>& all)
{
	for (size_t i = 0; i < MapHeight; i++)
	{
		for (size_t k = 0; k < MapWidth; k++)
			all[i][k] = false;
	}
}

void SetTrue(std::vector <std::vector<bool>>& all, VEC2 pos, int xCollide, int yCollide)
{
	int x = static_cast<int>(pos.getx() / xCollide);
	int y = static_cast<int>(pos.gety() / yCollide);
	all[y][x] = true;
}

void SetFalse(std::vector <std::vector<bool>>& all, VEC2 pos, int xCollide, int yCollide)
{
	int x = static_cast<int>(pos.getx() / xCollide);
	int y = static_cast<int>(pos.gety() / yCollide);
	all[y][x] = false;

	//(*(all + x) + y) = false;
}

void SetTrue(std::vector <std::vector<bool>>& all, int x, int y)
{
	all[y][x] = true;
}

void SetFalse(std::vector <std::vector<bool>>& all, int x, int y)
{
	all[y][x] = false;
}

void SetState(std::vector <std::vector<bool>>& all, int x, int y, bool s)
{
	all[y][x] = s;
}

int GetGridX_index(VEC2 v)
{
	//std::vector <std::vector<bool>>& all = LOGIC->GetAImap();
	return static_cast<int>((v.getx()) / cWIDTH); // column
}

int GetGridY_index(VEC2 v)
{
	//std::vector <std::vector<bool>>& all = LOGIC->GetAImap();
	return static_cast<int>((v.gety()) / cHEIGHT); // row
}

VEC2 GetGrid_index(VEC2 v)
{
	return VEC2(floorf(v.getx() / cWIDTH), floorf(v.gety() / cHEIGHT));
}

VEC2 GetPos_index(int x, int y)
{
	float px = static_cast<float>((x * cWIDTH) + (cWIDTH / 2.0f));
	float py = static_cast<float>((y * cHEIGHT) + (cHEIGHT / 2.0f));
	return VEC2(px, py);
}

std::vector<VEC2> FindPath(std::vector<std::vector<bool>>& all, VEC2 src, VEC2 des)
{
	//if (LOGIC->IsPathFindCalled())
	//	return std::vector<VEC2>();


	//LOGIC->PathFindCalled();
	

	// Open List
	MyQueue<Node*, std::vector<Node*>, CompareNodeCost> openList;

	// Close List
	std::vector<Node*> closeList;

	// Find src grid + des grd

	//Node* mem = reinterpret_cast<Node*>(oa->Allocate());
	Node* start = new (AllocateMem()) Node(0, src, 0, cHEIGHT, cWIDTH);

	//mem = reinterpret_cast<Node*>(oa->Allocate());
	Node* target = new (AllocateMem()) Node(-1, des, 1, cHEIGHT, cWIDTH);

	Node* neighbor;

	//Node* start = std::make_shared<Node>(0, src, 0, cHEIGHT, cWIDTH);
	//std::shared_ptr<Node> target = std::make_shared<Node>(-1, des, 1, cHEIGHT, cWIDTH);
	//std::shared_ptr<Node> neighbor;
	bool inCloseList = false;
	bool inOpenList = false;

	//bool targetReachable = false;
	//std::shared_ptr<Node> tmpTgt;
	Node* tmpTgt;

	std::vector<Node*> connectPts;
	std::vector<VEC2> path;

	openList.push(start);

	//std::cout << "Path finding: " << target->worldGridX << " | " << target->worldGridY << std::endl;

	//start->PrintNode();
	//target->PrintNode();
	//std::cout << TestGridStat(src) << std::endl;
	//std::cout << TestGridStat(des) << std::endl;

	while (!openList.empty())
	{
		Node* curr = openList.top();
		//curr->PrintNode();
		
		/* Reach target */
		if (*curr == *target)
			//targetReachable = true;
		{
			std::cout << "PATH FOUND!" << std::endl;
			Node* p = curr;
			//path.push_back((target->location));
			path.emplace(path.begin(), (p->location));
			
			//p = p->parent.lock();

			// while parent is not nullptr
			p = p->parent;
			while (p)
			{
				// rig
				if (p->totalCost != 0)
				{
					path.emplace(path.begin(), (p->location));
					//p.get()->PrintNode();
				}
				p = p->parent;
			}
			//path.pop_front();
			closeList.clear();
			while (!openList.empty())
				openList.pop();
			connectPts.clear();
			LOGIC->_oa->Clear();
			return path;
		}

		// Create Neighbour of node
		connectPts.clear();

		int currX = curr->worldGridX;
		int currY = curr->worldGridY;

		// Check if border of the map
		if ((currY + 1) < MapHeight) // UP
		{
			connectPts.push_back(new (AllocateMem()) Node(curr, 1, currX, currY + 1, cHEIGHT, cWIDTH));
		}
		if ((currX + 1) < MapWidth) // RIGHT
		{
			connectPts.push_back(new (AllocateMem()) Node(curr, 1, currX + 1, currY, cHEIGHT, cWIDTH));
		}
		if ((currY - 1) > -1) // DOWN
		{
			connectPts.push_back(new (AllocateMem()) Node(curr, 1, currX, currY - 1, cHEIGHT, cWIDTH));
		}
		if ((currX - 1) > -1) // LEFT
		{
			connectPts.push_back(new (AllocateMem()) Node(curr, 1, currX - 1, currY, cHEIGHT, cWIDTH));
		}

		// For all neighbour
		for (size_t i = 0; i < connectPts.size(); i++)
		{
			neighbor = connectPts[i];
			//if (neighbor->GetParent() != nullptr)
			//	neighbor->totalCost = neighbor->GetParent()->totalCost;
			//++neighbor->totalCost;
			
			// Check Grid if can travel aimap[column][row]
			if (!all[neighbor->worldGridY][neighbor->worldGridX]) {
				continue;// cannot travel false continue
			}

			//if (tmpTgt  == nullptr && *curr == *target)
			//	tmpTgt = curr;
			//else if (tmpTgt != nullptr && tmpTgt->totalCost > curr->totalCost)
			//	tmpTgt = curr;

			// check in open list
			auto result = openList.find(neighbor);
			if (result != openList.cend())
			{
				inOpenList = true;

				//std::cout << neighbor->totalCost << " " << (*result)->totalCost << std::endl;
				// use the one with lesser cost
   				if (neighbor->totalCost < (*result)->totalCost)
				{
					openList.replace(*result, neighbor);

					if (*curr == *target)
						tmpTgt = curr;
				}

				//break;
			}
			else
			{
				if (*curr == *target)
					tmpTgt = curr;
			}

			if (inOpenList) { // Found in open? list, next neighbor
				inOpenList = false;
			
			
			
				continue;
			}

			// check in closed list
			for (Node* q : closeList)
			{
				// Found in close, stop checking
				if (*neighbor == *q) {

					//std::cout << neighbor->totalCost << " " << q->totalCost << std::endl;
					// use the one with lesser cost
					//if (neighbor->totalCost < q->totalCost)
					//{
					//	q = neighbor;
					//
					//	// if (*curr == *target)
					//	//	tmpTgt = curr;
					//}


					inCloseList = true;
					break;
				}
			}
			if (inCloseList) // Found in close list, next neighbor
			{
				inCloseList = false;
				continue;
			}

			//++neighbor->totalCost;
			openList.push(neighbor);

		}
		
		
		closeList.push_back(curr);
		// Remove curr
		openList.pop();
	}
	//if (*curr == *target)
	//{
	//	std::cout << "PATH FOUND!" << std::endl;
	//	std::shared_ptr<Node> p = curr;
	//	path.push_back((target->location));
	//	path.emplace(path.begin(), (p->location));

	//	// while parent is not nullptr
	//	while (p = p->parent.lock())
	//	{
	//		path.emplace(path.begin(), (p.get()->location));
	//	}
	//	return path;
	//}

	// target is found
	//if (targetReachable)
	//{
	//	//std::cout << "PATH FOUND!" << std::endl;
	//	//const auto& itr = std::find(std::begin(closeList), std::end(closeList), target);
	//	//if (itr == std::end(closeList))
	//	//	return path;

	//	//std::shared_ptr<Node> p = target;

	//	for (auto& Node : closeList)
	//	{
	//		//std::cout << "finding: " << target->location.x << " " << target->location.y <<
	//		//	" | " << Node->location.x << " " << Node->location.y << std::endl;

	//		if (*Node == *target)

	//		//if (Node->location == target->location)
	//		{
	//			//p = Node;
	//			std::cout << "found ! " << std::endl;
	//			path.emplace(path.begin(), (Node->location));
	//			while (Node = Node->parent.lock())
	//			{
	//				std::cout << "PEW PEW" << std::endl;
	//				path.emplace(path.begin(), (Node.get()->location));
	//			}

	//			break;
	//		}
	//			
	//	}
	//	/*if (p != nullptr)
	//	{
	//		path.emplace(path.begin(), (p->location));
	//		while (p = p->parent.lock())
	//		{
	//			std::cout << "PEW PEW" << std::endl;
	//			path.emplace(path.begin(), (p.get()->location));
	//		}
	//	}*/
	//	
	//	
	//	return path;
	//}


	std::cout << "NO PATH FOUND!" << std::endl;
	//path.push_back((start->location));
	closeList.clear();
	while (!openList.empty())
		openList.pop();
	connectPts.clear();
	LOGIC->_oa->Clear();
	return path;
}

bool CheckAllSide(VEC2 Npos, int range)
{
	//Npos & Playerpos = VEC2 in world
		// Can get player position here
	VEC2 Ppos = PHYSICS->GetPlayerGOC().lock()->has(Transform)->GetPosition();
	// change Npos & Playerpos using World to grid
	// get their array number
	int gNposX = static_cast<int>(Npos.getx() / cWIDTH); // x, column, w
	int gNposY = static_cast<int>(Npos.gety() / cHEIGHT); // y, row, h
	
	int gPposX = static_cast<int>(Ppos.getx() / cWIDTH); // x, column, w
	int gPposY = static_cast<int>(Ppos.gety() / cHEIGHT); // y, row, h
	// Get all the grid it needs to check (UP DOWN LEFT RIGHT)
	int up = (gNposY + range);
	int down = (gNposY - range);
	int right = (gNposX + range);
	int left = (gNposX - range);

	// if player in the list of grid, return true
	if (left <= gPposX && right >= gPposX && gNposY == gPposY)
	{
		//std::cout << "NPC see something left or right." << std::endl;
		return true;
	}
	else if (up >= gPposY && down <= gPposY && gNposX == gPposX)
	{
		//std::cout << "NPC see something up or down." << std::endl;
		return true;
	}
	else
		return false;

	// Check for blockage
	
	// default return
	return false;
}

bool CheckOneSide(VEC2 Npos, int range, int v)
{
	bool canSee = false;
	bool noBlock = true;
	int checkWith = 0;
	// left(0) right(1) up(2) down(3)
		// Can get player position here
	VEC2 Ppos = PHYSICS->GetPlayerGOC().lock()->has(Transform)->GetPosition();
	int gNposX = static_cast<int>(Npos.getx() / cWIDTH); // x, column, w
	int gNposY = static_cast<int>(Npos.gety() / cHEIGHT); // y, row, h

	int gPposX = static_cast<int>(Ppos.getx() / cWIDTH); // x, column, w
	int gPposY = static_cast<int>(Ppos.gety() / cHEIGHT); // y, row, h
	
	if (v == 0) // left
	{
		checkWith = gNposX - range;
		// checkwith <- X
		// Y = Y
		if (checkWith <= gPposX && gNposX >= gPposX && gNposY == gPposY) {
			//std::cout << "NPC see something on the left." << std::endl;
			canSee = true;
		}
	}
	else if (v == 1) // right
	{
		checkWith = gNposX + range;
		// X -> checkwith
		// Y = Y
		if (gNposX <= gPposX && checkWith >= gPposX && gNposY == gPposY) {
			//std::cout << "NPC see something on the right." << std::endl;
			canSee = true;
		}
	}
	else if (v == 2) // up
	{
		checkWith = gNposY + range;
		// Y -> checkwith
		// X = X
		if (checkWith >= gPposY && gNposY <= gPposY && gNposX == gPposX) {
			//std::cout << "NPC see something up." << std::endl;
			canSee = true;
		}
	}
	else if (v == 3) // down
	{
		checkWith = gNposY - range;
		// checkwith <- Y
		// X = X
		if (gNposY >= gPposY && checkWith <= gPposY && gNposX == gPposX) {
			//std::cout << "NPC see something down." << std::endl;
			canSee = true;
		}
	}
	else
		canSee = false;

	//check for blockage
	if (v == 0) // left
	{
		// from npc x to player x
		for (int i = gNposX; i > gPposX; i--)
			if (!CheckRoadCutoff(i, gNposY))
				noBlock = false;
	}
	else if (v == 1) // right
	{
		for (int i = gNposX; i < gPposX; i++)
			if (!CheckRoadCutoff(i, gNposY))
				noBlock = false;
	}
	else if (v == 2) // up
	{
		for (int i = gNposY; i < gPposY; i++)
			if (!CheckRoadCutoff(gNposX, i))
				noBlock = false;
	}
	else if (v == 3) // down
	{
		for (int i = gNposY; i > gPposY; i--)
			if (!CheckRoadCutoff(gNposX, i))
				noBlock = false;
	}
	else
		noBlock = false;
	
	if (canSee && noBlock)
	{
		return true;
		/*Message msg(MessageId::Lose);
		LOGIC->SendMessageW(msg);*/
	}

	// default return
	return false;
}

// check blockage function using grid index
// false = cannot walk, true = can walk
bool CheckRoadCutoff(int x, int y)
{
	std::vector <std::vector<bool>>& all = LOGIC->GetAImap();
	// in map, 1 is walkable
	if (x >= MapWidth)
		x = MapWidth - 1;

	x = x >= MapWidth ? MapWidth - 1 : x;
	y = y >= MapHeight ? MapHeight - 1 : y;

	if (!all[y][x]) // cannot travel
	{
		return false;
	}
	else
		return true;

	return true;
}

bool DetectedPlayer(const VEC2& Npos, int dir, float range)
{
	VEC2 Ppos = PHYSICS->GetPlayerGOC().lock()->has(Transform)->GetPosition();
	VEC2 PNpos = Ppos - Npos;
	VEC2Normalize(PNpos, PNpos);

	// Check if NPC is inside viewport
	if (PHYSICS->PointToAABBIsColliding(Npos, GRAPHICS->GetCameraAABB()) ==  false)
	{
		return false;
	}

	if (VEC2Distance(Ppos, Npos) > range)
	{
		//std::cout << VEC2Distance(Npos, Ppos) << std::endl;
		return false;
	}
	//std::cout << "inrange" << std::endl;

	float x;
	float y;

	if (dir == 0)
	{
		x = -1;
		y = 0;
	}
	else if (dir == 1)
	{
		x = 1;
		y = 0;
	}
	else if (dir == 2)
	{
		x = 0;
		y = 1;
	}
	else
	{
		x = 0;
		y = -1;
	}

	VEC2 vel{ x, y };

	float dotproduct = Vec2Dot(PNpos, vel);

	if (dotproduct < 0.7f) // approx 45degree cone detection
	{
		return false;
	}

	auto colliderboxes = PHYSICS->GetBoxCollidersList(CollideTypes::ENVIRONMENT);

	for (auto& collider : colliderboxes)
	{
		auto box = MyResource.GetComponentWithID<BoxCollider>(collider);

		AABB camaabb = GRAPHICS->GetCameraAABB();

		if (box->GetCollidedAABB().CheckCollided(&camaabb) == false)
			continue;

		if (PHYSICS->LineToAABBisColliding(Ppos, Npos, box->GetCollidedAABB()))
		{
			//std::cout << "cannot see player" << std::endl;
			return false;
		}
	}

	return true;
}

