#ifndef InvertedAABBox2D_h
#define InvertedAABBox2D_h

#include "Vector2D.h"

class InvertedAABBox2D
{
private:

	Vector2D  m_vTopLeft;
	Vector2D  m_vBottomRight;

	Vector2D  m_vCenter;

public:

	InvertedAABBox2D(Vector2D tl,
		Vector2D br):m_vTopLeft(tl),
		m_vBottomRight(br),
		m_vCenter((tl+br)/2.0)
	{}

	//returns true if the bbox described by other intersects with this one
	bool isOverlappedWith(const InvertedAABBox2D& other)const
	{
		return !((other.Top() > this->Bottom()) ||
			(other.Bottom() < this->Top()) ||
			(other.Left() > this->Right()) ||
			(other.Right() < this->Left()));
	}


	Vector2D TopLeft()const{return m_vTopLeft;}
	Vector2D BottomRight()const{return m_vBottomRight;}

	float    Top()const{return m_vTopLeft.y;}
	float    Left()const{return m_vTopLeft.x;}
	float    Bottom()const{return m_vBottomRight.y;}
	float    Right()const{return m_vBottomRight.x;}
	Vector2D Center()const{return m_vCenter;}
};

#endif