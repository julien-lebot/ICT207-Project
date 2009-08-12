//  collision.cpp
//
//  Implementation file for Collsion Class
//  Defines all the methods declared, but not defined, in collsion.h
//
//  Shay Leary, March 2005
//--------------------------------------------------------------------------------------

#include "collision.h"

//--------------------------------------------------------------------------------------
//  Creates a linked list for each quadrant and then copies the bounding box data from
//  AABB (array) to the required linked list.
//  I initially just created AABB, then decided to place the bounding box info into a list.
//  I kept the array has it allows for the data to be copied into the list in any reuired 
//  order.
//--------------------------------------------------------------------------------------
void Collision::CreateLinkedList()

{
	int tempNoBoxes = GetNoBoundingBoxes();
	// initilize list size for each quadrant
	m_listSize[0] = 0;
	m_listSize[1] = 0;
	m_listSize[2] = 0;
	m_listSize[3] = 0;

	for (int count = 0; count < tempNoBoxes; count++)
	{
		GLdouble maxX = GetAABBMaxX(count);
		GLdouble minX = GetAABBMinX(count);
		GLdouble maxY = GetAABBMaxY(count);
		GLdouble minY = GetAABBMinY(count);
		GLdouble maxZ = GetAABBMaxZ(count);
		GLdouble minZ = GetAABBMinZ(count);
		// 1st quadrant
		if (((minX <= m_worldSizeX / 2.0) || (maxX <= m_worldSizeX / 2.0)) && 
			((minZ <= m_worldSizeZ / 2.0) || (maxZ <= m_worldSizeZ / 2.0)))
		{
			// increment list size
			m_listSize[0]++;
			// add bb data to list
			m_list[0].AddToStart(maxX, minX, maxY, minY, maxZ, minZ);
		}
		// 2nd quadrant
		if (((minX <= m_worldSizeX / 2.0) || (maxX <= m_worldSizeX / 2.0)) &&
			((minZ >= m_worldSizeZ / 2.0) || (maxZ >= m_worldSizeZ / 2.0)))
		{
			// increment list size
			m_listSize[1]++;
			// add bb data to list
			m_list[1].AddToStart(maxX, minX, maxY, minY, maxZ, minZ);
		}
		// 3rd quadrant
		if (((minX >= m_worldSizeX / 2.0) || (maxX >= m_worldSizeX / 2.0)) && 
			((minZ <= m_worldSizeZ / 2.0) || (maxZ <= m_worldSizeZ / 2.0)))
		{
			// increment list size
			m_listSize[2]++;
			// add bb data to list
			m_list[2].AddToStart(maxX, minX, maxY, minY, maxZ, minZ);
		}
		// 4th quadrant
		if (((minX >= m_worldSizeX / 2.0) || (maxX >= m_worldSizeX / 2.0)) && 
			((minZ >= m_worldSizeZ / 2.0) || (maxZ >= m_worldSizeZ / 2.0)))
		{
			// increment list size
			m_listSize[3]++;
			// add bb data to list
			m_list[3].AddToStart(maxX, minX, maxY, minY, maxZ, minZ);
		}
	}
	// Call AABB constructor the delete array and clear memory
	// (the array AABB is not required once the lists have been created)
	m_AABB.~AABB();
}

//--------------------------------------------------------------------------------------
//  Returns TRUE if a collsion occurs.
//  The parameters passed to this function are the co-ordinates of the camera.
//  At present this function uses if statements to split the world into quadrants
//  this is to be improved (for the project) so that the quadrants (or levels) are
//  stored in a list or tree data structure.
//--------------------------------------------------------------------------------------

bool Collision::Collide(GLdouble endX, GLdouble endY, GLdouble endZ)
{
	bool Collision = false;
	// check 1st quadrant (1st linked list)
	if (((endX <= m_worldSizeX / 2.0) || (endX <= m_worldSizeX / 2.0)) && 
		((endZ <= m_worldSizeZ / 2.0) || (endZ <= m_worldSizeZ / 2.0)))
	{
		Collision = CheckCollision(0, endX, endY, endZ);
	}
	// check 2nd quadrant (2nd linked list)
	if (((endX <= m_worldSizeX / 2.0) || (endX <= m_worldSizeX / 2.0)) && 
		((endZ >= m_worldSizeZ / 2.0) || (endZ >= m_worldSizeZ / 2.0)))
	{
		Collision = CheckCollision(1, endX, endY, endZ);
	}
	// check 3rd quadrant (3rd linked list)
	if (((endX >= m_worldSizeX / 2.0) || (endX >= m_worldSizeX / 2.0)) && 
	((endZ <= m_worldSizeZ / 2.0) || (endZ <= m_worldSizeZ / 2.0)))
	{
		Collision = CheckCollision(2, endX, endY, endZ);
	}
	// check 4th quadrant (4th linked list)
	if (((endX >= m_worldSizeX / 2.0) || (endX >= m_worldSizeX / 2.0)) && 
	((endZ >= m_worldSizeZ / 2.0) || (endZ >= m_worldSizeZ / 2.0)))
	{
		Collision = CheckCollision(3, endX, endY, endZ);
	}
	return Collision;
}

//--------------------------------------------------------------------------------------
// Called from above function to check if collsion occurred.
//--------------------------------------------------------------------------------------
bool Collision::CheckCollision(int index, GLdouble endX, GLdouble endY, GLdouble endZ)
{
	bool CollisionFound = false;
	for (int count = 0; count < m_listSize[index]; count++)
	{
		if (((endX < m_list[index].GetMaxX(count)) && (endX > m_list[index].GetMinX(count))) &&
			((endZ < m_list[index].GetMaxZ(count)) && (endZ > m_list[index].GetMinZ(count))))
		{
			CollisionFound = true;
		}
	}
	return CollisionFound;
}
//--------------------------------------------------------------------------------------
