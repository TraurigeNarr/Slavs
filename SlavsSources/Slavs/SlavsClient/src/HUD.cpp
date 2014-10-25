#include "HUD.h"
#include "ClientEnums.h"
#include "InputManager.h"

#include <CEGUI/PropertyHelper.h>

#include <boost/bind.hpp>

HUD::HUD()
	: m_pInformField(NULL), m_pResourcePanel(NULL)
{}

HUD::~HUD()
{
	if(NULL != m_pInformField)
		delete m_pInformField;
	if(NULL != m_pResourcePanel)
		delete m_pResourcePanel;
}


void HUD::Init()
{
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setImage("TaharezLook/MouseArrow");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	m_pRootCEGUIWindow = wmgr.createWindow("DefaultWindow");
	//menu
	CEGUI::Window *backToMenu = wmgr.createWindow("TaharezLook/Button");
	backToMenu->setText("Menu");
	backToMenu->setSize(CEGUI::USize(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	backToMenu->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.05f, 0)));
	backToMenu->setID(Btn_UI_Menu);
	backToMenu->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&ScreenBase::ButtonHandler, static_cast<ScreenBase*>(this)));
	m_pRootCEGUIWindow->addChild(backToMenu);

	//create root window for information field
	CEGUI::Window *pInfoRootWnd = wmgr.createWindow("DefaultWindow");
	//place to the right bottom of the screen
	pInfoRootWnd->setSize(CEGUI::USize(CEGUI::UDim(0.81f, 0), CEGUI::UDim(0.2f, 0)));
	pInfoRootWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0.19f, 0), CEGUI::UDim(0.8f, 0)));
	//initialize Information window
	m_pInformField = new InformationField(this, pInfoRootWnd);
	m_pRootCEGUIWindow->addChild(pInfoRootWnd);

	//create root window for resource panel
	CEGUI::Window* pResPanelRootWnd = wmgr.createWindow("DefaultWindow");
	pResPanelRootWnd->setSize(CEGUI::USize(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.2f, 0)));
	pResPanelRootWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0.67f, 0), CEGUI::UDim(0.0f, 0)));
	//initialize resource panel
	m_pResourcePanel = new ResourcePanel(this, pResPanelRootWnd);
	m_pRootCEGUIWindow->addChild(pResPanelRootWnd);
	//create button for hiding/showing resource panel
	CEGUI::Window* pHideShowResPanel = wmgr.createWindow("DefaultWindow");
	pHideShowResPanel->setSize(CEGUI::USize(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.1f, 0)));
	pHideShowResPanel->setPosition(CEGUI::UVector2(CEGUI::UDim(0.97f, 0), CEGUI::UDim(0.0f, 0)));
	pHideShowResPanel->setText("\\/");
	pHideShowResPanel->setTooltipText("Show/Hide resource panel");
	m_pRootCEGUIWindow->addChild(pHideShowResPanel);
	
  m_help_screen = new HelpBox(m_pRootCEGUIWindow);
  m_help_screen->Init();
  CEGUI::Window* show_help = wmgr.createWindow("TaharezLook/Button");
  show_help->setText("Help");
  show_help->setID(Btn_UI_Help);
  show_help->subscribeEvent(CEGUI::PushButton::EventClicked,
    CEGUI::Event::Subscriber(boost::bind(&HelpBox::ButtonHandler, boost::ref(m_help_screen), _1)));

	show_help->setSize(CEGUI::USize(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	show_help->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.12f, 0)));
  m_pRootCEGUIWindow->addChild(show_help);
  
	//set GUI sheet
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(m_pRootCEGUIWindow);
}

void HUD::Update(float elapsedTime)
{
	
}

bool HUD::HandleData(CommandToUI id, void* data)
{
	bool processed = false;
	
  switch (id)
  {
  case Cmd_UI_ResourcesChanged:
    m_pResourcePanel->UpdateResources(static_cast<ResourceInformation*>(data));
    processed = true;
    break;
  }

	if(!processed)
		m_pInformField->HandleData(id, data);

	return processed;
}