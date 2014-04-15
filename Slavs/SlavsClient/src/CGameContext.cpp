#include "CGameContext.h"
#include "CGameObject.h"
#include "CRendererComponent.h"
#include "CTerrain.h"
#include "OgreFramework.h"
//common
#include <Game/GameObjectState.h>
#include <Game/IGameObject.h>
#include <Game/Enumerations.h>
#include <Patterns/Singleton.h>
#include <Utilities/FileUtilities.h>
#include <Utilities/TemplateFunctions.h>
#include <Utilities/XmlUtilities.h>
//standard
#include <algorithm>
#include <string>

const std::string ObjectsMapFileName = "client\\configs\\ObjectsMap.xml";

CGameContext::CGameContext()
  : IGameContext("")
  , m_iOwnMask(0)
  , mh_undefined_configuration(nullptr)
  {  }

CGameContext::CGameContext(const std::string& mapName, int ownMask)
	: IGameContext(mapName), m_iOwnMask(ownMask)
  , mh_undefined_configuration(nullptr)
{
	
}

CGameContext::~CGameContext()
  {
  
  }

void CGameContext::Init()
  {
  InitObjectsMap();
  }

void CGameContext::TickPerformed()
{
	/*std::for_each(m_mGameObjects.begin(), m_mGameObjects.end(), [](std::pair<long, IGameObject*> p)
	{
		p.second->TickPerformed();
	});*/
}

void CGameContext::ReleaseContext()
  {
  ReleaseGameObjects();
  }

void CGameContext::RemoveObject(CGameObject* gameObject)
{
	ObjectsMap::const_iterator iter = m_mGameObjects.find(gameObject->GetID());

	if(m_mGameObjects.end() != iter)
	{
		m_mGameObjects.erase(gameObject->GetID());
		delete gameObject;
		gameObject = NULL;
	}
}

void CGameContext::ApplyState(long id, const GameObjectState& state)
{
	ObjectsMap::const_iterator iter = m_mGameObjects.find(id);

	//if this object is in map only applies state to it
	if(m_mGameObjects.end() != iter)
	{
		if(0 != (state.iFlags & GOF_Destroyed))
		{
			RemoveObject(static_cast<CGameObject*>(iter->second));
			return;
		}
		static_cast<CGameObject*>(iter->second)->ApplyState(state);
	}
	//if not - add object and apply state
	else
	{
		std::map<int, const TiXmlElement*>::const_iterator iter = m_mConfigElements.find(state.oType);
    const TiXmlElement* p_configuration_elem = iter != m_mConfigElements.end() ? iter->second : mh_undefined_configuration.get();
		if(p_configuration_elem != nullptr)
		{
			int m = QM_All;
			if(QM_OwnButUncontroller & state.iMask)
				m |= QM_OwnButUncontroller;
			else if(QM_Environment & state.iMask)
				m |= QM_Environment;
			else if(QM_Resources & state.iMask)
				m |= QM_Resources;
			else if(QM_Enemy & state.iMask)
				m |= QM_Enemy;
			else if(m_iOwnMask & state.iMask)
				m |= QM_Own;

			CGameObject *newObject = new CGameObject(id, state.oType, m, p_configuration_elem);
			m_mGameObjects.insert(std::pair<long, CGameObject*>(id, newObject));
			newObject->ApplyState(state);
		}
		else
			Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Creation of object fails: " + state.oType);
	}
}

void CGameContext::AddTerrain(const std::string &configName)
{
	m_pTerrain = new CTerrain(configName);

}

const TiXmlElement* CGameContext::GetConfigElement(int oType) const
{
	std::map<int, const TiXmlElement*>::const_iterator iter = m_mConfigElements.find(oType);

	if(m_mConfigElements.end() != iter)
		return iter->second;

	return mh_undefined_configuration.get();
}

IGameObject* CGameContext::GetObjectByRay(const Ogre::RaySceneQueryResult &results) const
{
		Ogre::RaySceneQueryResult::const_iterator iter = results.begin();
		Ogre::RaySceneQueryResult::const_iterator endIter = results.end();
		ObjectsMap::const_iterator objEndIter = m_mGameObjects.end();

		for(iter; iter != endIter; ++iter)
		{
			//get begin of the array of the Actors
			ObjectsMap::const_iterator objIter =  GetIteratorBegin();
			for(objIter; objIter != objEndIter ; ++objIter)
			{
				CRendererComponent *cRenderer = objIter->second->GetComponent<CRendererComponent>();
				if(iter->movable->getParentSceneNode()->getName() == cRenderer->GetRootSceneNode().getName())
					return objIter->second;
			}
		}
		return NULL;
}

void CGameContext::ClearData()
{
	ReleaseGameObjects();
}

void CGameContext::InitObjectsMap()
{
	m_mConfigElements.clear();
	TiXmlDocument document;
	if(!XmlUtilities::LoadXmlDocument(FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), ObjectsMapFileName), document))
	{
		MessageBoxA(NULL, "Error in loading xml-config of actors: ObjectsMap.", "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		exit(1);
	}

	TiXmlElement *rootElem = document.RootElement();

	std::string elementName = "";

	const TiXmlElement* childElement = 0;

	int otype = OT_None;
  std::map<std::string, int>::iterator it;
	while ((childElement = XmlUtilities::IterateChildElements(rootElem, childElement)))
	{
		elementName = childElement->Value();

    if (elementName == "Undefined")
      {
      mh_undefined_configuration.reset(new TiXmlElement(*childElement));
      }

    it = m_definitions_map.find(elementName);
		otype = it != m_definitions_map.end() ? it->second : OT_None;

		if(OT_None != otype)
		  {
			if(m_mConfigElements.end() == m_mConfigElements.find(otype))
				m_mConfigElements[otype] = new TiXmlElement(*childElement);
		  }
	}
}

void CGameContext::AddDefinition(const std::pair<std::string, int>&& i_definition)
  {
  m_definitions_map.insert(i_definition);
  }