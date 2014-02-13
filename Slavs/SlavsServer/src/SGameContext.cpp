 #include "SGameContext.h"
//server
#include "HumanStates.h"
#include "IController.h"
#include "SGameObject.h"
#include "Management/Goverment.h"
#include "Management/IEconomyManager.h"
#include "Management/IStoreSystem.h"
//common
#include <Game/GameObjectState.h>
#include <Math/Vector2D.h>
#include <Patterns/Singleton.h>
#include <Utilities/TemplateFunctions.h>
#include <Utilities/XmlUtilities.h>
#include <Utilities/FileUtilities.h>
//standard
#include <algorithm>
#include <iostream>
#include <list>
#include <stdlib.h>//srand
#include <time.h>

//////////////////////////////////////////////////////////////////////////
template<> std::shared_ptr<HumanGlobal> Singleton<HumanGlobal>::mp_singleton = nullptr;
template<> std::shared_ptr<HumanIdle> Singleton<HumanIdle>::mp_singleton = nullptr;
template<> std::shared_ptr<HumanMove> Singleton<HumanMove>::mp_singleton = nullptr;
template<> std::shared_ptr<HumanWork> Singleton<HumanWork>::mp_singleton = nullptr;

long SGameContext::lID = 0;
const std::string ObjectsMapFileName = "server\\configs\\ObjectsMap.xml";
const std::string InitialObjectsFileName = "server\\configs\\InitialObjects.xml";
const std::wstring OutputDirectoryName = L".\\server\\data";

//////////////////////////////////////////////////////////////////////////



SGameContext::SGameContext(const std::string& mapName)
	: IGameContext(mapName), m_writer(OutputDirectoryName, true)
{
	//seed random number generator
	srand((unsigned) time(NULL));
	m_vSpawns = new std::vector<Vector2D*>();
	InitObjectsMap();
	//initialize human states
	new Singleton<HumanGlobal>(new HumanGlobal());
	new Singleton<HumanIdle>(new HumanIdle());
	new Singleton<HumanMove>(new HumanMove());
	new Singleton<HumanWork>(new HumanWork());
}

SGameContext::~SGameContext()
{
}

void SGameContext::ReleaseContext()
  {
  m_dead_pool.clear();
  try
    {
    WriteData();
    }
  catch(std::exception& e)
    {
    std::cout << e.what();
    }
  ClearData();
  //uninitialized human states
  Singleton<HumanGlobal>::ReleaseIfValid(); 
  Singleton<HumanIdle>::ReleaseIfValid(); 
  Singleton<HumanMove>::ReleaseIfValid(); 
  Singleton<HumanWork>::ReleaseIfValid(); 
  lID = 0;
  }

OutputManager& SGameContext::GetOutputManager()
{
  return m_writer;
}

void SGameContext::WriteData()
{
  m_writer.WriteData();
}

void SGameContext::Init()
{
}

void SGameContext::TickPerformed()
{
  while(!m_dead_pool.empty())
  {
    RemoveObject(m_dead_pool.back());
    m_dead_pool.pop_back();
  }

	std::for_each(m_mGameObjects.begin(), m_mGameObjects.end(), [this](std::pair<long, SGameObject*> p)
	{
		p.second->TickPerformed();
		if(p.second->Destroyed())
			m_dead_pool.push_back(p.second);
	});
}

void SGameContext::AddSpawn(const Vector2D& spawn)
{
	Vector2D *v = new Vector2D(spawn);
	m_vSpawns->push_back(v);
}

void SGameContext::AddObstacle(SStaticObjCom *const obstacle)
{
	m_vObstacles.push_back(obstacle);
}

void SGameContext::AddDynamicObject(SDynamicObjCom *const dynamicObj)
{
	m_vDynamicObjects.push_back(dynamicObj);
}

SGameObject* SGameContext::AddObject(int oType, const Vector2D &position, IController* owner /* = NULL */, QueryMask qm /* = QM_ALL */)
{
	SGameObject *newObject = NULL;
	std::map<int, ObjectConfiguration*>::const_iterator iter = m_mObjectsInformation.find(oType);
	if(m_mObjectsInformation.end() != iter)
	{
		newObject = new SGameObject(this, lID, oType,
			NULL != owner ? owner->GetMask() | qm : qm, 
			iter->second->ConfigElement, new Vector2D(position), owner);
		m_mGameObjects.insert(std::pair<long, SGameObject*>(newObject->GetID(), newObject));
		++lID;
	}
	return newObject;
}

