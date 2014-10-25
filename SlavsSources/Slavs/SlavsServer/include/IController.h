#pragma once

#include "SlavsServerAPI.h"

//server
#include "SResourceManager.h"
//common
#include <Game/Enumerations.h>
#include <Game/TimeController.h>

#include <map>

namespace net
  {
  class Connection;
  }

namespace Slavs
  {
  class GameContext;
  }

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
  private:
    //unique mask for each controller
    int					                m_iMask;
    Slavs::GameContext&         m_game_context;

  protected:
    std::unique_ptr<Goverment>  mp_goverment;

  protected:
    bool                CheckResources(int i_object_type);
    bool                CreateObject(int i_object_type);

    void				        ControllerStaff();

    SGameContext*		m_pContext;
    SResourceManager*	m_pResourceManager;
    //needed size for current serialization. Depends on ResourceManager NeededSize. Subclasses may add another dependencies
    mutable int			m_iNeededSize;

  public:
	  SLAVS_SERVER_EXPORT IController(int iMask, Slavs::GameContext& i_context);
	  virtual SLAVS_SERVER_EXPORT ~IController();

	  virtual void		    Init() = 0;
    virtual void	      HoldPacket(net::Connection* connection, unsigned char* packet, int bytes_read){}
    virtual void        NotifyObjectChanges(const std::map<long, GameObjectState*>& ip_object_states) {};

	  void				        SetContext(SGameContext *context);

    Slavs::GameContext&  GetGameContext();
    int					         GetMask()	const;
    Goverment&           GetGoverment();

	  SGameContext*		    GetContext()const { return m_pContext; }
	  
	  SResourceManager*	  GetResourceMgr() const { return m_pResourceManager; }
    

	  int					        Serialize(char* buf_end, int size) const;
	  int					        NeededSize() const;
  };

//////////////////////////////////////////////////////////////////////////

inline Slavs::GameContext& IController::GetGameContext()
  {
  return m_game_context;
  }

inline int IController::GetMask() const
  {
  return m_iMask;
  }

inline Goverment& IController::GetGoverment()
  {
  return *mp_goverment;
  }