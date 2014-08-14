#include "stdafx.h"
#include "ApplicationStateBase.h"

#include "Application.h"
#include "InputManager.h"

namespace ClientStates
  {

  ApplicationStateBase::ApplicationStateBase(Application& i_application)
    : m_application(i_application)
    {
    m_application.GetInputManager().AddSubscriber(this);
    }

  ApplicationStateBase::~ApplicationStateBase()
    {
    m_application.GetInputManager().RemoveSubscriber(this);
    }

  } // ClientStates