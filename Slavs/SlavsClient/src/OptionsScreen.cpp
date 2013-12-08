#include "OptionsScreen.h"

OptionsScreen::OptionsScreen()
{}

OptionsScreen::~OptionsScreen()
{}


void OptionsScreen::Init()
{
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setImage("TaharezLook/MouseArrow");
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	m_pRootCEGUIWindow = wmgr.createWindow("DefaultWindow", "Menu");

	CEGUI::Window *goToMenu = wmgr.createWindow("TaharezLook/Button");
	goToMenu->setText("Back to menu");
	goToMenu->setSize(CEGUI::USize(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	goToMenu->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.55f, 0)));
	goToMenu->setID(Btn_UI_Menu);

	goToMenu->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&ScreenBase::ButtonHandler, static_cast<ScreenBase*>(this)));

	m_pRootCEGUIWindow->addChild(goToMenu);

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(m_pRootCEGUIWindow);
}

void OptionsScreen::Update(float elapsedTime)
{

}