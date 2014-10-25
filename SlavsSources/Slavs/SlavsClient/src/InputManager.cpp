#include "InputManager.h"
#include "OgreFramework.h"

#include <algorithm>

#include <Patterns/Singleton.h>

InputManager::InputManager()
{
	//subscribe for events
	Singleton<OgreFramework>::GetInstancePtr()->m_pKeyboard->setEventCallback(this);
	Singleton<OgreFramework>::GetInstancePtr()->m_pMouse->setEventCallback(this);

	Singleton<OgreFramework>::GetInstancePtr()->m_pRenderWnd->resetStatistics();
}

InputManager::~InputManager()
{
}

void InputManager::Update(float ElapsedTime)
{
	Singleton<OgreFramework>::GetInstancePtr()->m_pKeyboard->capture();
	Singleton<OgreFramework>::GetInstancePtr()->m_pMouse->capture();
}

/************************************************************************/
/*                      Event handlers                                  */
/************************************************************************/
bool InputManager::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	std::for_each(m_vSubscribers.begin(), m_vSubscribers.end(), [&keyEventRef](InputSubscriber* subsriber)
		{
			subsriber->KeyPressed(keyEventRef);
	});
	return true;
}

bool InputManager::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	std::for_each(m_vSubscribers.begin(), m_vSubscribers.end(), [&keyEventRef](InputSubscriber* subsriber)
	{
		subsriber->KeyReleased(keyEventRef);
	});
	Singleton<OgreFramework>::GetInstancePtr()->KeyReleased(keyEventRef);
	return true;
}

bool InputManager::mouseMoved(const OIS::MouseEvent &evt)
{
	std::for_each(m_vSubscribers.begin(), m_vSubscribers.end(), [&evt](InputSubscriber* subsriber)
	{
		subsriber->MouseMoved(evt);
	});
	Singleton<OgreFramework>::GetInstancePtr()->MouseMoved(evt);
	return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	std::for_each(m_vSubscribers.begin(), m_vSubscribers.end(), [&evt, id](InputSubscriber* subsriber)
	{
		subsriber->MousePressed(evt, id);
	});
	Singleton<OgreFramework>::GetInstancePtr()->MousePressed(evt, id);
	return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	std::for_each(m_vSubscribers.begin(), m_vSubscribers.end(), [&evt, id](InputSubscriber* subsriber)
	{
		subsriber->MouseReleased(evt, id);
	});
	Singleton<OgreFramework>::GetInstancePtr()->MouseReleased(evt, id);
	return true;
}

void InputManager::AddSubscriber(InputSubscriber* subscriber)
{
	if(m_vSubscribers.end() == std::find(m_vSubscribers.begin(), m_vSubscribers.end(), subscriber))
		m_vSubscribers.push_back(subscriber);
}

void InputManager::RemoveSubscriber(const InputSubscriber* subscriber)
{
	std::vector<InputSubscriber*>::iterator iterFind = std::find(m_vSubscribers.begin(), m_vSubscribers.end(), subscriber);
	if(m_vSubscribers.end() != iterFind)
	{
		m_vSubscribers.erase(iterFind);
	}
}