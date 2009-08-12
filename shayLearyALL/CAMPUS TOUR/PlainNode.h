//  PlainNode.h
//  Header file for the PlainNode class
//  Stores details for each plain.  Each node is stored in each link of the Plain Linked List

//  Shay Leary, April 2005
//--------------------------------------------------------------------------------------

#ifndef PLAINNODE_H
#define PLAINNODE_H

//--------------------------------------------------------------------------------------

#include <iostream>
#include <math.h>
#include <gl/glut.h>

class PlainNode
{

public:
	PlainNode () {Clear();}
    virtual ~PlainNode () {Clear();}

	//----------------------------------------------------------------------------------

	void  Clear   ();

	//----------------------------------------------------------------------------------
	//  Get Methods
	//----------------------------------------------------------------------------------
	GLdouble GetType () {return m_type;}
	GLdouble GetXstart () {return m_xPlainStart;}
	GLdouble GetXend () {return m_xPlainEnd;}
	GLdouble GetYstart () {return m_yPlainStart;}
	GLdouble GetYend () {return m_yPlainEnd;}
	GLdouble GetZstart () {return m_zPlainStart;}
	GLdouble GetZend () {return m_zPlainEnd;}

    // Return the address of the link to the next node in the list
    PlainNode *GetNext () const {return m_next;}

	//----------------------------------------------------------------------------------
	//  Set Methods
	//----------------------------------------------------------------------------------
	void SetData(const int tempType,
				 const GLdouble tempXs, const GLdouble tempXe,
				 const GLdouble tempYs, const GLdouble tempYe,
				 const GLdouble tempZs, const GLdouble tempZe);

    // Set the address of the link to the next node in the list
    void SetNext (PlainNode *next) {m_next = next;}

private:
    // The address of the next node in the list
    PlainNode *m_next;

	// Stores type of plain:
	// (0: flat plane)
	// (1: incline from z to y)
	// (2: incline from x to y)
	GLdouble m_type;
	

	// stores start and end co-ordinates of plane on x, y and z axis
	GLdouble m_xPlainStart, m_xPlainEnd;
	GLdouble m_yPlainStart, m_yPlainEnd;
	GLdouble m_zPlainStart, m_zPlainEnd;

	//----------------------------------------------------------------------------------

    // Privatised copy constructor and assignment operator
    PlainNode (const PlainNode &newNode) {};
    PlainNode &operator = (const PlainNode &newNode) {};
};

#endif

//--------------------------------------------------------------------------------------