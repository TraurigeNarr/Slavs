#ifndef HUD_h
#define HUD_h

#include "ScreenBase.h"
#include "InformationField.h"
#include "ResourcePanel.h"
#include "HelpBox.h"


class HUD : public ScreenBase
{
public:
	HUD();
	~HUD();

	void Init();
	void Update(float elapsedTime);
	//some unit were selected, then HUD handles std::vector<ObjectType> and displays information
	bool HandleData(CommandToUI id, void* data);
protected:
	InformationField*	m_pInformField;
	ResourcePanel*		m_pResourcePanel;
  HelpBox*          m_help_screen;
};

#endif