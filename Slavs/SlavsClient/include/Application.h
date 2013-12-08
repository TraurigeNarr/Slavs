#ifndef Application_h
#define Application_h

#include "AppState.h"
#include "Options.h"

#include <Patterns/StateMachine.h>
#include <Patterns/Singleton.h>

#include <OGRE/OgreFrameListener.h>


namespace net
{
	class Connection;
}

class Application : public Ogre::FrameListener
{
public:
	void						Start();
	void						Shutdown();

	StateMachine<Application, long>*	GetFSM() const { return m_pFSM; }

	bool						      SetConnection(net::Connection* connection, const int port);

	net::Connection*			GetConnection() const { return m_cConnection; }
	void						      StopConnection();

  Options&              GetOptions() { return m_options; }

	bool FromGame;
protected:
	bool						_InitSystem();
	void						_Release();

	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	StateMachine<Application, long>*  m_pFSM;
	bool                        m_bShutdown;
	net::Connection*            m_cConnection;
  Options                     m_options;
};

#endif