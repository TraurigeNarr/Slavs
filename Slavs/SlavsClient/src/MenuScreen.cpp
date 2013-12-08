#include "MenuScreen.h"

MenuScreen::MenuScreen()
{}

MenuScreen::~MenuScreen()
{}


void MenuScreen::Init()
{
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setImage("TaharezLook/MouseArrow");
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	m_pRootCEGUIWindow = wmgr.createWindow("DefaultWindow", "MenuScreen");

	CEGUI::Window *enterGame = wmgr.createWindow("TaharezLook/Button");
	enterGame->setText("Create");
	enterGame->setSize(CEGUI::USize(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	enterGame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.4f,0)));
	enterGame->setID(Btn_UI_CreateLevel);

	CEGUI::Window *opts = wmgr.createWindow("TaharezLook/Button");
	opts->setText("Options");
	opts->setSize(CEGUI::USize(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	opts->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.5f, 0)));
	opts->setID(Btn_UI_Options);

	CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button");
	quit->setText("Quit");
	quit->setSize(CEGUI::USize(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.6f, 0)));
	quit->setID(Btn_UI_Quit);

	enterGame->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&ScreenBase::ButtonHandler, static_cast<ScreenBase*>(this)));

	opts->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&ScreenBase::ButtonHandler, static_cast<ScreenBase*>(this)));

	quit->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&ScreenBase::ButtonHandler, static_cast<ScreenBase*>(this)));

	m_pRootCEGUIWindow->addChild(enterGame);
	m_pRootCEGUIWindow->addChild(opts);
	m_pRootCEGUIWindow->addChild(quit);

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(m_pRootCEGUIWindow);
}

void MenuScreen::Update(float elapsedTime)
{

}