#ifndef GameResource_h
#define GameResource_h
#include <string>

#include <list>
#include <vector>
#include <memory>

//////////////////////////////////////////////////////////////////////////

class TiXmlElement;

//////////////////////////////////////////////////////////////////////////
enum GameResourceGroup
{
	GRG_Material,
	GRG_Productive,
	GRG_Food,
	GRG_Military,
	GRG_Physiological,
	GRG_Social,
	GRG_None
};

enum GameResourceType
{
	GR_Population, GR_Money,
	GR_Tree, GR_Wood, GR_Stone, GR_IronOre,
	GR_Wheat, GR_WheatFlour, GR_Bread, GR_Fruits, 
	GR_Vegetables, GR_Meat, GR_Milk, GR_Cheese, 
	GR_Wool, GR_Skin,
	GR_None
};

/*
Serialization order:
1. Resource type
2. Current number
3. Max number
*/
class GameResourceBox
{
public:
								GameResourceBox(GameResourceType grType, size_t number, size_t iExpireCycles, size_t i_price, TiXmlElement* i_config_element = nullptr, float fQuality = 1);

	void						TickPerformed();
	bool						Empty() const { return m_uiResNumber == 0; }
	void						Clear();

	void						Add(size_t number) { m_uiResNumber += number; }
	GameResourceType			GetGRType() const { return m_ResType; }
	GameResourceGroup			GetGRGroup() const { return m_ResGroup; }
	size_t						GetExpireCycles() const {return m_iExpireCycles;}
	size_t						GetNumber() const { return m_uiResNumber; }
	size_t						GetResources(size_t number);
	bool						Available() const { return m_bAvailable; }
  size_t          Price() const { return m_price; }

	static GameResourceType		DefineResType(const std::string& name);
	static GameResourceGroup	DefineResGroup(GameResourceType grType);
	static std::string			GetResTypeString(GameResourceType grType);
protected:
  void	ParseElelement(const TiXmlElement* i_config_element);
  
  GameResourceType			m_ResType;
	GameResourceGroup			m_ResGroup;
	//this resource available this time
	size_t						m_iExpireCycles;
	//current cycle
	size_t						m_iCurrentCycle;
	float						m_fQuality;
	size_t						m_uiResNumber;
	bool						m_bAvailable;
  size_t          m_price;
};

class GameResourceContainer
{
	std::list<GameResourceBox*>			m_lResourceList;
	GameResourceType					m_ResType;
	size_t								m_uiMaxNumber;
	size_t								m_uiCurrentNumber;
	//true then some resources were added or removed
	mutable bool						m_bStateChanged;
	bool								m_bHumans;
public:
										GameResourceContainer(GameResourceType grType, size_t maxNum, size_t num = 0);
										~GameResourceContainer();
										
	void								TickPerformed();

	bool								AddResource(GameResourceBox* gr);
	void								ExpandContainer(size_t number);
	size_t								PeekResource(size_t number);

	GameResourceType					GetResType() const { return m_ResType; }
	
	std::list<GameResourceBox*>&		GetResourceList();
	void								UpdateAccordingToList();

	size_t								GetResNumber() const { return m_uiCurrentNumber; }
	size_t								GetResMaxNumber() const { return m_uiMaxNumber; }
	bool								AvailableToAdd();
	bool								HasChanges() const { return m_bStateChanged; }

	size_t								Serialize(char* buf_end, size_t size) const;
	size_t								Deserialize(char* buf_end);

	static size_t						NeededSize();
protected:
	size_t								GetResourceNumber() const;
};

struct GeneralResourceInfo 
{
	size_t				uiCurrentResources;
	size_t				uiMaxResources;
	GameResourceType	m_res_type;

	mutable bool		bHasChanges;

	GeneralResourceInfo()
		: m_res_type(GR_None), uiCurrentResources(0), uiMaxResources(0), bHasChanges(true)
	{}

	GeneralResourceInfo(GameResourceType gr, size_t currentResources, size_t maxResources)
		: m_res_type(gr), uiCurrentResources(currentResources), uiMaxResources(maxResources), bHasChanges(true)
	{}

	size_t				Serialize(char* buf_end, size_t size) const;
	size_t				Deserialize(char* buf_end);

	static size_t		NeededSize();
};

//////////////////////////////////////////////////////////////////////////

typedef std::vector<std::shared_ptr<GameResourceContainer>> TResourceContainers;
#endif