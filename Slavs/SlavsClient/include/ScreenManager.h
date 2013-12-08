#ifndef ScreenManager_h
#define ScreenManager_h

#include "ScreenBase.h"

class ScreenManager
{
	ScreenBase* m_pCurrentScreen;
public:
	ScreenManager();
	~ScreenManager();

	void Update(float elapsedTime);

	void SetCurrentScreen(ScreenBase* currentScreen);
	ScreenBase* GetCurrentScreen() const { return m_pCurrentScreen; }
};

#endif