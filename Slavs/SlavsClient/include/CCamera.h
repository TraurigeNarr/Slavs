#ifndef CCamera_h
#define CCamera_h

#include "InputSubscriber.h"

#include <Game/TimeController.h>

#include <OgreCamera.h>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreRay.h>


class CCamera : public InputSubscriber, public TickListener
{
public:
	CCamera();
	~CCamera();

	void			TickPerformed();

	bool			KeyPressed(const OIS::KeyEvent &keyEventRef);
	bool			KeyReleased(const OIS::KeyEvent &keyEventRef);

	bool			MouseMoved(const OIS::MouseEvent &evt);
	bool			MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool			MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	
	bool			ButtonPressed(ButtonID id, void *extraData);

	void			ResetScreenSize();
	Ogre::Camera*	GetCamera() const;
	Ogre::Ray		GetRay(float screenX, float screenY) const;
	Ogre::Vector3	GetWorldPoint(float mouseX, float mouseY) const;
protected:
	void			Init();
	Ogre::Vector3	GetOptions();
	Ogre::Vector3&	CalcLookAt(const Ogre::Vector3 &camPosition);
	
	Ogre::Camera *m_pCamera;
	Ogre::SceneNode *m_pNode;
	Ogre::Vector3 m_vPosition;

	float m_fAngle;
	float m_fMoveSpeed;
	Ogre::Vector2 m_vScreenSize;
};

#endif