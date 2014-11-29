#ifndef __COMMANDSPANEL_H__
#define __COMMANDSPANEL_H__

#include "ElementsPanel.h"
#include "CeguiScreenBase.h"

namespace UI
  {

  /*
  ElementsPanel controls all command boxes that will be created
  Its position is set in cegui layout
  Command boxes created in run time depends on plugins that were defined on server side
  Maybe some default commands should be defined
  */
  class CommandsPanel : public CeguiScreenBase
    {
    private:
      ElementsPanel m_panel;

    private:
      virtual void Initialize() override;

      void RedrawList();
      void RedrawCommands();
      void RedrawAll();

      void ShiftListLeft();
      void ShiftListRight();
      void ShiftCommandsLeft();
      void ShiftCommandsRight();

      bool ShiftButtonPressed(const CEGUI::EventArgs& i_arguments);
      bool ListButtonPressed(const CEGUI::EventArgs& i_arguments);
    public:
      CommandsPanel(ScreenManager& i_screen_manager);
      virtual ~CommandsPanel();

      virtual void Update(long i_elapsed_time) override;
    };

  } // UI

#endif