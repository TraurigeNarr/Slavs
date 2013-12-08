#ifndef HelpBox_h
#define HelpBox_h

#include <CEGUI\CEGUI.h>
#include <CEGUI\widgets\MultiLineEditbox.h>

class HelpBox
{
public:
	HelpBox(CEGUI::Window* i_root_window);
	~HelpBox();

	void Init();

  bool ButtonHandler(const CEGUI::EventArgs &e);
protected:
  void Activate();
  void Deactivate();
  CEGUI::Window* m_modal_window;
  CEGUI::MultiLineEditbox* m_edit_box;
  CEGUI::Window* m_ok_btn;
  CEGUI::Window* m_root_window;
};

#endif