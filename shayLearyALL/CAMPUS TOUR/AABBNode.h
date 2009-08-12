//  AABBNODE.h
//  Header file for the AABBNODE class
//  Stores details for each bounding box used for collsion detection.  Each node is stored
//  in each link of the AABB Linked List

//  Shay Leary, March 2005
//--------------------------------------------------------------------------------------

#ifndef AABBNODE_H
#define AABBNODE_H

#include <vector>
#include <gl/glut.h>

//--------------------------------------------------------------------------------------

class AABBNode
{
public:
	AABBNode () {Clear();}
    virtual ~AABBNode () {Clear();}

	//----------------------------------------------------------------------------------

	void  Clear   ();

	//----------------------------------------------------------------------------------
	//  Get Methods
	//----------------------------------------------------------------------------------
	GLdouble GetMaxX () {return m_BBox.max.x;}
	GLdouble GetMinX () {return m_BBox.min.x;}
	GLdouble GetMaxY () {return m_BBox.max.y;}
	GLdouble GetMinY () {return m_BBox.min.y;}
	GLdouble GetMaxZ () {return m_BBox.max.z;}
	GLdouble GetMinZ () {return m_BBox.min.z;}

    // Return the address of the link to the next node in the list
    AABBNode *GetNext () const {return m_next;}

	//----------------------------------------------------------------------------------
	//  Set Methods
	//----------------------------------------------------------------------------------
	void SetData(const GLdouble maxX, const GLdouble minX,
				 const GLdouble maxY, const GLdouble minY,
				 const GLdouble maxZ, const GLdouble minZ);

    // Set the address of the link to the next node in the list
    void SetNext (AABBNode *next) {m_next = next;}

private:
    // The address of the next node in the list
    AABBNode *m_next;

	// stores x,y,z co-ordinates
	struct XYZ 
	{ 
		GLdouble x, y, z; 
	}; 
	// stores max and min values of co-ordinates
	struct BoundingBox 
	{ 
		XYZ max; 
		XYZ min; 
	}; 
	// stores above info
	BoundingBox m_BBox;

	//----------------------------------------------------------------------------------

    // Privatised copy constructor and assignment operator
    AABBNode (const AABBNode &newNode) {};
    AABBNode &operator = (const AABBNode &newNode) {};
};

#endif
//--------------------------------------------------------------------------------------