#pragma once

#include "SlavsServerAPI.h"
#include "Types.h"

#include "GameWorld.h"

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
  class GameContext : public IGameContext<GameObject>
                                        , public TickListener
                                        , boost::noncopyable
    {
    public:

			class Modifier
				{
				int m_id;
				// what this modifier influence on
				//		For controllers that will ask for modifiers of certain type
				int m_flags;
				};
			typedef std::unique_ptr<Modifier> ModifierPtr;

      // class for modifying environment for
      //  1. Environment          - for creation Trees, Plants, Mountings, etc.
      //  2. All controllers      - global changes
      //  3. Specific controller  - change environment for one controller (e.x. change price for object creation)
			//		will be held in ControllerInformation with its modifiers
      class EnvironmentModifier : public Modifier
        {

        };

      struct ControllerInformation : boost::noncopyable
        {
        std::unique_ptr<IController>											mp_controller;
        /// resources that will be needed for object creation of object
        std::map<int/*object type*/, ResourcesCountSet>		m_needed_resources;

				std::vector<ModifierPtr>															m_modifier;

        SLAVS_SERVER_EXPORT ControllerInformation();
        SLAVS_SERVER_EXPORT ControllerInformation(ControllerInformation&& i_information);
        SLAVS_SERVER_EXPORT bool operator == (const ControllerInformation& i_other);

				void AddModifier(ModifierPtr ip_modifier){}
        };

    private:
      /// objects must have unique identifiers in borders of one context
      long m_next_object_id;

    // internal data
    private:
      std::set<GameObject*>               m_dead_pool;
      std::vector<ControllerInformation>  m_controllers;
      std::unique_ptr<GlobalEconomics>    mp_global_economics;

			GameWorld									m_game_world;

    public:
      SLAVS_SERVER_EXPORT GameContext();
      SLAVS_SERVER_EXPORT ~GameContext();


      SLAVS_SERVER_EXPORT virtual void TickPerformed() override;
      SLAVS_SERVER_EXPORT virtual void  ReleaseContext() override;

      __declspec(deprecated)
      void Init(){}

      //returns the object that was created or NULL
      //@param i_type - what type context should create
      //@param i_controllers_mask - if this parameter is not 0 then controller with such mask becomes the owner of the object
      //@param qm	   - if qm != QM_ALL, then query mask of the object becomes qm
			SLAVS_SERVER_EXPORT TGameObject					AddObject(int i_type, const Vector2D& i_position, int i_controllers_mask = 0, int i_selection_mask = 0);
      /// add object to the dead pool
      /// dead pool is clearing in TickPerformed method before updating all objects
			SLAVS_SERVER_EXPORT void								RemoveObject(TGameObject ih_object);

      SLAVS_SERVER_EXPORT void                RegisterController(std::unique_ptr<IController> ip_controller);
      SLAVS_SERVER_EXPORT void                RegisterResources (int i_type, const ResourcesCountSet& i_resources);

      /// @param i_type - type of object for which resources needed
      /// @param i_controller_mask - because price for different controllers can vary due to
      ///                             development of technologies or smth else
      ///                             resources for each controller can be different
			SLAVS_SERVER_EXPORT const ResourcesCountSet&    GetResourcesFor(int i_type, int i_controller_mask);

      GlobalEconomics&            GetGlobalEconomics() const
				{
				return *mp_global_economics;
				}

      const std::vector<ControllerInformation>& GetControllers() const
        {
        return m_controllers;
        }

			GameWorld& GetGameWorld()
				{
				return m_game_world;
				}
    };

  } // Slavs