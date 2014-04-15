#pragma once

#include "SlavsServerAPI.h"
#include "Types.h"

#include <Game/IGameContext.h>
#include <Game/TimeController.h>

#include <boost/noncopyable.hpp>

//////////////////////////////////////////////////////////////////////////

class Vector2D;
class IController;

//////////////////////////////////////////////////////////////////////////

namespace Slavs
  {

  /*
  Context of game that keeps track of 
    - objects that present on scene.
    - controllers
    - global data that can be influenced with outside
  */
  class SLAVS_SERVER_EXPORT GameContext : public IGameContext<GameObject>
                                        , public TickListener
                                        , boost::noncopyable
    {
    private:
      /// objects must have unique identifiers in borders of one context
      long m_next_object_id;

      std::set<GameObject*> m_dead_pool;

    public:
      GameContext();
      ~GameContext();

      virtual void TickPerformed() override;

      __declspec(deprecated)
      void Init(){}
      __declspec(deprecated)
      void ReleaseContext(){}

      //returns the object that was created or NULL
      //@param i_type - what type context should create
      //@param owner - if this parameter is not nullptr then controller becomes the owner of the object
      //@param qm	   - if qm != QM_ALL, then query mask of the object becomes qm
      TGameObject					AddObject(int i_type, const Vector2D& i_position, IController* ip_owner = nullptr, int i_selection_mask = 0);
      /// add object to the dead pool
      /// dead pool is clearing in TickPerformed method before updating all objects
      void								RemoveObject(TGameObject ih_object);

      
    };

  } // Slavs