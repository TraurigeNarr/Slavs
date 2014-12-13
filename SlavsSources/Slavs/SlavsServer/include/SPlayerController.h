#ifndef SPlayerController_h
#define SPlayerController_h

#include "IController.h"
#include "Game/CommandData.h"

#include "ObjectSelectorHandler.h"

#include <vector>

#include <ISendable.h>

namespace Slavs
  {
  class GameContext;
  class GameObject;
  }

class Vector2D;
class SGameObject;

class SPlayerController 
  : public IController
{
private:
	Slavs::ObjectSelectorHandler m_object_selector;
	std::vector<Slavs::GameObject*> m_selected_objects;

public:
	SPlayerController(int iMask, Slavs::GameContext& i_context);
	~SPlayerController();

	void Init();
	void TickPerformed();

	virtual void HoldPacket(unsigned char* packet, int bytes_read) override;
};

#endif