#include "stdafx.h"

#include "LoadStateMessageProvider.h"

#include "LoadingState.h"
#include "LoadingStages.h"

#include <Common/Patterns/StateMachine.h>


namespace ClientStates
  {

  LoadStateMessageProvider::LoadStateMessageProvider(LoadingState& i_state)
    : m_state(i_state)
    , m_current_stage()
    , m_current_progress(0)
    {

    }

  LoadStateMessageProvider::~LoadStateMessageProvider()
    {

    }

  void LoadStateMessageProvider::Invalidate()
    {
    __super::Invalidate();
    
    // cache data
    if (m_state.GetStateMachine().IsInState<LoadingStages::ConnectionState>())
      {
      m_current_stage     = "Connection state";
      m_general_progress  = 0.f;
      m_current_progress  = 0.f;
      }
    else if (m_state.GetStateMachine().IsInState<LoadingStages::GettingData>())
      {
      auto p_current_state = std::static_pointer_cast<LoadingStages::GettingData>(m_state.GetStateMachine().GetCurrentState());

      if (m_general_progress < 0.33)
        {
        m_current_stage    = "Getting data";
        m_general_progress = 0.33f;
        }
      
      float laoded_progress = p_current_state->GetTotalContentNumber() != 0 
                                                ? static_cast<float>(p_current_state->GetLoadedContentNumber()) / static_cast<float>(p_current_state->GetTotalContentNumber()) 
                                                : 0;
      
      auto current_data_type = p_current_state->GetStage();
      float starting_value = .0f;
      switch (current_data_type)
        {
        case LoadingStages::GettingData::CurrentData::CD_DEFINITIONS:
          starting_value = .0f;
          break;
        case LoadingStages::GettingData::CurrentData::CD_OBJECTS:
          starting_value = .5f;
        }

      m_current_progress = starting_value + .5f * laoded_progress;

      m_general_progress = 0.33f + m_current_progress/3;
      }
    else if (m_state.GetStateMachine().IsInState<LoadingStages::ResultsState>())
      {
      m_current_stage = "Waiting for others";
      m_general_progress  = 1.f;
      m_current_progress  = 1.f;
      }
    }

  std::string LoadStateMessageProvider::GetCurrentStage() const
    {
    return m_current_stage;
    }

  float LoadStateMessageProvider::GetCurrentProgress() const
    {
    return m_current_progress;
    }

  float LoadStateMessageProvider::GetGeneralProgress() const
    {
    return m_general_progress;
    }

  } // ClientStates