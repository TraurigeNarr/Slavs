#ifndef IGameObject_h
#define IGameObject_h

#include "..\SlavsCommonAPI.h"

#include "IMovable.h"

#include <string>
#include <vector>

class IComponent;
class GameObjectState;

class /*COMMON_EXPORT*/ IGameObject : public IMovable
{
	long m_lID;
protected:
	int                      m_type;
	std::vector<IComponent*> m_components;
	//if this is true then GOState has only 2 parameters: id and flag (GOS_Destroyed)
	bool m_bDestroyed;
	//one of the Own, Allied, Enemy for client is always with | QM_ALL, on Server -> mask is Environment, Resources or address of client, who created this object
	int m_qmMask;
	//this is true then the state of the object changes, if it is false, then there is no sense to get it state!!!
	//we need to modify flag in GetState  --> sets to false
	mutable bool m_bStateChanged;
	//needed for selecting objects
	bool m_bSelected;

  long  m_ticks_for_components;

	void							              TickForComponents();
public:
									                IGameObject(long ID, int otype, int iMask);
	virtual							            ~IGameObject();
	
  long                            GetElapsedTicksForComponents() const { return m_ticks_for_components; }
	long							              GetID() const { return m_lID; }
	int       						          GetType() const;
	int     						            GetQueryMask() const { return m_qmMask; }
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

	static std::string				      ToString(int oType);
	static int      				        GetGameObjectType(const std::string& oType);
	static int                      GetGameObjectType(int cmdID);

  virtual void     SetPosition(const Vector2D& i_position) override {}
  virtual Vector2D GetPosition() const override { return Vector2D(); }
  virtual Vector2D& AccessPosition() override { throw std::logic_error("Call IGameObject::AccessPosition. Not implemented."); }
};

template<typename T>
T* IGameObject::GetComponent() const
{
	for(unsigned int i = 0; i < m_components.size(); ++i)
	{
		if(typeid(*m_components[i]) == typeid(T))
		{
			return static_cast<T*>(m_components[i]);
		}
	}
	return NULL;
}

#endif