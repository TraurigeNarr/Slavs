#include "stdafx.h"
#include "ApplicationStateBase.h"

#include "Application.h"
#include "InputManager.h"

namespace ClientStates
  {

  ApplicationStateBase::ApplicationStateBase(Application& i_application)
    : m_application(i_application)
    {
    
    }

  ApplicationStateBase::~ApplicationStateBase()
    {
    
    }

  } // ClientStates