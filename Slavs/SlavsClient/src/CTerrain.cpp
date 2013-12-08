#include "CTerrain.h"
#include "OgreFramework.h"

#include <Ogre.h>

#include <Patterns/Singleton.h>

CTerrain::CTerrain(const std::string &configName)
{
	CreateTerrain();
}

void CTerrain::CreateTerrain()
{
	m_PlaneTerrain = new Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", "Environment",
		*m_PlaneTerrain, 50000, 50000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity *entity;

	entity = Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->createEntity("ground");
	m_Node = Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->getRootSceneNode()->createChildSceneNode();
	m_Node->attachObject(entity);
	m_Node->setPosition(Ogre::Vector3::ZERO);

	entity->setMaterialName("Ground", "Environment");
	entity->setCastShadows(true);
	entity->setVisible(true);
}