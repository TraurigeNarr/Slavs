#include "LoadGameScreen.h"

#include <CEGUI/ImageManager.h>
#include <CEGUI/Size.h>
#include <CEGUI/GUIContext.h>

LoadGameScreen::LoadGameScreen()
{}

LoadGameScreen::~LoadGameScreen()
{}


void LoadGameScreen::Init()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::GUIContext& default_context = CEGUI::System::getSingleton().getDefaultGUIContext();
	m_pRootCEGUIWindow = wmgr.createWindow("DefaultWindow");
	default_context.getMouseCursor().hide();
  CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(m_pRootCEGUIWindow);
	
	//create and tune imageset
  //temp solution - no image while loading
	//CEGUI::ImageManager::getSingleton().addFromImageFile("Backgrounds", "LoadScreenBackground.jpg", "Imagesets");
	// Menu Background
	CEGUI::DefaultWindow* MenuBackground = static_cast<CEGUI::DefaultWindow*>(wmgr.createWindow("TaharezLook/StaticImage", "Background"));
	m_pRootCEGUIWindow->addChild( MenuBackground );
	MenuBackground->setPosition( CEGUI::UVector2( CEGUI::UDim( 0.0f, 0.0f ), CEGUI::UDim( 0.0f, 0.0f) ) );
	MenuBackground->setSize( CEGUI::USize( CEGUI::UDim( 1.0f, 0.0f ), CEGUI::UDim( 1.0f, 0.0f ) ) );  // full screen

	MenuBackground->setProperty("Image", "set:Backgrounds image:full_image");
}

void LoadGameScreen::Update(float elapsedTime)
{

}