#pragma once

#include <OISKeyboard.h>
#include <OISMouse.h>

namespace UI
  {
  struct ControlData;
  }

class InputSubscriber
{
public:
  virtual ~InputSubscriber(){}
	virtual bool KeyPressed(const OIS::KeyEvent &keyEventRef) { return false; }
	virtual bool KeyReleased(const OIS::KeyEvent &keyEventRef) { return false; }

	virtual bool MouseMoved(const OIS::MouseEvent &evt) { return false; }
	virtual bool MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id) { return false; }
	virtual bool MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id) { return false; }

  virtual bool UiEvent (const UI::ControlData& i_ui_event) {return false; }
};

typedef std::shared_ptr<InputSubscriber> InputSubscriberPtr;