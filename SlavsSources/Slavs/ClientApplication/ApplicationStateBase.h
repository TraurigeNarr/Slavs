#pragma once

#include "Types.h"
#include "InputSubscriber.h"

#include <Common/Patterns/State.h>

namespace ClientStates
  {

  class ApplicationStateBase : public ApplicationState
                             , public InputSubscriber
    {
    protected:
      Application& m_application;

    public:
      ApplicationStateBase(Application& i_application);
      virtual ~ApplicationStateBase();
    };

  } // ClientStates