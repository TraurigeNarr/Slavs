#ifndef BuildingsBox_h
#define BuildingsBox_h

#include "ClientEnums.h"

#include <CEGUI\CEGUI.h>
#include <vector>

class HUD;

class BuildingsBox
{
public:
	BuildingsBox(HUD *owner, CEGUI::Window *rootWindow);
	~BuildingsBox();

	void							Show();
	void							Hide();
private:
	bool							ButtonHandler(const CEGUI::EventArgs &e);
	void							ShowBuildingTypes();
	void							ShowBuildingsList();

	HUD*							m_pOwner;
	CEGUI::Window*					m_pRootCEGUIWindow;
	std::vector<ButtonID>			m_vBuildingsList;
	ButtonID						m_CurrentGroupBtn;
};

#endif