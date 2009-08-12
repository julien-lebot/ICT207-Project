//  PlainLinkedList.cpp
//
//  Implementation file for PlainLinkedList Class
//  Defines all the methods declared, but not defined, in PlainLinkedList.h
//
//  Shay Leary, April 2005
//--------------------------------------------------------------------------------------

#include "PlainLinkedList.h"

//--------------------------------------------------------------------------------------

void PlainLinkedList::Clear()
{
	PlainNode *ptr = m_first;

	while (ptr->GetNext() != NULL)
	
	// clear memory
	Delete(ptr);
}

//--------------------------------------------------------------------------------------

void PlainLinkedList::Delete(PlainNode *before)
{
	PlainNode *temp = before->GetNext();

	before->SetNext(temp->GetNext());

	delete temp;
}

//--------------------------------------------------------------------------------------


bool PlainLinkedList::AddToStart (const int tempType,
							      const GLdouble tempXs, const GLdouble tempXe,
								  const GLdouble tempYs, const GLdouble tempYe,
				                  const GLdouble tempZs, const GLdouble tempZe)
{
	PlainNode *newNode;

	try
	{
		newNode = new PlainNode;
	}
	catch(...)
	{
		return false;
	}
	
	// add the value to the node
	newNode->SetData(tempType, tempXs, tempXe, tempYs, tempYe, tempZs, tempZe);
	// set the address of the net node
	newNode->SetNext(m_first->GetNext());
	// reset the address of the first node
	m_first->SetNext(newNode);

	return true;
}

//--------------------------------------------------------------------------------------

GLdouble PlainLinkedList::GetType (int ptrCount)
{
	PlainNode *ptr = (m_first);
	for (int count = 0; count < ptrCount; count++)
	{
		ptr = ptr->GetNext();
	}

	if (ptr->GetNext() != NULL)
		return ptr->GetNext()->GetType();	
	else 
		return NULL;
}

//--------------------------------------------------------------------------------------

GLdouble PlainLinkedList::GetXstart (int ptrCount)
{
	PlainNode *ptr = (m_first);
	for (int count = 0; count < ptrCount; count++)
	{
		ptr = ptr->GetNext();
	}

	if (ptr->GetNext() != NULL)
		return ptr->GetNext()->GetXstart();	
	else 
		return NULL;
}

//--------------------------------------------------------------------------------------

GLdouble PlainLinkedList::GetXend (int ptrCount)
{
	PlainNode *ptr = (m_first);
	for (int count = 0; count < ptrCount; count++)
	{
		ptr = ptr->GetNext();
	}

	if (ptr->GetNext() != NULL)
		return ptr->GetNext()->GetXend();	
	else 
		return NULL;
}

//--------------------------------------------------------------------------------------

GLdouble PlainLinkedList::GetYstart (int ptrCount)
{
	PlainNode *ptr = (m_first);
	for (int count = 0; count < ptrCount; count++)
	{
		ptr = ptr->GetNext();
	}

	if (ptr->GetNext() != NULL)
		return ptr->GetNext()->GetYstart();	
	else 
		return NULL;
}

//--------------------------------------------------------------------------------------

GLdouble PlainLinkedList::GetYend (int ptrCount)
{
	PlainNode *ptr = (m_first);
	for (int count = 0; count < ptrCount; count++)
	{
		ptr = ptr->GetNext();
	}

	if (ptr->GetNext() != NULL)
		return ptr->GetNext()->GetYend();	
	else 
		return NULL;
}

//--------------------------------------------------------------------------------------

GLdouble PlainLinkedList::GetZstart (int ptrCount)
{
	PlainNode *ptr = (m_first);
	for (int count = 0; count < ptrCount; count++)
	{
		ptr = ptr->GetNext();
	}

	if (ptr->GetNext() != NULL)
		return ptr->GetNext()->GetZstart();	
	else 
		return NULL;
}

//--------------------------------------------------------------------------------------

GLdouble PlainLinkedList::GetZend (int ptrCount)
{
	PlainNode *ptr = (m_first);
	for (int count = 0; count < ptrCount; count++)
	{
		ptr = ptr->GetNext();
	}

	if (ptr->GetNext() != NULL)
		return ptr->GetNext()->GetZend();	
	else 
		return NULL;
}


//--------------------------------------------------------------------------------------

void PlainLinkedList::SetData (const int &ptrCount, const int tempType,
							   const GLdouble tempXs, const GLdouble tempXe,
				               const GLdouble tempYs, const GLdouble tempYe,
				               const GLdouble tempZs, const GLdouble tempZe)
{
	PlainNode *ptr = (m_first);

	for (int count = 0; count < ptrCount; count++)
	{
		ptr = ptr->GetNext();
	}
	ptr->GetNext()->SetData(tempType, tempXs, tempXe, tempYs, tempYe, tempZs, tempZe);
}

//--------------------------------------------------------------------------------------

int PlainLinkedList::GetListSize()
{
	int tmpSize = 0;
	// count size of list
	for (PlainNode *ptr = (m_first); ptr->GetNext() != NULL; ptr = ptr->GetNext())
	{
		tmpSize++;
	}
	return tmpSize;
}

//--------------------------------------------------------------------------------------