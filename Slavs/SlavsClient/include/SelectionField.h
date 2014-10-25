#ifndef SelectionField_h
#define SelectionField_h

#include "ClientEnums.h"
#include "CommandBox.h"
#include <CEGUI\CEGUI.h>

class HUD;

class SelectionField
{
public:
	SelectionField(HUD *owner, CEGUI::Window *rootWindow);
	~SelectionField();

	void Hide();

	void SingleSelect(ObjectInformation* objInfo);
	void MultipleSelect(void* data);
	bool HandleData(CommandToUI cmd, void*data);
protected:
	void Initialize();

	enum SelectionState { SS_MultipleSelection, SS_SingleSelection, SS_NoSelection };

	CommandBox*		m_pCommandBox;
	HUD*			m_pOwner;
	CEGUI::Window*	m_pRootWindow;
	CEGUI::Window*	m_pSelectedObjsWnd;
	SelectionState	m_State;
};

#endif