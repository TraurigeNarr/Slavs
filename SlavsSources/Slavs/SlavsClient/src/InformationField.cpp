#include "InformationField.h"

#include "CGameObject.h"
#include "CGameContext.h"
#include "ClientGameState.h"

InformationField::InformationField(HUD* owner, CEGUI::Window* rootWindow)
	: m_pOwner(owner), m_pRootWindow(rootWindow), m_pSelectionField(NULL), m_pBuildingsBox(NULL)
{
	Initialize();
}


InformationField::~InformationField()
{
	if(NULL != m_pSelectionField)
		delete m_pSelectionField;
	if(NULL != m_pBuildingsBox)
		delete m_pBuildingsBox;
}

void InformationField::Initialize()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Window* pSelectionBox = wmgr.createWindow("DefaultWindow");
	pSelectionBox->setSize(CEGUI::USize(CEGUI::UDim(1.0f, 0), CEGUI::UDim(1.0f, 0)));
	pSelectionBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.0f, 0)));
	m_pSelectionField = new SelectionField(m_pOwner, pSelectionBox);

	CEGUI::Window* pBuildingsBox = wmgr.createWindow("DefaultWindow");
	pBuildingsBox->setSize(CEGUI::USize(CEGUI::UDim(1.0f, 0), CEGUI::UDim(1.0f, 0)));
	pBuildingsBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.0f, 0)));
	m_pBuildingsBox = new BuildingsBox(m_pOwner, pBuildingsBox);

	m_pRootWindow->addChild(pSelectionBox);
	m_pRootWindow->addChild(pBuildingsBox);
}

bool InformationField::HandleData(CommandToUI id, void* data)
{
	bool processed = false;
	long *objID = NULL;
	ObjectInformation *objInfo = NULL;

	processed = m_pSelectionField->HandleData(id, data);
	if(processed)
	{
		return true;
	}

	switch(id)
	{
	case Cmd_UI_SingleSelection:
		m_pBuildingsBox->Hide();
		objID = static_cast<long*>(data);
		objInfo = static_cast<CGameObject*>(Singleton<ClientGameState>::GetInstancePtr()->GetContext()->GetGameObject(*objID))->GetObjectInfo();
		m_pSelectionField->SingleSelect(objInfo);
		processed = true;
		break;
	case Cmd_UI_MultipleSelection:
		m_pBuildingsBox->Hide();
		objID = static_cast<long*>(data);
		objInfo = static_cast<CGameObject*>(Singleton<ClientGameState>::GetInstancePtr()->GetContext()->GetGameObject(*objID))->GetObjectInfo();
		m_pSelectionField->MultipleSelect(objInfo);
		processed = true;
		break;
	case Cmd_UI_Deselect:
		m_pSelectionField->Hide();
		m_pBuildingsBox->Show();
		processed = true;
		break;
	}
	return processed;
}