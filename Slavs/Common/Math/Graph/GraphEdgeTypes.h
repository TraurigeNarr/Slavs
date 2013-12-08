#ifndef GraphEdgeTypes_h
#define GraphEdgeTypes_h

#include "NodeTypeEnumerations.h"

class GraphEdge
{
public:
			GraphEdge(int from, int to, float cost)
				: m_fCost(cost), m_iFrom(from),	m_iTo(to)
			{}

			GraphEdge(int from, int  to)
				: m_fCost(1.0),	m_iFrom(from), m_iTo(to)
			{}

			GraphEdge()
				: m_fCost(1.0f), m_iFrom(invalid_node_index), m_iTo(invalid_node_index)
			{}

	virtual	~GraphEdge(){}

	int		From()const{return m_iFrom;}
	void	SetFrom(int NewIndex){m_iFrom = NewIndex;}

	int		To()const{return m_iTo;}
	void	SetTo(int NewIndex){m_iTo = NewIndex;}

	float	Cost()const{return m_fCost;}
	void	SetCost(float NewCost){m_fCost = NewCost;}


	//these two operators are required
	bool	operator==(const GraphEdge& rhs)
	{
		return rhs.m_iFrom == this->m_iFrom &&
			rhs.m_iTo   == this->m_iTo   &&
			rhs.m_fCost == this->m_fCost;
	}

	bool	operator!=(const GraphEdge& rhs)
	{
		return !(*this == rhs);
	}
protected:
	//An edge connects two nodes. Valid node indices are always positive.
	int     m_iFrom;
	int     m_iTo;

	//the cost of traversing the edge
	float	m_fCost;
};

#endif