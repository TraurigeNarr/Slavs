#ifndef SGameContext_h
#define SGameContext_h
//server
#include "SlavsServerAPI.h"

#include "SGameObject.h"
#include "SStaticObjCom.h"
#include "SDynamicObjCom.h"

//common
#include <Game/Enumerations.h>
#include <Game/GameResources.h>
#include <Game/IGameContext.h>
#include <Game/TimeController.h>
//output
#include <OutputManager.h>
//standard
#include <list>
#include <map>
#include <vector>

class TiXmlElement;
class IController;
/*
-------------------------------------------------------
This class is the main class in the application.
It defined an IDs of new objects on the scene.
-------------------------------------------------------
*/
class GameObjectState;
class Vector2D;

typedef std::map<long, SGameObject*>		ObjectsMap;
typedef std::map<GameResourceType, size_t>	ResourceMap;
typedef std::list<SStaticObjCom* const>		Obstacles;
typedef std::list<SDynamicObjCom* const>	DynamicObjects;

struct ObjectConfiguration
{
	const TiXmlElement* ConfigElement;
	ResourceMap NeededResources;
  ~ObjectConfiguration(){delete ConfigElement;}
};

struct ResourceConfigs
{
  GameResourceType m_type;
  size_t m_price;
  float m_calories;
};

class SLAVS_SERVER_EXPORT SGameContext : public IGameContext<SGameObject>, public TickListener
{
public:
												SGameContext(const std::string& mapName);
												~SGameContext();

	void										Init();
	void										TickPerformed();
  virtual void            ReleaseContext() override;

  //call this function before destruction of the object
  void                    WriteData();
  OutputManager&          GetOutputManager();
	//returns the object that was created or NULL
	//@param otype - what type context should create
	//@param owner - if this parameter is not NULL then controller becomes the owner of the object
	//@param qm	   - if qm != QM_ALL, then query mask of the object becomes qm
	SGameObject*						AddObject(int otype, const Vector2D &position, IController* owner = NULL, QueryMask qm = QM_All);
	void										RemoveObject(SGameObject* gameObject);

	void										AddSpawn(const Vector2D& spawn);
	void										AddObstacle(SStaticObjCom *const obstacle);
	void										AddDynamicObject(SDynamicObjCom *const dynamicObj);

	void										InitObjectsMap();
	void										GiveInitialObjects(IController* controller);
	ResourceMap *const			GetNeededResources(int oType);
	const Obstacles&				GetObstacles() const { return m_vObstacles;}
	const DynamicObjects&		GetDynamicObjects() const { return m_vDynamicObjects; }
  bool                    GetResConfigcs(GameResourceType i_type, ResourceConfigs& o_configs) const;

	void										TagObstaclesWithinViewRange(SDynamicObjCom *const pVehicle, float radius);
	void										TagDynamicObjectsWithinViewRange(SDynamicObjCom *const pVehicle, float radius);
protected:
  void                    GetResourcesConfigs(const TiXmlElement* i_config_element);
	void										RemoveReferences(SGameObject* gameObject);
	void										GetInitialSceneObjects();
	void										ClearData();
	void										GetNeededResources(const TiXmlElement* element, ObjectConfiguration* objInfo);


  OutputManager                     m_writer;
	std::vector<Vector2D*>*						m_vSpawns;
  //initial -> to other class
	std::vector<int>						m_vInitialObjects;
  std::map<GameResourceType, size_t>	        m_mInitialResources;
  
  //suitable in MetaFactory
	std::map<int, ObjectConfiguration*>	m_mObjectsInformation;
  

  std::map<GameResourceType, ResourceConfigs> m_resources_configs;
	
  //cash the information about obstacles
	Obstacles									m_vObstacles;
	//cash the information about dynamic objects
	DynamicObjects						m_vDynamicObjects;

  std::vector<SGameObject*> m_dead_pool;
private:
	static long									lID;
};

#endif