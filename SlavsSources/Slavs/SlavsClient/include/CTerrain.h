#ifndef CTerrain_h
#define CTerrain_h

#include <string>
#include <OgreNode.h>
#include <OgreSceneManager.h>

class CTerrain
{
public:
	CTerrain(const std::string &configName);
	~CTerrain();

	Ogre::Plane* GetPlane() const { return m_PlaneTerrain; }

protected:
	void CreateTerrain();

	Ogre::SceneNode *m_Node;
	Ogre::SceneManager *m_SceneMgr;
	Ogre::Plane *m_PlaneTerrain;
};

#endif