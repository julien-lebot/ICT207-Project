//  collsion.h
//  Header file for the collision class
// 
//
//  Shay Leary, March 2005
//--------------------------------------------------------------------------------------
#ifndef COLLISION_H
#define COLLISION_H

//--------------------------------------------------------------------------------------

#include "AABB.h"
#include "AABBLinkedList.h"

//--------------------------------------------------------------------------------------

class Collision
{
public:

	Collision() {}
	virtual ~Collision() {}

	//----------------------------------------------------------------------------------
	//  Set Methods
	//----------------------------------------------------------------------------------
	// sets initial co-ordinates of bounding boxes (these set the co-ords is AABB, the array,
	// the values for the list are copied from the array using CreateLinkedList).
	void SetAABBMaxX(const int & tempIndex, const double &tempX) {m_AABB.SetMaxX(tempIndex, tempX);}
	void SetAABBMinX(const int & tempIndex, const double &tempX) {m_AABB.SetMinX(tempIndex, tempX);}
	void SetAABBMaxY(const int & tempIndex, const double &tempY) {m_AABB.SetMaxY(tempIndex, tempY);}
	void SetAABBMinY(const int & tempIndex, const double &tempY) {m_AABB.SetMinY(tempIndex, tempY);}
	void SetAABBMaxZ(const int & tempIndex, const double &tempZ) {m_AABB.SetMaxZ(tempIndex, tempZ);}
	void SetAABBMinZ(const int & tempIndex, const double &tempZ) {m_AABB.SetMinZ(tempIndex, tempZ);}

	// sets the actual world co-ordinates
	void SetWorldX(const double &tempX) {m_worldSizeX = tempX;}
	void SetWorldZ(const double &tempZ) {m_worldSizeZ = tempZ;}

	// set number of bounding boxes
	void SetNoBoundingBoxes(const int & tempSize) {m_AABB.SetNoBoundingBoxes(tempSize);}

	//----------------------------------------------------------------------------------
	//  Returns Methods
	//----------------------------------------------------------------------------------
	// returns co-ordinates of bounding boxes
	double GetAABBMaxX (const int & tempIndex) {return m_AABB.GetMaxX (tempIndex);}
	double GetAABBMinX (const int & tempIndex) {return m_AABB.GetMinX (tempIndex);}
	double GetAABBMaxY (const int & tempIndex) {return m_AABB.GetMaxY (tempIndex);}
	double GetAABBMinY (const int & tempIndex) {return m_AABB.GetMinY (tempIndex);}
	double GetAABBMaxZ (const int & tempIndex) {return m_AABB.GetMaxZ (tempIndex);}
	double GetAABBMinZ (const int & tempIndex) {return m_AABB.GetMinZ (tempIndex);}

	// returns number of bounding boxes
	int GetNoBoundingBoxes() {return m_AABB.GetNoBoundingBoxes();}

	// returns TRUE if a collsion occurred
	bool Collide (double endX, double endY, double endZ);

	// reads the BB info from AABB (dynamic array) and creates a Linked List
	// containing BB data
	void CreateLinkedList();

private:
	// initially stores BB info in AABB (dynamic array) before copying to Linked List
	AABB m_AABB;

	// lists to store bounding box info in each quadrant
	AABBLinkedList m_list[4];

	// Stores the list size of each linked list
	// Set to 4 has the world is split into 4 quadrants
	int m_listSize[4];

	// stores world co-ordinates
	double m_worldSizeX;
	double m_worldSizeZ;

	// checks if collsion occurred (called from Collide)
	bool CheckCollision(int index, double endX, double endY, double endZ);

	//----------------------------------------------------------------------------------

    // Privatised copy constructor and assignment operator
    Collision (const Collision &coll) {};
    Collision &operator = (const Collision &coll) {};
};

#endif
//--------------------------------------------------------------------------------------