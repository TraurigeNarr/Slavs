#include "ScreenManager.h"

ScreenManager::ScreenManager()
	: m_pCurrentScreen(NULL)
{}

ScreenManager::~ScreenManager()
{}

void ScreenManager::Update(float elapsedTime)
{
	if(NULL != m_pCurrentScreen)
	{
		CEGUI::System::getSingleton().injectTimePulse(elapsedTime);
		m_pCurrentScreen->Update(elapsedTime);
	}
}

void ScreenManager::SetCurrentScreen(ScreenBase* currentScreen)
{
	if(NULL != m_pCurrentScreen)
	{
		delete m_pCurrentScreen;
		m_pCurrentScreen = NULL;
	}
	m_pCurrentScreen = currentScreen;
	m_pCurrentScreen->Init();
}