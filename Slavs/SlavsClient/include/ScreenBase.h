#ifndef ScreenBase_h
#define ScreenBase_h

#include "ClientEnums.h"
#include <CEGUI\CEGUI.h>

class ClientMessageBox;

class ScreenBase
{
public:
	ScreenBase();
	virtual ~ScreenBase();

	virtual void	Init() = 0;
	virtual void	Update(float elapsedTime) = 0;

	//id determines to what type transform data
	virtual bool	HandleData(CommandToUI id, void* data) { return false; }

	virtual bool	ButtonHandler(const CEGUI::EventArgs &e);

	static void		ClearWindow(CEGUI::Window* wnd);

  CEGUI::Window* GetRootWindow() const { return m_pRootCEGUIWindow; }

  void           ShowMessageBox(const std::string& i_text, int i_state);
  void           HideMessageBox();
protected:
	CEGUI::Window*	m_pRootCEGUIWindow;

  ClientMessageBox*     m_message_box;
};

#endif