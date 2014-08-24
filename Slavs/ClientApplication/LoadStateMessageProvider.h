#pragma once

#include "IMessageProvider.h"

namespace ClientStates
  {

  class LoadingState;

  class LoadStateMessageProvider : public UI::IMessageProvider
    {
    private:
      /// Can be:
      ///  1. Connecting to server
      ///  2. Get definitions
      ///  3. Get objects
      ///  4. Load resources
      std::string  m_current_stage;
      ///  (Current Number of loaded objects) / (Total number of objects)
      float        m_current_progress;

      float        m_general_progress;

      LoadingState& m_state;

    public:
      LoadStateMessageProvider(LoadingState& i_state);
      virtual ~LoadStateMessageProvider();

      std::string   GetCurrentStage() const;
      float         GetCurrentProgress() const;
      float         GetGeneralProgress() const;

      virtual void  Invalidate() override;
    };

  } // ClientStates