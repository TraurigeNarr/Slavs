#pragma once

#include <string>
#include <CEGUI\Window.h>

enum MessageBoxState
{
  MBS_OK = 1 << 15, 
  MBS_Cancel = 1 << 14, 
  MBS_Ignore = 1 << 13
};


class ClientMessageBox
{
public:
  ClientMessageBox(const std::string& i_text, int i_state);
  ~ClientMessageBox();

  bool	ButtonHandler(const CEGUI::EventArgs &e);
private:
  void BuildGUI(const std::string& i_text, int i_state);

  CEGUI::Window* m_root_window;
};