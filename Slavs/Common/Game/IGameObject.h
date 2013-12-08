#ifndef IGameObject_h
#define IGameObject_h

#include "IComponent.h"
#include "Enumerations.h"

#include <string>
#include <vector>

class IGameObject
{
	long m_lID;
protected:
	ObjectType m_Type;
	std::vector<IComponent*> m_lComponents;
	//if this is true then GOState has only 2 parameters: id and flag (GOS_Destroyed)
	bool m_bDestroyed;
	//one of the Own, Allied, Enemy for client is always with | QM_ALL, on Server -> mask is Environment, Resources or address of client, who created this object
	QueryMask m_qmMask;
	//this is true then the state of the object changes, if it is false, then there is no sense to get it state!!!
	//we need to modify flag in GetState  --> sets to false
	mutable bool m_bStateChanged;
	//needed for selecting objects
	bool m_bSelected;

  long m_ticks_for_components;

	void							              TickForComponents();
public:
									                IGameObject(long ID, ObjectType otype, QueryMask iMask);
	virtual							            ~IGameObject();
	
  long                            GetElapsedTicksForComponents() const { return m_ticks_for_components; }
	const std::vector<IComponent>&	GetComponents() const;
	long							              GetID() const { return m_lID; }
	ObjectType						          GetType() const;
	QueryMask						            GetQueryMask() const { return m_qmMask; }
	bool							              HasChanges() const { return m_bStateChanged; }
	void							              StateChanged() { m_bStateChanged = true; }

	virtual void					          Init() = 0;
	virtual void					          TickPerformed() = 0;

	virtual GameObjectState*		    GetState() const;
	template<typename T>
	T*								              GetComponent() const;

	bool							              Destroyed()const { return m_bDestroyed; }
	bool							              Selected() const { return m_bSelected; }
	virtual void					          Select(){ m_bSelected = true; }
	virtual void					          Deselect() { m_bSelected = false; }

	static std::string				      ToString(ObjectType oType);
	static ObjectType				        GetGameObjectType(const std::string& oType);
	static ObjectType				        GetGameObjectType(CommandID cmdID);
};

template<typename T>
T* IGameObject::GetComponent() const
{
	for(unsigned int i = 0; i < m_lComponents.size(); ++i)
	{
		if(typeid(*m_lComponents[i]) == typeid(T))
		{
			return dynamic_cast<T*>(m_lComponents[i]);
		}
	}
	return NULL;
}

#endif