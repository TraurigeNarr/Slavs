#include "CGameObject.h"
#include "CRendererComponent.h"
//client
#include "ScreenManager.h"
#include "ClientGameState.h"
#include "CGameContext.h"
#include "OgreFramework.h"
#include "VisualInformation.h"
//common
#include <Game/GameObjectState.h>
#include <Utilities\XmlUtilities.h>
#include <Utilities\StringUtilities.h>
#include <Utilities/TemplateFunctions.h>

//////////////////////////////////////////////////////////////////////////
const Ogre::String LB_Material = "LifeBoxMaterial";

//////////////////////////////////////////////////////////////////////////
CGameObject::CGameObject(long ID, ObjectType otype, int iMask/* = 0x00 */, const TiXmlElement* configElement/* = NULL*/)
	: IGameObject(ID, otype, (QueryMask)iMask)
{
	
	m_ObjectInformation = new ObjectInformation(IGameObject::ToString(otype));
	if(NULL != configElement)
		ParseElement(configElement);
}

CGameObject::~CGameObject()
{
	delete m_ObjectInformation;
  ClearVector(m_lComponents);
}

void CGameObject::Init()
{

}

void CGameObject::TickPerformed()
{
	TickForComponents();
}

void CGameObject::ApplyState(const GameObjectState& state)
{
	ParseState(state);

	std::for_each(m_lComponents.begin(), m_lComponents.end(), [&state](IComponent* component)
	{
			component->ApplyState(state);
	});
}

void CGameObject::ReCreateObject(ObjectType oType)
{
	m_Type = oType;
	m_ObjectInformation->Clear();
	const TiXmlElement* configElement = Singleton<ClientGameState>::GetInstancePtr()->GetContext()->GetConfigElement(oType);
	
	const TiXmlElement* childElement = 0;
	const TiXmlElement* commandsElement = 0;

	std::string elementName = "";

	while (childElement = XmlUtilities::IterateChildElements(configElement, childElement))
	{
		elementName = childElement->Value();

		if("component" == elementName)
		{
			if("renderer" == XmlUtilities::GetStringAttribute(childElement, "type"))
			{
				GetComponent<CRendererComponent>()->ChangeModel(childElement);
			}
		}
		if("commands" == elementName)
		{
			while(commandsElement = XmlUtilities::IterateChildElements(childElement, commandsElement))
				m_ObjectInformation->AddCommand(VisualInformation::DefineBtnID(commandsElement->Value()));
		}
	}
}

void CGameObject::ParseElement(const TiXmlElement* configElement)
{
	std::string elementName = "";

	const TiXmlElement* childElement = 0;
	const TiXmlElement* commandsElement = 0;

	while (childElement = XmlUtilities::IterateChildElements(configElement, childElement))
	{
		elementName = childElement->Value();
		
		if("component" == elementName)
		{
			if("renderer" == XmlUtilities::GetStringAttribute(childElement, "type"))
			{
				CRendererComponent *cRenderer = new CRendererComponent(this, childElement);
				m_lComponents.push_back(cRenderer);
			}
		}
		if("commands" == elementName)
		{
			while(commandsElement = XmlUtilities::IterateChildElements(childElement, commandsElement))
				m_ObjectInformation->AddCommand(VisualInformation::DefineBtnID(commandsElement->Value()));
		}
	}
}

void CGameObject::ParseState(const GameObjectState& state)
{
	if(0 != (state.iFlags & GOF_ChangeType))
		ReCreateObject(state.oType);
	//clear information for further update
	m_ObjectInformation->vInformation.clear();
	//if there is resources
	if(0 != (state.iFlags & GOF_Resources))
	{
		for(size_t i = 0; i < state.vContainers.size(); ++i)
		{
			std::string sInfo = "";
			sInfo = GameResourceBox::GetResTypeString(state.vContainers[i]->GetResType()) + "-" 
				+ StringUtilities::t_to_string(state.vContainers[i]->GetResNumber()) + "/" 
				+ StringUtilities::t_to_string(state.vContainers[i]->GetResMaxNumber());
			m_ObjectInformation->vInformation.push_back(sInfo);
			m_ObjectInformation->bStore = true;
		}
		/*if(m_bSelected)
		{
			long id = GetID();
			Singleton<ScreenManager>::GetInstancePtr()->GetCurrentScreen()->HandleData(Cmd_UI_GOStateChanged, m_ObjectInformation);
		}*/
	}
  if(0 != (state.iFlags & GOF_Money))
    m_ObjectInformation->vInformation.push_back("Money: " + StringUtilities::t_to_string(state.uiMoney));
	//for entities that can have skill
	if(0 != (state.iFlags & GOF_HasSkill))
	{
    GetComponent<CRendererComponent>()->AddBillboardSet(LB_Material, 5, 1);
    GetComponent<CRendererComponent>()->SetBillboard(state.uiSkill);
	}

	if(0 != (state.iFlags & GOF_Working))
	{
		m_ObjectInformation->vInformation.push_back("Working");
	}
  //for manufacture look for workers
  if(0 != (state.iFlags & GOF_HasWorkers))
  {
    if(0 == state.iWorkersNumber)
      m_ObjectInformation->vInformation.push_back("Waiting for workers.");
    else
      m_ObjectInformation->vInformation.push_back("Workers: " + StringUtilities::t_to_string(state.iWorkersNumber));
  }
  if(NULL != state.request_for_workers && 0 != (state.iFlags & GOF_RequestForWorkers))
  {
    m_ObjectInformation->vInformation.push_back("Worker`s type:" + IGameObject::ToString(state.request_for_workers->otNeededType));

    if(state.request_for_workers->bAvailability)
    {
      if(state.request_for_workers->bNeedStore)
        m_ObjectInformation->vInformation.push_back("Waits for store or money");
      m_ObjectInformation->vInformation.push_back("Needed Skill:" + StringUtilities::t_to_string(state.request_for_workers->fNeededSkill));    
    }
    else
      m_ObjectInformation->vInformation.push_back("Average Skill:" + StringUtilities::t_to_string(state.request_for_workers->fAverageSkill));
  }
  //for house state
  if(0 != (state.iFlags & GOF_HouseInformation))
  {
    m_ObjectInformation->AddInformation("Inhabitants: " + StringUtilities::t_to_string(state.uiInhabitantNumber));
    m_ObjectInformation->AddInformation("Maximum: " + StringUtilities::t_to_string(state.uiInhabitantsMaxNumber));
    m_ObjectInformation->AddInformation("Working: " + StringUtilities::t_to_string(state.uiInhabitantsWorking));
  }
}