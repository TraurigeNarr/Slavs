#include "stdafx.h"
#include "InputManager.h"
#include "InputSubscriber.h"

#include "OgreFramework.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////////

InputManager::InputManager(OgreFramework& i_ogre_framework)
  : m_ogre_framework(i_ogre_framework)
  {
  m_ogre_framework.GetKeyboard()->setEventCallback(this);
  m_ogre_framework.GetMouse()->setEventCallback(this);
  }

InputManager::~InputManager()
  {
  }

void InputManager::Update(long i_elapsed_time)
  {
	m_ogre_framework.GetKeyboard()->capture();
	m_ogre_framework.GetMouse()->capture();
  }

/************************************************************************/
/*                      Event handlers                                  */
/************************************************************************/
bool InputManager::keyPressed(const OIS::KeyEvent &keyEventRef)
  {
	std::for_each(m_subscribers.begin(), m_subscribers.end(), [&keyEventRef](InputSubscriberPtr subsriber)
		{
			subsriber->KeyPressed(keyEventRef);
	});
  std::for_each(m_raw_subscribers.begin(), m_raw_subscribers.end(), [&keyEventRef](InputSubscriber* subsriber)
    {
    subsriber->KeyPressed(keyEventRef);
    });
	return true;
  }

bool InputManager::keyReleased(const OIS::KeyEvent &keyEventRef)
  {
	std::for_each(m_subscribers.begin(), m_subscribers.end(), [&keyEventRef](InputSubscriberPtr subsriber)
	{
		subsriber->KeyReleased(keyEventRef);
	});
  std::for_each(m_raw_subscribers.begin(), m_raw_subscribers.end(), [&keyEventRef](InputSubscriber* subsriber)
    {
    subsriber->KeyReleased(keyEventRef);
    });
	return true;
  }

bool InputManager::mouseMoved(const OIS::MouseEvent &evt)
  {
	std::for_each(m_subscribers.begin(), m_subscribers.end(), [&evt](InputSubscriberPtr subsriber)
	{
		subsriber->MouseMoved(evt);
	});
  std::for_each(m_raw_subscribers.begin(), m_raw_subscribers.end(), [&evt](InputSubscriber* subsriber)
    {
    subsriber->MouseMoved(evt);
    });
	return true;
  }

bool InputManager::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
  {
	std::for_each(m_subscribers.begin(), m_subscribers.end(), [&evt, id](InputSubscriberPtr subsriber)
	{
		subsriber->MousePressed(evt, id);
	});
  std::for_each(m_raw_subscribers.begin(), m_raw_subscribers.end(), [&evt, id](InputSubscriber* subsriber)
    {
    subsriber->MousePressed(evt, id);
    });
	return true;
  }

bool InputManager::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
  {
	std::for_each(m_subscribers.begin(), m_subscribers.end(), [&evt, id](InputSubscriberPtr subsriber)
	  {
		subsriber->MouseReleased(evt, id);
	  });
  std::for_each(m_raw_subscribers.begin(), m_raw_subscribers.end(), [&evt, id](InputSubscriber* subsriber)
    {
    subsriber->MouseReleased(evt, id);
    });
	return true;
  }

//////////////////////////////////////////////////////////////////////////
// Internal staff

void InputManager::ValidateCache()
  {
  m_subscribers_cache.clear();

  std::for_each(m_subscribers.begin(), m_subscribers.end(), [this](InputSubscriberPtr subsriber)
    {
    m_subscribers_cache.insert(subsriber.get());
    });

  std::for_each(m_raw_subscribers.begin(), m_raw_subscribers.end(), [this](InputSubscriber* subsriber)
    {
    m_subscribers_cache.insert(subsriber);
    });
  }

//////////////////////////////////////////////////////////////////////////
// InputManager

void InputManager::AddSubscriber(InputSubscriberPtr subscriber)
  {
	if(m_subscribers.end() == std::find(m_subscribers.begin(), m_subscribers.end(), subscriber))
		m_subscribers.push_back(subscriber);
  ValidateCache();
  }

void InputManager::AddSubscriber(InputSubscriber* ip_subscriber)
  {
  if(m_raw_subscribers.end() == std::find(m_raw_subscribers.begin(), m_raw_subscribers.end(), ip_subscriber))
    m_raw_subscribers.push_back(ip_subscriber);
  ValidateCache();
  }

void InputManager::RemoveSubscriber(InputSubscriberPtr subscriber)
  {
	std::vector<InputSubscriberPtr>::iterator iterFind = std::find(m_subscribers.begin(), m_subscribers.end(), subscriber);
	if(m_subscribers.end() != iterFind)
		m_subscribers.erase(iterFind);
  ValidateCache();
  }

void InputManager::RemoveSubscriber(InputSubscriber* ip_subscriber)
  {
  std::vector<InputSubscriber*>::iterator iterFind = std::find(m_raw_subscribers.begin(), m_raw_subscribers.end(), ip_subscriber);
  if(m_raw_subscribers.end() != iterFind)
    m_raw_subscribers.erase(iterFind);
  ValidateCache();
  }

const std::set<InputSubscriber*>& InputManager::GetSubscribers() const
  {
  return m_subscribers_cache;
  }