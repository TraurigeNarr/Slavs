#ifndef CPlayerController_h
#define CPlayerController_h

#include "InputSubscriber.h"
#include "Application.h"

#include <Game/GameResources.h>

#include <vector>

class CPlayerController : public InputSubscriber
{
public:
	CPlayerController(Application* owner);
	~CPlayerController();

	/*Deserialize set of resource containers. PT_ControllerState.*/
	int Deserialize(char* buf, int size);
private:
	void ApplyChanges(GeneralResourceInfo* container);

	Application*		m_pOwner;
	std::vector<GeneralResourceInfo*> m_vResourceContainers;
};

#endif