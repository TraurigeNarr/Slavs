#include "ResourcePanel.h"
//client
#include "HUD.h"
#include "VisualInformation.h"
//common
#include <Utilities/TemplateFunctions.h>
//standard
#include <algorithm>

ResourcePanel::ResourcePanel(HUD* owner, CEGUI::Window* rootWindow)
	: m_pOwner(owner), m_pRootWindow(rootWindow), m_pResInGroup(NULL), m_CurrentlyExpandPanel(Btn_None), m_CurrentlyExpandGroup(GRG_None)
{
	m_vResourceGroups.push_back(Btn_RG_Material);
	m_vResourceGroups.push_back(Btn_RG_Productive);
	m_vResourceGroups.push_back(Btn_RG_Food);
	m_vResourceGroups.push_back(Btn_RG_Military);
	m_vResourceGroups.push_back(Btn_RG_Social);
	Show();
}

ResourcePanel::~ResourcePanel()
{
	std::for_each(m_mResources.begin(), m_mResources.end(), [](std::pair<GameResourceGroup, std::vector<ResourceInformation*>> p)
	{
		ClearVector(p.second);
	});
}

void ResourcePanel::UpdateResources(ResourceInformation* resInfo)
{
	assert(resInfo &&
		"<ResourcePanel::UpdateResources>: Resource information is NULL.");
	RGroupMap::iterator iter = m_mResources.find(resInfo->grGroup);
	if(m_mResources.end() != iter)
	{
		bool find = false;
		//find specific resource in vector
		for(size_t i = 0; i < m_mResources[resInfo->grGroup].size(); ++i)
			if(m_mResources[resInfo->grGroup].at(i)->grType == resInfo->grType)
			{
				m_mResources[resInfo->grGroup].at(i)->sValue = resInfo->sValue;
				find = true;
			}
		//if not find
		if(!find)
			m_mResources[resInfo->grGroup].push_back(resInfo);
	}
	else
	{
		m_mResources[resInfo->grGroup].push_back(resInfo);
	}
	if(m_CurrentlyExpandGroup == resInfo->grGroup)
		ShowResGroup(m_CurrentlyExpandGroup);
}

void ResourcePanel::Show()
{
	m_pRootWindow->activate();
	Initialize();
}

void ResourcePanel::Hide()
{
	m_pRootWindow->deactivate();
	ScreenBase::ClearWindow(m_pRootWindow);
}

void ResourcePanel::Initialize()
{
	CEGUI::WindowManager* wmgr = CEGUI::WindowManager::getSingletonPtr();

	for(size_t i = 0; i < m_vResourceGroups.size(); ++i)
	{
		//create button
		CEGUI::Window *button = wmgr->createWindow("TaharezLook/Button");
		button->setSize(CEGUI::USize(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.5f, 0)));
		button->setPosition(CEGUI::UVector2(CEGUI::UDim(i*0.2f, 0), CEGUI::UDim(0.0f, 0)));
		button->setID(m_vResourceGroups.at(i));
		m_pRootWindow->addChild(button);

		ButtonInformation *btnInfo = Singleton<VisualInformation>::GetInstancePtr()->GetBtnInformation(m_vResourceGroups.at(i));
		if(NULL != btnInfo)
		{
			button->setText(btnInfo->BtnText);
			button->setTooltipText(btnInfo->BtnToolTip);
			button->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::Event::Subscriber(&ResourcePanel::ButtonHandler, this));
		}
	}

	//create detail resource window
	m_pResInGroup = CEGUI::WindowManager::getSingletonPtr()->createWindow("DefaultWindow");
	m_pResInGroup->setSize(CEGUI::USize(CEGUI::UDim(1.0f, 0), CEGUI::UDim(0.5f, 0)));
	m_pResInGroup->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.5f, 0)));
	m_pRootWindow->addChild(m_pResInGroup);
}

void ResourcePanel::ShowResGroup(GameResourceGroup resGroup)
{
	assert(m_pResInGroup && 
		"<ResourcePanel::ShowResGroup>: NULL detailed resource group window pointer.");
	ScreenBase::ClearWindow(m_pResInGroup);
	

	RGroupMap::iterator iter = m_mResources.find(resGroup);
	if(m_mResources.end() != iter)
	{
		CEGUI::WindowManager* wmgr = CEGUI::WindowManager::getSingletonPtr();
		//show information about resources that belongs to resGroup
    size_t j = 0;
		for(size_t i = 0; i < m_mResources[resGroup].size(); ++i)
		{
      if(i != 0 && i%5 == 0)
        ++j;
			CEGUI::DefaultWindow *staticText = static_cast<CEGUI::DefaultWindow*>(wmgr->createWindow("TaharezLook/StaticText"));
			staticText->setSize(CEGUI::USize(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.5f, 0)));
			staticText->setPosition(CEGUI::UVector2(CEGUI::UDim(i*0.2f, 0), CEGUI::UDim(j*0.5f, 0)));
			staticText->setText(m_mResources[resGroup].at(i)->sValue);
			staticText->setTooltipText(GameResourceBox::GetResTypeString(m_mResources[resGroup].at(i)->grType) + ": " + m_mResources[resGroup].at(i)->sValue);
			m_pResInGroup->addChild(staticText);
		}
	}
}

bool ResourcePanel::ButtonHandler(const CEGUI::EventArgs &e)
{
	bool processed = false;
	
	const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);
	ButtonID btnID = (ButtonID)args->window->getID();

	//if button of the expanded resource is pressed hide additional panel
	if(m_CurrentlyExpandPanel == btnID)
	{
		HideResGroup();
		return true;
	}

	switch(btnID)
	{
	case Btn_RG_Material:
		ShowResGroup(GRG_Material);
		m_CurrentlyExpandPanel = btnID;
		m_CurrentlyExpandGroup = GRG_Material;
		break;
	case Btn_RG_Productive:
		ShowResGroup(GRG_Productive);
		m_CurrentlyExpandPanel = btnID;
		m_CurrentlyExpandGroup = GRG_Productive;
		break;
	case Btn_RG_Food:
		ShowResGroup(GRG_Food);
		m_CurrentlyExpandPanel = btnID;
		m_CurrentlyExpandGroup = GRG_Food;
		break;
	case Btn_RG_Military:
		ShowResGroup(GRG_Military);
		m_CurrentlyExpandPanel = btnID;
		m_CurrentlyExpandGroup = GRG_Military;
		break;
	case Btn_RG_Social:
		ShowResGroup(GRG_Social);
		m_CurrentlyExpandPanel = btnID;
		m_CurrentlyExpandGroup = GRG_Social;
		break;
	}

	return processed;
}

void ResourcePanel::HideResGroup()
{
	ScreenBase::ClearWindow(m_pResInGroup);
	m_CurrentlyExpandPanel = Btn_None;
	m_CurrentlyExpandGroup = GRG_None;
}