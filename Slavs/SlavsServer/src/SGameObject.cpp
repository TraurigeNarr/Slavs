#include "SGameObject.h"

#include "IController.h"
#include "SGameContext.h"
#include "misc\ServerEnums.h"
//components
#include "SBuildingComp.h"
#include "SDynamicObjCom.h"
#include "SHumanComponent.h"
#include "SManufacureCom.h"
#include "SStaticObjCom.h"
#include "SStoreHouseCom.h"
//common
#include <Game/CommandData.h>
#include <Math/Vector2D.h>
#include <Utilities/XmlUtilities.h>
#include <Utilities/TemplateFunctions.h>

SGameObject::SGameObject(SGameContext* pContext, long ID, int otype, int iMask, const TiXmlElement* configElement/* = NULL*/, Vector2D *extraData/* = NULL*/, IController* owner/* = NULL*/)
	: IGameObject(ID, otype, (QueryMask)iMask)
  , m_pOwner(owner)
  , m_pContext(pContext)
  , m_iFlags(0)
{
	if(NULL != configElement)
		ParseElement(configElement, extraData);
}

SGameObject::~SGameObject()
  {
  while(!m_components.empty()) 
    {
    IComponent* obj = m_components.back();
    m_components.pop_back();
    delete obj;
    }
  }

void SGameObject::Init()
{

}

void SGameObject::TickPerformed()
{
	TickForComponents();
}

bool SGameObject::ProcessCommand(const CommandData& cData)
{
	Telegram telegram(0, GetID(), GetID(), cData.GetCommand(), cData.GetExtraData());

	bool processed = false;
	processed = HandleMessage(telegram);

	return processed;
}

bool SGameObject::HandleMessage(const Telegram& msg)
{
	switch(msg.Msg)
	{
	case Msg_ObtainProfession:
			m_type = static_cast<SManufacureCom*>(msg.ExtraInfo)->GetWorkerType();
      break;
  case Msg_HumanFired:
    m_type = *static_cast<ObjectType*>(msg.ExtraInfo);
    break;
  case Msg_HumanDied:
    m_bDestroyed = true;
    return true;
    break;
	}
	for(unsigned int i = 0; i < m_components.size(); ++i)
	{
		if(m_components.at(i)->HandleMessage(msg))
		{
			return true;
		}
	}
	return false;
}

void SGameObject::ParseElement(const TiXmlElement* configElement, Vector2D *extraData)
{
	std::string elementName = "";

	const TiXmlElement* childElement = 0;

	while (childElement = XmlUtilities::IterateChildElements(configElement, childElement))
	{
		elementName = childElement->Value();

		if("component" == elementName)
		{
			AddComponent(childElement, extraData);
		}
	}

	//initialize all references between components inside of game object
	for(size_t i = 0; i < m_components.size(); ++i)
		m_components.at(i)->Probe();
}

void SGameObject::AddComponent(const TiXmlElement* componentElement, Vector2D *extraData)
{
	std::string componentType = XmlUtilities::GetStringAttribute(componentElement, "type");
	if("static_object" == componentType)
	{
		SStaticObjCom *staticComponent = NULL;
		if(NULL != extraData)
			staticComponent = new SStaticObjCom(this, componentElement, extraData);
		else
			staticComponent = new SStaticObjCom(this, componentElement);
		m_components.push_back(staticComponent);
		m_iFlags |= c_t_staticObject;
	}
	if("dynamic_object" == componentType)
	{
		SDynamicObjCom *dynamicComponent = NULL;
		if(NULL != extraData)
			dynamicComponent = new SDynamicObjCom(this, componentElement, extraData);
		else
			dynamicComponent = new SDynamicObjCom(this, componentElement);
		m_components.push_back(dynamicComponent);
		m_iFlags |= c_t_dynamicObject;
	}
	if("store_house" == componentType)
	{
    throw std::runtime_error("Deprecated usage");
		///SStoreHouseCom *storeHouse = new SStoreHouseCom(this, componentElement);
		///m_components.push_back(storeHouse);
		///m_iFlags |= c_t_storeHouse;
	}
	if("human" == componentType)
	{
		//SHumanComponent* human = new SHumanComponent(this, componentElement);
		//m_components.push_back(human);
		//m_iFlags |= c_t_human;
	}
	if("house" == componentType)
	{
    throw std::runtime_error("Deprecated usage");
		//SHouseComponent *house = new SHouseComponent(this, componentElement);
		//m_components.push_back(house);
		//m_iFlags |= c_t_house;
	}
	if("manufacture" == componentType)
	{
    throw std::runtime_error("Deprecated usage");
		///SManufacureCom *resMining = new SManufacureCom(this, componentElement);
		///m_components.push_back(resMining);
		///m_iFlags |= c_t_manufacture;
	}
	if("building" == componentType)
	{
    throw std::runtime_error("Deprecated usage");
		//SBuildingComp *building = new SBuildingComp(this, componentElement);
		//m_components.push_back(building);
		//m_iFlags |= c_t_building;
	}
}

void SGameObject::AddComponent(IComponent* ip_component)
  {
  m_components.push_back(ip_component);
  }