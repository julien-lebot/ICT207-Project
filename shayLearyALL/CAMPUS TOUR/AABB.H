//  AABH.h
//  Header file for the AABB class
//  Dynamic Array used to store the co-ordinates of the bounding boxes used for collsion 
//  detection.  The array is only used initially before being copied into AABB Linked Lists.
//  The array is then terminated.
//  
//  I created this class originally before using linked lists to store the BB info. But decided
//  to keep it has the stored data can be easily copied across into different linked lists
//  in any prefered order. (i.e. the data is copied into across into linked lists for different
//  quadrants)
//
//  Shay Leary, March 2005
//--------------------------------------------------------------------------------------

#ifndef AABB_H
#define AABB_H

//--------------------------------------------------------------------------------------

#include <iostream>
#include <math.h>
#include <gl/glut.h>

//--------------------------------------------------------------------------------------

class AABB
{
private:
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
	// dynamic array to store info
	BoundingBox *m_BBox;
	// number of BB created (i.e. array size)
	int m_NoOfBoxes;
	// deletes arrays and clears memory
	void ClearBB (BoundingBox* &tempArray);

	//----------------------------------------------------------------------------------

    // Privatised copy constructor and assignment operator
    AABB (const AABB &aabb) {};
    AABB &operator = (const AABB &aabb) {};

public:
	AABB () {}
    virtual ~AABB () {ClearBB(m_BBox);}
	
	//----------------------------------------------------------------------------------
	//  Set Methods
	//----------------------------------------------------------------------------------
	void SetMaxX(const int & tempIndex, const GLdouble &tempX) {m_BBox[tempIndex].max.x = tempX;}
	void SetMinX(const int & tempIndex, const GLdouble &tempX) {m_BBox[tempIndex].min.x = tempX;}
	void SetMaxY(const int & tempIndex, const GLdouble &tempY) {m_BBox[tempIndex].max.y = tempY;}
	void SetMinY(const int & tempIndex, const GLdouble &tempY) {m_BBox[tempIndex].min.y = tempY;}
	void SetMaxZ(const int & tempIndex, const GLdouble &tempZ) {m_BBox[tempIndex].max.z = tempZ;}
	void SetMinZ(const int & tempIndex, const GLdouble &tempZ) {m_BBox[tempIndex].min.z = tempZ;}
	void SetNoBoundingBoxes(const int & tempSize);

	//----------------------------------------------------------------------------------
	//  Get Methods
	//----------------------------------------------------------------------------------
	GLdouble GetMaxX (const int & tempIndex) {return m_BBox[tempIndex].max.x;}
	GLdouble GetMinX (const int & tempIndex) {return m_BBox[tempIndex].min.x;}
	GLdouble GetMaxY (const int & tempIndex) {return m_BBox[tempIndex].max.y;}
	GLdouble GetMinY (const int & tempIndex) {return m_BBox[tempIndex].min.y;}
	GLdouble GetMaxZ (const int & tempIndex) {return m_BBox[tempIndex].max.z;}
	GLdouble GetMinZ (const int & tempIndex) {return m_BBox[tempIndex].min.z;}
	int GetNoBoundingBoxes () {return m_NoOfBoxes;}
};

#endif
//--------------------------------------------------------------------------------------