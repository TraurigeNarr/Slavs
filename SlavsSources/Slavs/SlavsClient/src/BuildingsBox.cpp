#include "BuildingsBox.h"
#include "HUD.h"

#include "VisualInformation.h"

#include <CEGUI\Size.h>

BuildingsBox::BuildingsBox(HUD *owner, CEGUI::Window *rootWindow)
	: m_pOwner(owner), m_pRootCEGUIWindow(rootWindow), m_CurrentGroupBtn(Btn_BGroup_General)
{
	Show();
}

BuildingsBox::~BuildingsBox()
{
	m_pRootCEGUIWindow->destroy();
}

void BuildingsBox::Show()
{
	m_pRootCEGUIWindow->activate();
	ScreenBase::ClearWindow(m_pRootCEGUIWindow);
	ShowBuildingTypes();
	ShowBuildingsList();
}

void BuildingsBox::Hide()
{
	m_pRootCEGUIWindow->deactivate();
	m_vBuildingsList.clear();
	ScreenBase::ClearWindow(m_pRootCEGUIWindow);
}

bool BuildingsBox::ButtonHandler(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);
	ButtonID btnID = (ButtonID)args->window->getID();

	if(Singleton<VisualInformation>::GetInstancePtr()->IsBuildingGroupBtn(btnID))
	{
		m_CurrentGroupBtn = btnID;
		Show();
		return true;
	}

	return m_pOwner->ButtonHandler(e);
}

void BuildingsBox::ShowBuildingTypes()
{
	m_vBuildingsList = Singleton<VisualInformation>::GetInstancePtr()->GetBuildingGroups();
	float sizeOfBtn = 1.0f/m_vBuildingsList.size();

	for(unsigned int i = 0; i < m_vBuildingsList.size(); ++i)
	{
		ButtonInformation *info = Singleton<VisualInformation>::GetInstancePtr()->GetBtnInformation(m_vBuildingsList.at(i));
		if(NULL == info)
			continue;
		CEGUI::Window* btn = CEGUI::WindowManager::getSingletonPtr()->createWindow("TaharezLook/Button");
		btn->setText(info->BtnText);
		btn->setTooltipText(info->BtnToolTip);
		btn->setSize(CEGUI::USize(CEGUI::UDim(sizeOfBtn, 0), CEGUI::UDim(0.3f, 0)));
		btn->setPosition(CEGUI::UVector2(CEGUI::UDim(i*sizeOfBtn, 0), CEGUI::UDim(0.7f, 0)));
		btn->setID(info->BtnID);

		btn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BuildingsBox::ButtonHandler, this));

		m_pRootCEGUIWindow->addChild(btn);
	}
	m_vBuildingsList.clear();
}

void BuildingsBox::ShowBuildingsList()
{
	m_vBuildingsList = Singleton<VisualInformation>::GetInstancePtr()->GetBuildings(m_CurrentGroupBtn);

	float sizeOfBtn = 0.2f;

	for(unsigned int i = 0; i < m_vBuildingsList.size(); ++i)
	{
		ButtonInformation *info = Singleton<VisualInformation>::GetInstancePtr()->GetBtnInformation(m_vBuildingsList.at(i));
		if(NULL == info)
			continue;
		CEGUI::Window* btn = CEGUI::WindowManager::getSingletonPtr()->createWindow("TaharezLook/Button");
		btn->setText(info->BtnText);
		btn->setTooltipText(info->BtnToolTip);
		btn->setSize(CEGUI::USize(CEGUI::UDim(sizeOfBtn, 0.0f), CEGUI::UDim(0.7f, 0.0f)));
		btn->setPosition(CEGUI::UVector2(CEGUI::UDim(i*sizeOfBtn, 0.0f), CEGUI::UDim(0.0f, 0)));
		btn->setID(info->BtnID);

		btn->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BuildingsBox::ButtonHandler, this));

		m_pRootCEGUIWindow->addChild(btn);
	}
}