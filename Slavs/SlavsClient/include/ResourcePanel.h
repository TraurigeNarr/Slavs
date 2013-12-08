#ifndef ResourcePanel_h
#define ResourcePanel_h
//client
#include "ClientEnums.h"
#include "ObjectInformation.h"
//common
#include "Game/GameResources.h"
//external
#include <CEGUI\CEGUI.h>
//standard
#include <vector>
#include <map>

class HUD;

typedef std::map<GameResourceGroup, std::vector<ResourceInformation*>> RGroupMap;

class ResourcePanel
{
public:
	ResourcePanel(HUD* owner, CEGUI::Window* rootWindow);
	~ResourcePanel();

	void UpdateResources(ResourceInformation* resInfo);
	
	void Show();
	void Hide();
protected:
	void Initialize();
	void ShowResGroup(GameResourceGroup resGroup);
	inline void HideResGroup();
	bool ButtonHandler(const CEGUI::EventArgs &e);

	HUD*				m_pOwner;
	CEGUI::Window*		m_pRootWindow;
	CEGUI::Window*		m_pResInGroup;
	ButtonID			m_CurrentlyExpandPanel;
	GameResourceGroup	m_CurrentlyExpandGroup;

	std::vector<ButtonID> m_vResourceGroups;
	RGroupMap m_mResources;
};

#endif