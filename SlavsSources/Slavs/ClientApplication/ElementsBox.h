#ifndef __ELEMENTSBOX_H__
#define __ELEMENTSBOX_H__

namespace UI
  {

  class ElementsPanel;

  class ElementsBox
    {
    public:
      struct Command
        {
        int m_id;
        std::string m_name;

        Command(int i_id, const std::string& i_name)
          : m_id(i_id)
          , m_name(i_name)
          {}
        };

    private:
      ElementsPanel&        m_owner_panel;
      std::string           m_name;
      int                   m_id;

      std::vector<Command>  m_commands;

      bool                  m_is_visible;
      bool                  m_is_active;

    public:
      ElementsBox(ElementsPanel& i_owner_panel, const std::string& i_name, int i_id);
      virtual ~ElementsBox();

      std::string     GetName() const;
      int             GetId() const;
      ElementsPanel&  GetOwner() const;

      void            AddCommand(const std::string& i_display_name, int i_command_id);
      void            RemoveCommand(int i_command_id);

      int             GetCommandId(const std::string& i_display_name) const;
      std::string     GetCommandName(int i_command_id) const;

      bool            IsVisible() const;
      bool            IsActive() const;

      void            Activate();
      void            Deactivate();

      bool            CanShiftRght() const;
      bool            CanShiftLeft() const;

      bool            ShiftRight();
      bool            ShiftLeft();

      const std::vector<Command>& GetCurrentShownCommands() const;
    };

  inline std::string ElementsBox::GetName() const
    {
    return m_name;
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
