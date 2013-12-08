#include "CreateLevelScreen.h"
#include "InputManager.h"

#include <algorithm>

CreateLevelScreen::CreateLevelScreen()
{}

CreateLevelScreen::~CreateLevelScreen()
{}


void CreateLevelScreen::Init()
{
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setImage("TaharezLook/MouseArrow");
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	m_pRootCEGUIWindow = wmgr.createWindow("DefaultWindow");

	CEGUI::Window *goToMenu = wmgr.createWindow("TaharezLook/Button");
	goToMenu->setText("Back to menu");
	goToMenu->setSize(CEGUI::USize(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	goToMenu->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8f, 0), CEGUI::UDim(0.9f, 0)));
	goToMenu->setID(Btn_UI_Menu);

	CEGUI::Window *loadLevel = wmgr.createWindow("TaharezLook/Button");
	loadLevel->setText("Load Level");
	loadLevel->setSize(CEGUI::USize(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
	loadLevel->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8f, 0), CEGUI::UDim(0.82f, 0)));
	loadLevel->setID(Btn_UI_StartGame);
	//subscribe
	goToMenu->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&ScreenBase::ButtonHandler, static_cast<ScreenBase*>(this)));

	loadLevel->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&ScreenBase::ButtonHandler, static_cast<ScreenBase*>(this)));

	//create combobox
	m_cCombobox = static_cast<CEGUI::Combobox*>(wmgr.createWindow("TaharezLook/Combobox"));
	m_cCombobox->setReadOnly(true);
	m_cCombobox->setSize(CEGUI::USize(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.15f, 0)));
	m_cCombobox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.1f, 0)));
	m_cCombobox->setID(Btn_UI_MapChanged);
	m_cCombobox->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, 
		CEGUI::Event::Subscriber(&CreateLevelScreen::ButtonHandler, this));

	m_pRootCEGUIWindow->addChild(goToMenu);
	m_pRootCEGUIWindow->addChild(loadLevel);
	m_pRootCEGUIWindow->addChild(m_cCombobox);

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(m_pRootCEGUIWindow);
}

void CreateLevelScreen::Update(float elapsedTime)
{

}

bool CreateLevelScreen::HandleData(CommandToUI id, void* data)
{
	bool processed = false;
	switch(id)
	{
	case Cmd_UI_AddMap:
		CEGUI::ListboxTextItem* itemCombobox = new CEGUI::ListboxTextItem((char*)data, 1);
		itemCombobox->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
		m_cCombobox->addItem(itemCombobox);
		processed = true;
		break;
	}
	return processed;
}

bool CreateLevelScreen::ButtonHandler(const CEGUI::EventArgs &e)
{
	const std::vector<InputSubscriber*> &subs = Singleton<InputManager>::GetInstancePtr()->GetSubscribers();
	const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);
	ButtonID id = (ButtonID)args->window->getID();
	void *data = NULL;
	if(Btn_UI_MapChanged == id)
		data = (char*)m_cCombobox->getSelectedItem()->getText().c_str();
	std::for_each(subs.begin(), subs.end(), [id, data](InputSubscriber* subscriber)
	{
		subscriber->ButtonPressed(id, data);
	});
	return false;
}