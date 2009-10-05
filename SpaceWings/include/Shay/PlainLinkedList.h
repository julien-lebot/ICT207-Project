//  PlainLinkedList.h
//  Header file for the PlainLinkedList class
//  Linked List used to store nodes (PlainNode) which contain the co-ordinates of the 
//  of each plain used in the program.
//
//	The program will split the world into four quadrants and creates a linked list to
//  store the bounding box details for each
//
//  Author:  Shay Leary
//  April 2005
//--------------------------------------------------------------------------------------

#ifndef PLAINLINKED_LIST_H
#define PLAINLINKED_LIST_H

//--------------------------------------------------------------------------------------

#include "PlainNode.h"

//--------------------------------------------------------------------------------------

class PlainLinkedList
{
public:
	// constructor creates pointer to first node
	PlainLinkedList() {m_first = new PlainNode;}

	virtual ~PlainLinkedList() {Clear();}

	//----------------------------------------------------------------------------------

	// clears linked list and frees memory
	void Clear();

	// add a node to the start of the linked list
	bool AddToStart (const int tempType,
				     const GLdouble tempXs, const GLdouble tempXe,
				     const GLdouble tempYs, const GLdouble tempYe,
				     const GLdouble tempZs, const GLdouble tempZe);
	
	// set the values of the node data
	void SetData(const int &ptrCount, const int tempType,
				 const GLdouble tempXs, const GLdouble tempXe,
				 const GLdouble tempYs, const GLdouble tempYe,
				 const GLdouble tempZs, const GLdouble tempZe);

	//----------------------------------------------------------------------------------
	//  Get Methods
	//----------------------------------------------------------------------------------
	GLdouble GetType (int ptrCount);
	GLdouble GetXstart (int ptrCount);
	GLdouble GetXend (int ptrCount);
	GLdouble GetYstart (int ptrCount);
	GLdouble GetYend (int ptrCount);
	GLdouble GetZstart (int ptrCount);
	GLdouble GetZend (int ptrCount);

	// Return size of list
	int GetListSize ();
	
	// Return the address of the link to the next node in the list
    PlainNode *GetNext () const {return m_first->GetNext();}
	// Return the address of the link to the first node in the list
	PlainNode *GetFirst() const {return m_first;}


//--------------------------------------------------------------------------------------

private:
	// pointer to first node in list
	PlainNode *m_first;

	// used to clear memory
	void Delete (PlainNode *before);

	// Privatised copy constructor and assignment operator
	PlainLinkedList (const PlainLinkedList &array) {};
	PlainLinkedList &operator = (const PlainLinkedList &array) {};
};

#endif

//--------------------------------------------------------------------------------------