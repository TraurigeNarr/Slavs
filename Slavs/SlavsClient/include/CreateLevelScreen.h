#ifndef CreateLevelScreen_h
#define CreateLevelScreen_h

#include "ScreenBase.h"

class CreateLevelScreen : public ScreenBase
{
public:
	CreateLevelScreen();
	~CreateLevelScreen();

	void Init();
	void Update(float elapsedTime);

	bool HandleData(CommandToUI id, void* data);
	bool ButtonHandler(const CEGUI::EventArgs &e);
private:
	CEGUI::Combobox *m_cCombobox;
};

#endif