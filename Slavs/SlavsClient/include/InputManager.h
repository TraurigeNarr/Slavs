#ifndef InputManager_h
#define InputManager_h

#include "InputSubscriber.h"

#include <Patterns/Singleton.h>

#include <OIS.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <vector>

class InputManager : public OIS::MouseListener, public OIS::KeyListener 
{
public:
	InputManager();
	~InputManager();

	void Update(float ElapsedTime);

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	void AddSubscriber(InputSubscriber* subscriber);
	void RemoveSubscriber(const InputSubscriber* subscriber);
	const std::vector<InputSubscriber*>& GetSubscribers() const { return m_vSubscribers; }
private:
	std::vector<InputSubscriber*> m_vSubscribers;
};

#endif