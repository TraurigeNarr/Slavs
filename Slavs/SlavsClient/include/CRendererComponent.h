#ifndef CRendererComponent_h
#define CRendererComponent_h

#include "OgreMaxModel.h"
#include "Game/IComponent.h"

#include <OgreNode.h>
#include <string>
#include <map>

class CGameObject;
class TiXmlElement;
class Telegram;

class CRendererComponent : public IComponent
{
  struct BillboardData
  {
    std::string material_name;
    size_t stacks;
    size_t slices;
    size_t uv_index;
  };
public:
	CRendererComponent(CGameObject *owner, const TiXmlElement *modelElement);
	~CRendererComponent();

	static void				ClearModelMap();

	CGameObject*			GetOwner() const { return m_pOwner; }
  //initialise billboard set with i_material and constructs cells of textures from one in i_material
  //if billboard exists do nothing
  void              AddBillboardSet(const std::string& i_material, Ogre::uchar i_stacks, Ogre::uchar i_slices);
  //what texture from billboard set is to use
  void              SetBillboard(Ogre::uchar i_uv_index);
	void							ApplyState(const GameObjectState& newState);
	void							ChangeModel(const TiXmlElement *modelElement);

	bool							HandleMessage(const Telegram& msg);

	Ogre::SceneNode&	GetRootSceneNode() const;
protected:
	typedef std::map<const std::string, OgreMax::OgreMaxModel*> ModelMap;
	/* Gets the collection of all model templates, each of which is keyed by file name */
	//const ModelMap& GetModelMap() const;

	/* Gets the model template with the specified file name, loading it if it isn't already loaded */
	static OgreMax::OgreMaxModel*	GetModel(const std::string& fileName, const std::string& resourceGroupName);

	void							ParseElelement(const TiXmlElement *modelElement);
private:
	CGameObject*				m_pOwner;

	Ogre::SceneNode*		m_Node;
  Ogre::BillboardSet* mp_billboard_set;  
  BillboardData       m_billboard_data;
	/*
  A lookup table for all the models that have been loaded
  */
  static ModelMap					m_ModelMap;
};

#endif