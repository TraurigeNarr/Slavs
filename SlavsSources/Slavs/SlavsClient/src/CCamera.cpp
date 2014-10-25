#include "CCamera.h"
#include "CGameContext.h"
#include "CTerrain.h"
#include "ClientGameState.h"
#include "InputManager.h"
#include "OgreFramework.h"

#include <Patterns/Singleton.h>

#include <OgreConfigFile.h>
#include <OgreQuaternion.h>

#include <Utilities\XmlUtilities.h>
#include <Utilities\FileUtilities.h>

const std::string ConfigFileName = "client\\configs\\game\\camera.cfg";
const float PI = 3.14159265f;

CCamera::CCamera()
{
	m_vPosition = Ogre::Vector3::ZERO;
	Init();
}

CCamera::~CCamera()
{
	Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->destroyCamera(m_pCamera);
	Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->destroySceneNode(m_pNode);
	Singleton<InputManager>::GetInstancePtr()->RemoveSubscriber(this);
}

void CCamera::TickPerformed()
{
	m_pNode->translate(m_vPosition);

	Ogre::Quaternion q = m_pCamera->getOrientation();
	q.normalise();
	m_pCamera->setOrientation(q);
}

void CCamera::Init()
{
	Ogre::Vector3 position = GetOptions();

	Ogre::SceneManager *pSceneManager = Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager;

	m_pCamera = pSceneManager->createCamera("Camera");

	m_pCamera->setPosition(position);
	Ogre::Vector3 vec = CalcLookAt(position);
	m_pCamera->lookAt(vec);
	/*m_pCamera->setPosition(Ogre::Vector3(0, 25, -50));
	m_pCamera->lookAt(Ogre::Vector3(0, 0, 0));*/
	m_pCamera->setNearClipDistance(1.0f);
	m_pCamera->setFarClipDistance(50000);

	ResetScreenSize();

	Singleton<OgreFramework>::GetInstancePtr()->m_pViewport->setCamera(m_pCamera);

	m_pNode = pSceneManager->getRootSceneNode()->createChildSceneNode();

	m_pNode->attachObject(m_pCamera);

	Singleton<InputManager>::GetInstancePtr()->AddSubscriber(this);
}

Ogre::Vector3 CCamera::GetOptions()
{
	Ogre::ConfigFile cf;
	cf.load(FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), ConfigFileName));

	Ogre::Vector3 *position = new Ogre::Vector3(0, 0, 0);
	position->x = atof(cf.getSetting("positionX").c_str());
	position->y = atof(cf.getSetting("positionY").c_str());
	position->z = atof(cf.getSetting("positionZ").c_str());

	m_fAngle = atof(cf.getSetting("angle").c_str());
	m_fMoveSpeed = atof(cf.getSetting("moveSpeed").c_str());
	return *position;
}

Ogre::Vector3& CCamera::CalcLookAt(const Ogre::Vector3 &camPosition)
{
	Ogre::Vector3 *vec = new Ogre::Vector3();
	vec->x = camPosition.x;
	vec->y = 0;
	float t = tan(m_fAngle * PI/180);
	vec->z = camPosition.z - camPosition.y * t;
	return *vec;
}
/************************************************************************/
/*                        Input handlers                                */
/************************************************************************/

bool CCamera::KeyPressed(const OIS::KeyEvent &keyEventRef)
{
	switch(keyEventRef.key)
	{
	case OIS::KC_UP:
	case OIS::KC_W:
		m_vPosition.z -= m_fMoveSpeed;
		break;
	case OIS::KC_DOWN:
	case OIS::KC_S:
		m_vPosition.z += m_fMoveSpeed;
		break;
	case OIS::KC_LEFT:
	case OIS::KC_A:
		m_vPosition.x -= m_fMoveSpeed;
		break;
	case OIS::KC_RIGHT:
	case OIS::KC_D:
		m_vPosition.x += m_fMoveSpeed;
		break;
	}
	return true;
}

bool CCamera::KeyReleased(const OIS::KeyEvent &keyEventRef)
{
	switch(keyEventRef.key)
	{
	case OIS::KC_UP:
	case OIS::KC_W:
	case OIS::KC_DOWN:
	case OIS::KC_S:
	case OIS::KC_LEFT:
	case OIS::KC_A:
	case OIS::KC_RIGHT:
	case OIS::KC_D:
		m_vPosition = Ogre::Vector3::ZERO;
		break;
	}
	return true;
}

bool CCamera::MouseMoved(const OIS::MouseEvent &evt)
{
	return true;
}

bool CCamera::MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

bool CCamera::MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

bool CCamera::ButtonPressed(ButtonID id, void *extraData)
{
	return false;
}

/************************************************************************/
/*                    Helper functions                                  */
/************************************************************************/
void CCamera::ResetScreenSize()
{
	m_vScreenSize.x = Ogre::Real(Singleton<OgreFramework>::GetInstancePtr()->m_pViewport->getActualWidth());
	m_vScreenSize.y = Ogre::Real(Singleton<OgreFramework>::GetInstancePtr()->m_pViewport->getActualHeight());

	m_pCamera->setAspectRatio( m_vScreenSize.x / m_vScreenSize.y);
}

Ogre::Camera* CCamera::GetCamera() const
{
	return m_pCamera;
}

Ogre::Ray CCamera::GetRay(float screenX, float screenY) const
{
	return m_pCamera->getCameraToViewportRay(screenX, screenY);
}

Ogre::Vector3 CCamera::GetWorldPoint(float mouseX, float mouseY) const
{
	Ogre::Ray cameraRay = m_pCamera->getCameraToViewportRay(mouseX / m_vScreenSize.x, mouseY / m_vScreenSize.y);
	Ogre::Plane *plane = Singleton<ClientGameState>::GetInstancePtr()->GetContext()->GetTerrain()->GetPlane();
	std::pair<bool, Ogre::Real> t = cameraRay.intersects(*plane);
	if(t.first)
		return cameraRay.getPoint(t.second);
	return Ogre::Vector3();
}

