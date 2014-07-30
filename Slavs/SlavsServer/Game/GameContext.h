#pragma once

#include "SlavsServerAPI.h"
#include "Types.h"

#include <Common/Game/IGameContext.h>
#include <Common/Game/TimeController.h>

#include <boost/noncopyable.hpp>

//////////////////////////////////////////////////////////////////////////

class Vector2D;
class IController;
class GlobalEconomics;

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
    public:
      // class for modifying environment for
      //  1. Environment          - for creation Trees, Plants, Mountings, etc.
      //  2. All controllers      - global changes
      //  3. Specific controller  - change environment for one controller (e.x. change price for object creation)
      class EnvironmentModifier
        {

        };

      struct ControllerInformation : boost::noncopyable
        {
        std::unique_ptr<IController>                    mp_controller;
        /// resources that will be needed for object creation of object
        std::map<int/*object type*/, ResourcesCountSet>  m_needed_resources;

        ControllerInformation();
        ControllerInformation(ControllerInformation&& i_information);
        bool operator == (const ControllerInformation& i_other);
        };

    private:
      /// objects must have unique identifiers in borders of one context
      long m_next_object_id;

    // internal data
    private:
      std::set<GameObject*>               m_dead_pool;
      std::vector<ControllerInformation>  m_controllers;
      std::unique_ptr<GlobalEconomics>    mp_global_economics;

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
      //@param i_controllers_mask - if this parameter is not 0 then controller with such mask becomes the owner of the object
      //@param qm	   - if qm != QM_ALL, then query mask of the object becomes qm
      TGameObject					AddObject(int i_type, const Vector2D& i_position, int i_controllers_mask = 0, int i_selection_mask = 0);
      /// add object to the dead pool
      /// dead pool is clearing in TickPerformed method before updating all objects
      void								RemoveObject(TGameObject ih_object);

      void                RegisterController(std::unique_ptr<IController> ip_controller);
      void                RegisterResources (int i_type, const ResourcesCountSet& i_resources);

      /// @param i_type - type of object for which resources needed
      /// @param i_controller_mask - because price for different controllers can vary due to
      ///                             development of technologies or smth else
      ///                             resources for each controller can be different
      const ResourcesCountSet&    GetResourcesFor (int i_type, int i_controller_mask);

      GlobalEconomics&            GetGlobalEconomics() const;
    };

  } // Slavs