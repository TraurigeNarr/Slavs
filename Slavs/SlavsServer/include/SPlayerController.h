#ifndef SPlayerController_h
#define SPlayerController_h

#include "IController.h"
#include "Game/CommandData.h"

#include <vector>

#include <ISendable.h>

class Vector2D;
class SGameObject;

class SPlayerController 
  : public IController
{
public:
	SPlayerController(int iMask, SGameContext *context = 0);
	~SPlayerController();

	void Init();
	void TickPerformed();

	void HoldPacket(net::Connection* connection, unsigned char* packet, int bytes_read);
protected:
	void HoldCommand(CommandData* cData);
	void SelectObjects(unsigned char* packet, int bytes_read);
	//objects that were selected by user
	std::vector<SGameObject*> m_vSelectedObjects;
};

#endif