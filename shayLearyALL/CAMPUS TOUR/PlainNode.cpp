//  PlainNode.cpp
//
//  Implementation file for PlainNode Class
//  Defines all the methods declared, but not defined, in PlainNode.h
//
//  Shay Leary, April 2005
//--------------------------------------------------------------------------------------

#include "PlainNode.h"

//----------------------------------------------------------------

void PlainNode::Clear()
{
	m_next = NULL;
}

//----------------------------------------------------------------

void PlainNode::SetData(const int tempType,
						const GLdouble tempXs, const GLdouble tempXe,
						const GLdouble tempYs, const GLdouble tempYe,
						const GLdouble tempZs, const GLdouble tempZe)
{
	m_type = tempType;
	m_xPlainStart = tempXs;
	m_xPlainEnd = tempXe;
	m_yPlainStart = tempYs;
	m_yPlainEnd = tempYe;
	m_zPlainStart = tempZs;
	m_zPlainEnd = tempZe;
}

//----------------------------------------------------------------