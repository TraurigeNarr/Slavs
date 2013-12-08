#ifndef IController_h
#define IController_h
//server
#include "SResourceManager.h"
//net
#include "Net.h"
//common
#include <Game/Enumerations.h>
#include <Game/TimeController.h>

#include <map>

class SGameContext;
class Goverment;

/*
-------------------------------------------------------------------------------------------
This class is the parent class for controllers.
GameContext keeps map of this classes.
-------------------------------------------------------------------------------------------
*/

class IController : public TickListener
{
public:
	IController(int iMask, SGameContext* context = 0);
	virtual ~IController();

	virtual void		    Init() = 0;
  virtual void	      HoldPacket(net::Connection* connection, unsigned char* packet, int bytes_read){}
  virtual void        NotifyObjectChanges(const std::map<long, GameObjectState*>& ip_object_states) {};

	void				        SetContext(SGameContext *context);

	SGameContext*		    GetContext()const { return m_pContext; }
	int					        GetMask()	const { return m_iMask; }
	SResourceManager*	  GetResourceMgr() const { return m_pResourceManager; }
  Goverment&          GetGoverment();

	int					        Serialize(char* buf_end, int size) const;
	int					        NeededSize() const;
protected:
	bool				        ChechResources(ObjectType oType);
	void				        ControllerStaff();

	SGameContext*		m_pContext;
	SResourceManager*	m_pResourceManager;
	//needed size for current serialization. Depends on ResourceManager NeededSize. Subclasses may add another dependencies
	mutable int			m_iNeededSize;
private:
	//unique mask for each controller
	int					m_iMask;

  std::unique_ptr<Goverment> mp_goverment;
};

#endif