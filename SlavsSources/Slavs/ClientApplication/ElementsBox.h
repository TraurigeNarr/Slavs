#ifndef __ELEMENTSBOX_H__
#define __ELEMENTSBOX_H__

#include "CommandInfo.h"

namespace UI
  {

  class ElementsPanel;

  class ElementsBox
    {
    private:
      ElementsPanel&        m_owner_panel;
      int                   m_type_id;
      int                   m_id;

      std::vector<CommandInfo>  m_commands;

      bool                  m_is_visible;
      bool                  m_is_active;

    public:
      ElementsBox(ElementsPanel& i_owner_panel, int i_type_id, int i_id);
      virtual ~ElementsBox();

      int                     GetTypeId() const;
      int                     GetId() const;
      ElementsPanel&          GetOwner() const;

      void                    AddCommand(const CommandInfo& i_info);
      void                    RemoveCommand(int i_command_id);

      int                     GetCommandId(const std::string& i_string_id) const;

      bool                    IsVisible() const;
      bool                    IsActive() const;

      void                    Activate();
      void                    Deactivate();

      bool                    CanShiftRght() const;
      bool                    CanShiftLeft() const;

      bool                    ShiftRight();
      bool                    ShiftLeft();

      const std::vector<CommandInfo>& GetCurrentShownCommands() const;
    };

  inline int ElementsBox::GetTypeId() const
    {
    return m_type_id;
    }

  inline int ElementsBox::GetId() const
    {
    return m_id;
    }

  inline ElementsPanel& ElementsBox::GetOwner() const
    {
    return m_owner_panel;
    }

  inline bool ElementsBox::IsVisible() const
    {
    return m_is_visible;
    }

  inline bool ElementsBox::IsActive() const
    {
    return m_is_active;
    }

  inline void ElementsBox::Activate()
    {
    m_is_active = true;
    }

  inline void ElementsBox::Deactivate()
    {
    m_is_active = false;
    }

  } // UI

#endif
