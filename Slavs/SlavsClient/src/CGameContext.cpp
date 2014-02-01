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

CGameContext::CGameContext(const std::string& mapName, int ownMask)
	: IGameContext(mapName), m_iOwnMask(ownMask)
{
	InitObjectsMap();
}

CGameContext::~CGameContext()
  {
  
  }

void CGameContext::Init()
{}

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
		std::map<ObjectType, const TiXmlElement*>::const_iterator iter = m_mConfigElements.find(state.oType);
		if(m_mConfigElements.end() != iter)
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

			CGameObject *newObject = new CGameObject(id, state.oType, m, iter->second);
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

const TiXmlElement* CGameContext::GetConfigElement(ObjectType oType) const
{
	std::map<ObjectType, const TiXmlElement*>::const_iterator iter = m_mConfigElements.find(oType);

	if(m_mConfigElements.end() != iter)
		return iter->second;

	return NULL;
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

	ObjectType otype = OT_None;

	while ((childElement = XmlUtilities::IterateChildElements(rootElem, childElement)))
	{
		elementName = childElement->Value();
		otype = IGameObject::GetGameObjectType(elementName);

		if(OT_None != otype)
		{
			if(m_mConfigElements.end() == m_mConfigElements.find(otype))
				m_mConfigElements[otype] = new TiXmlElement(*childElement);
		}
	}
}
