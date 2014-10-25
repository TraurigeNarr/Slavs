#ifndef CommandBox_h
#define CommandBox_h

#include <CEGUI\CEGUI.h>
#include "ObjectInformation.h"

class HUD;

class CommandBox
{
public:
	CommandBox(HUD* owner, CEGUI::Window* rootWindow);
	~CommandBox();

	void			Show(ObjectInformation* objInfo);
	void			Hide();
protected:
	bool			ButtonHandler(const CEGUI::EventArgs &e);
	void			DrawCommandBox(ObjectInformation* objInfo);


	HUD*			m_pOwner;
	CEGUI::Window*	m_pRootWindow;
};

#endif