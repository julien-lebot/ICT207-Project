//  AABBNode.cpp
//
//  Implementation file for AABBNode Class
//  Defines all the methods declared, but not defined, in AABBNode.h
//
//  Shay Leary, March 2005
//--------------------------------------------------------------------------------------

#include "AABBNode.h"

//----------------------------------------------------------------

void AABBNode::Clear()
{
	m_next = NULL;
}

//----------------------------------------------------------------


void AABBNode::SetData(const GLdouble maxX, const GLdouble minX,
					   const GLdouble maxY, const GLdouble minY,
				       const GLdouble maxZ, const GLdouble minZ)
{
	m_BBox.max.x = maxX;
	m_BBox.min.x = minX;
	m_BBox.max.y = maxY;
	m_BBox.min.y = minY;
	m_BBox.max.z = maxZ;
	m_BBox.min.z = minZ;
}

//----------------------------------------------------------------
