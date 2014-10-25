#ifndef InformationField_h
#define InformationField_h

#include "BuildingsBox.h"
#include "SelectionField.h"
#include "ClientEnums.h"

class HUD;

class InformationField
{
public:
	InformationField(HUD* owner, CEGUI::Window* rootWindow);
	~InformationField();
	//some unit were selected, then handles Single or Multiple selection -> passes it to SelectionField
	bool HandleData(CommandToUI id, void* data);
protected:
	void Initialize();

	SelectionField*	m_pSelectionField;
	BuildingsBox*	m_pBuildingsBox;

	HUD*			m_pOwner;
	CEGUI::Window*	m_pRootWindow;

};

#endif