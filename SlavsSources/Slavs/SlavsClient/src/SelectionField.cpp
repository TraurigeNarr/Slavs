#include "SelectionField.h"
#include "HUD.h"

#include "ObjectInformation.h"

#include <algorithm>

#include <CEGUI/Size.h>
#include <CEGUI/Window.h>

SelectionField::SelectionField(HUD *owner, CEGUI::Window *rootWindow)
	: m_pOwner(owner), m_pRootWindow(rootWindow), m_pCommandBox(NULL), m_State(SS_NoSelection)
{
	Initialize();
}

SelectionField::~SelectionField()
{
	if(NULL != m_pCommandBox)
		delete m_pCommandBox;
}

void SelectionField::Hide()
{
	m_pCommandBox->Hide();
	ScreenBase::ClearWindow(m_pSelectedObjsWnd);
}

void SelectionField::Initialize()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	m_pSelectedObjsWnd = wmgr.createWindow("DefaultWindow");
	m_pSelectedObjsWnd->setSize(CEGUI::USize(CEGUI::UDim(0.79f, 0), CEGUI::UDim(1.0f, 0)));
	m_pSelectedObjsWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.0f, 0)));


	//root window for command box
	CEGUI::Window* pCmdBox = wmgr.createWindow("DefaultWindow");
	pCmdBox->setSize(CEGUI::USize(CEGUI::UDim(0.21f, 0), CEGUI::UDim(1.0f, 0)));
	pCmdBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.79f, 0), CEGUI::UDim(0.0f, 0)));
	m_pCommandBox = new CommandBox(m_pOwner, pCmdBox);

	m_pRootWindow->addChild(m_pSelectedObjsWnd);
	m_pRootWindow->addChild(pCmdBox);
}

void SelectionField::SingleSelect(ObjectInformation* objInfo)
{
	ScreenBase::ClearWindow(m_pSelectedObjsWnd);
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	m_pCommandBox->Show(objInfo);

	//set name
	CEGUI::DefaultWindow *wnd = static_cast<CEGUI::DefaultWindow*>(wmgr.createWindow("TaharezLook/StaticText"));
	wnd->setSize(CEGUI::USize(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.2f, 0)));
	wnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.0f, 0)));
	wnd->setText(objInfo->GetName());

	m_pSelectedObjsWnd->addChild(wnd);
	
	int i = 1;
	int j = 0;
	std::for_each(objInfo->vInformation.begin(), objInfo->vInformation.end(), [this, &i, &j, &wmgr](std::string info)
	{
		CEGUI::DefaultWindow *wnd = static_cast<CEGUI::DefaultWindow*>(wmgr.createWindow("TaharezLook/StaticText"));
		wnd->setSize(CEGUI::USize(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.2f, 0)));
		wnd->setPosition(CEGUI::UVector2(CEGUI::UDim(j*0.3f, 0), CEGUI::UDim(i*0.2f, 0)));

		m_pSelectedObjsWnd->addChild(wnd);
		wnd->setText(info);
		++i;
		if(i == 5)
		{
			i = 0;
			++j;
		}
	});
	m_State = SS_SingleSelection;
}

void SelectionField::MultipleSelect(void* data)
{
	SingleSelect(static_cast<ObjectInformation*>(data));
	m_State = SS_MultipleSelection;
}

bool SelectionField::HandleData(CommandToUI cmd, void*data)
{
	switch(cmd)
	{
	case Cmd_UI_GOStateChanged:
		{
			if(SS_SingleSelection == m_State)
			{
				SingleSelect(static_cast<ObjectInformation*>(data));
			}
			break;
		}
	}
	return false;
}