void SGameContext::RemoveObject(SGameObject* gameObject)
{
	ObjectsMap::const_iterator iter = m_mGameObjects.find(gameObject->GetID());

	if(m_mGameObjects.end() != iter)
	{
		m_mGameObjects.erase(gameObject->GetID());
		delete gameObject;
		gameObject = NULL;
	}
}

void SGameContext::RemoveReferences(SGameObject* gameObject)
{
	if(gameObject->HasComponent(SGameObject::c_t_staticObject))
	{
		Obstacles::iterator iterObstacle = m_vObstacles.begin();
		while(iterObstacle != m_vObstacles.end() || (*iterObstacle)->GetOwner()->GetID() != gameObject->GetID())
			++iterObstacle;
		if(iterObstacle != m_vObstacles.end())
			m_vObstacles.erase(iterObstacle);
	}
	else if(gameObject->HasComponent(SGameObject::c_t_dynamicObject))
	{
		DynamicObjects::iterator iterDyn = m_vDynamicObjects.begin();
		while(iterDyn != m_vDynamicObjects.end() || (*iterDyn)->GetOwner()->GetID() != gameObject->GetID())
			++iterDyn;
		if(iterDyn != m_vDynamicObjects.end())
			m_vDynamicObjects.erase(iterDyn);
	}
}

void SGameContext::ClearData()
{
	ClearVector(*m_vSpawns);
  delete m_vSpawns;
	ReleaseGameObjects();
	ClearSTLMap(m_mObjectsInformation);
}

void SGameContext::InitObjectsMap()
{
	ClearSTLMap(m_mObjectsInformation);
	TiXmlDocument document;
	if(!XmlUtilities::LoadXmlDocument(FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), ObjectsMapFileName), document))
	{
		std::cout << "Error in loading xml-config of actors: ObjectsMap." << std::endl;
		std::cin.get();
		exit(1);
	}

	TiXmlElement *rootElem = document.RootElement();

	std::string elementName = "";

	const TiXmlElement* childElement = 0;

	int otype = OT_None;

	while ((childElement = XmlUtilities::IterateChildElements(rootElem, childElement)))
	{
		elementName = childElement->Value();
		otype = IGameObject::GetGameObjectType(elementName);

		if(OT_None != otype)
		{
			if(m_mObjectsInformation.end() == m_mObjectsInformation.find(otype))
			{
				ObjectConfiguration* objInfo = new ObjectConfiguration;
				objInfo->ConfigElement = new TiXmlElement(*childElement);
				GetNeededResources(childElement, objInfo);
				m_mObjectsInformation[otype] = objInfo;
			}
		}
    else if(elementName == "resources")
      GetResourcesConfigs(childElement);
	}
	GetInitialSceneObjects();
}

bool SGameContext::GetResConfigcs(GameResourceType i_type, ResourceConfigs& o_configs) const
{
  std::map<GameResourceType, ResourceConfigs>::const_iterator it = m_resources_configs.find(i_type);

  if(m_resources_configs.end() != it)
  {
    o_configs = it->second;
    return true;
  }

  return false;
}

void SGameContext::GetInitialSceneObjects()
{
	m_vInitialObjects.clear();
	TiXmlDocument document;
	if(!XmlUtilities::LoadXmlDocument(FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), InitialObjectsFileName), document))
	{
		std::cout << "Error in loading xml-config of actors: ObjectsMap." << std::endl;
		std::cin.get();
		exit(1);
	}

	TiXmlElement *rootElem = document.RootElement();

	std::string elementName = "";

	const TiXmlElement* childElement = 0;

	int otype = OT_None;
	GameResourceType grType = GR_None;

	while ((childElement = XmlUtilities::IterateChildElements(rootElem, childElement)))
	{
		elementName = childElement->Value();
		//define initial objects------------------------------------
		otype = IGameObject::GetGameObjectType(elementName);
		if(OT_None != otype)
		{
			m_vInitialObjects.push_back(otype);
			continue;
		}
		//define initial resources------------------------------------
		if("resources" == elementName)
		{
			const TiXmlElement* resElement = NULL;
			while ((resElement = XmlUtilities::IterateChildElements(childElement, resElement)))
			{
				elementName = resElement->Value();
				grType = GameResourceBox::DefineResType(elementName);
				if(GR_None != grType)
				{
					int number = XmlUtilities::GetIntAttribute(resElement, "number", 0);
					m_mInitialResources[grType] = number;
					continue;
				}
			}
		}
		//----------------------------------------------------------
	}
}

