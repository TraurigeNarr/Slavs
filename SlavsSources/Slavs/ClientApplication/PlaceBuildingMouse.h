#ifndef __PLACEBUILDINGMOUSE_H__
#define __PLACEBUILDINGMOUSE_H__

#include "IMouse.h"

#include "InputSubscriber.h"

namespace UI
  {

  class PlaceBuildingMouse : public GameCore::UI::IMouse
                           , public InputSubscriber
    {
    private:
      bool                  m_was_cursor_visible;

      Ogre::SceneNode*	    mp_node;

      SDK::Task*            mp_current_task;

    private:
      virtual void          ActivateImpl(SDK::Task* ip_task) override;
      virtual void          DeactivateImpl() override;

      void                  ConfirmBuildingCreation();

    public:
      PlaceBuildingMouse();
      virtual ~PlaceBuildingMouse();

      virtual std::string   GetName() const override { return "Game.PlaceBuildingMouse"; }

      virtual void          Update(long i_elapsed_time) override;

    public:
      virtual bool KeyReleased(const OIS::KeyEvent &keyEventRef) override;

      virtual bool MouseMoved(const OIS::MouseEvent &evt) override;
      virtual bool MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id) override;
      virtual bool MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id) override;
    };

  } // UI

#endif