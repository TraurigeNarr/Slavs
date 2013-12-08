#ifndef CreateLevelState_h
#define CreateLevelState_h

#include "AppState.h"
#include <list>

namespace net
{
	class Connection;
	class Address;
}

class ClientCreateLevelState : public AppState
{
	//combobox for available maps
	std::string m_sCurrentMap;
public:
  ClientCreateLevelState();
  ~ClientCreateLevelState();

	virtual void  Enter(std::shared_ptr<Application> ip_owner) override;
	virtual void  Execute(std::shared_ptr<Application> ip_owner, long i_elapsed_time) override;
	virtual void  Exit(std::shared_ptr<Application> ip_owner) override;

	bool				  KeyPressed(const OIS::KeyEvent &keyEventRef);
	bool					KeyReleased(const OIS::KeyEvent &keyEventRef);

	bool					MouseMoved(const OIS::MouseEvent &evt);
	bool					MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool					MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	bool					ButtonPressed(ButtonID id, void* extraData);
protected:
	bool					Connect(std::shared_ptr<Application> ip_owner, const net::Address& address);
	void					AvailableMaps(net::Connection* connection);
	inline void		SendSelectedMap(net::Connection* connection, const char* selectedMap, int size);
	void					CreateScene();
	inline void		BuildGUI();
};


#endif