void SGameContext::GiveInitialObjects(IController* controller)
{
	Vector2D pos = *m_vSpawns->at(0);
	int changeDirAt = m_vInitialObjects.size()/2;
	std::for_each(m_vInitialObjects.begin(), m_vInitialObjects.end(), [this, &controller, &pos, &changeDirAt](int oType)
		{
			SGameObject* curObj = AddObject(oType, pos, controller);
			if(curObj->HasComponent(SGameObject::c_t_staticObject))
			{
				pos.x += 2*curObj->GetComponent<SStaticObjCom>()->GetBRadius();
			}
	});
	std::vector<GameResourceBox*> resToAdd;
	std::for_each(m_mInitialResources.begin(), m_mInitialResources.end(), [this, &controller, &resToAdd](std::pair<GameResourceType, size_t> resource)
		{
      ResourceConfigs res_conf;
      if(GetResConfigcs(resource.first, res_conf))
        resToAdd.push_back(new GameResourceBox(resource.first, resource.second, 0, res_conf.m_price));
	});
  controller->GetGoverment().GetEconomyManager()->GetStoreSystem()->Add(resToAdd);
}

ResourceMap *const SGameContext::GetNeededResources(int oType)
{
	std::map<int, ObjectConfiguration*>::const_iterator iter = m_mObjectsInformation.find(oType);

	if(m_mObjectsInformation.end() != iter)
		return &iter->second->NeededResources;
	return NULL;
}

void SGameContext::GetResourcesConfigs(const TiXmlElement* i_config_element)
{
  std::string elementName = "";

  const TiXmlElement* res_element = 0;

  GameResourceType gr_type = GR_None;

  while ((res_element = XmlUtilities::IterateChildElements(i_config_element, res_element)))
  {
    gr_type = GameResourceBox::DefineResType(res_element->Value());

    if(GR_None != gr_type)
    {
      ResourceConfigs res_conf;
      res_conf.m_type = gr_type;
      res_conf.m_price = XmlUtilities::GetIntAttribute(res_element, "price", 1);
      res_conf.m_calories = XmlUtilities::GetRealAttribute(res_element, "calories", 50);
      m_resources_configs[gr_type] = res_conf;
    }
  }
}

void SGameContext::GetNeededResources(const TiXmlElement* element, ObjectConfiguration* objInfo)
{
	int number	= XmlUtilities::GetIntAttribute(element, "tree", 0);
	if(0 != number)
		objInfo->NeededResources[GR_Tree] = number;

	number		= XmlUtilities::GetIntAttribute(element, "wood", 0);
	if(0 != number)
		objInfo->NeededResources[GR_Wood] = number;

	number		= XmlUtilities::GetIntAttribute(element, "stone", 0);
	if(0 != number)
		objInfo->NeededResources[GR_Stone] = number;

	number		= XmlUtilities::GetIntAttribute(element, "ironOre", 0);
	if(0 != number)
		objInfo->NeededResources[GR_IronOre] = number;

	number		= XmlUtilities::GetIntAttribute(element, "skin", 0);
	if(0 != number)
		objInfo->NeededResources[GR_Skin] = number;

	number		= XmlUtilities::GetIntAttribute(element, "wool", 0);
	if(0 != number)
		objInfo->NeededResources[GR_Wool] = number;
}

void SGameContext::TagObstaclesWithinViewRange(SDynamicObjCom *const pVehicle, float radius)
{
	Obstacles::iterator curObj = m_vObstacles.begin();
	Obstacles::const_iterator EndIter = m_vObstacles.end();

	for(curObj; curObj != EndIter; ++curObj)
	{
		(*curObj)->bTag = false;

		Vector2D to = (*curObj)->GetPosition() - pVehicle->GetPosition();
		//the bounding radius of the other is taken into account by adding it 
		//to the range
		float range = radius + (*curObj)->GetBRadius();

		//if entity within range, tag for further consideration. (working in
		//distance-squared space to avoid sqrts)
		if(to.LengthSq() < range*range)
		{
			(*curObj)->bTag = true;
		}
	}
}

void SGameContext::TagDynamicObjectsWithinViewRange(SDynamicObjCom *const pVehicle, float radius)
{
	DynamicObjects::iterator curObj = m_vDynamicObjects.begin();
	DynamicObjects::const_iterator EndIter = m_vDynamicObjects.end();

	for(curObj; curObj != EndIter; ++curObj)
	{
		(*curObj)->bTag = false;

		Vector2D to = (*curObj)->GetPosition() - pVehicle->GetPosition();
		//the bounding radius of the other is taken into account by adding it 
		//to the range
		float range = radius + (*curObj)->GetBRadius();

		//if entity within range, tag for further consideration. (working in
		//distance-squared space to avoid sqrts)
		if((*curObj) != pVehicle && to.LengthSq() < range*range)
		{
			(*curObj)->bTag = true;
		}
	}
}