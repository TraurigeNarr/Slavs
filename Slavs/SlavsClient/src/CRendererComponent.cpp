#include "CRendererComponent.h"
//client
#include "CGameObject.h"
#include "ClientUtilities.h"
#include "OgreFramework.h"
#include "OgreMaxModel.h"
//common
#include <Game/GameObjectState.h>
#include <Math/Vector2D.h>
#include <Patterns/Telegram.h>
#include <Patterns/Singleton.h>
#include <Utilities/XmlUtilities.h>
#include <Utilities/FileUtilities.h>
//standard
#include <algorithm>
#include <cassert>

CRendererComponent::ModelMap CRendererComponent::m_ModelMap;


CRendererComponent::CRendererComponent(CGameObject *owner, const TiXmlElement *modelElement)
	: m_pOwner(owner), m_Node(nullptr), mp_billboard_set(nullptr)
{
	ParseElelement(modelElement);
}

CRendererComponent::~CRendererComponent()
{
	m_Node->removeAndDestroyAllChildren();
	Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->destroySceneNode(m_Node);
	m_Node = NULL;
}

OgreMax::OgreMaxModel* CRendererComponent::GetModel(const Ogre::String& fileName, const Ogre::String& resourceGroupName)
{
	//Look up item and return it if found
	ModelMap::iterator item = m_ModelMap.find(fileName);
	if (item != m_ModelMap.end())
		return item->second;

	//Model couldn't be found, so load it
	OgreMax::OgreMaxModel* model = new OgreMax::OgreMaxModel;
	try
	{
		model->Load(fileName, resourceGroupName);
	}
	catch (...)
	{
		delete model;
		throw;
	}

	//If we're here, the model was loaded. Add it to the lookup
	m_ModelMap[fileName] = model;

	return model;
}

void CRendererComponent::ClearModelMap()
{
	std::for_each(m_ModelMap.begin(), m_ModelMap.end(), [](std::pair<Ogre::String, OgreMax::OgreMaxModel*> p)
	{
		delete p.second;
	});
	m_ModelMap.clear();
}

void CRendererComponent::ParseElelement(const TiXmlElement *componentElement)
{
	const TiXmlElement* childElement = NULL;
	std::string elementName = "";
	while (childElement = XmlUtilities::IterateChildElements(componentElement, childElement))
	{
		elementName = childElement->Value();
		if("model" == elementName)
		{
			std::string location = FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), XmlUtilities::GetStringAttribute(childElement, "location", ""));
			std::string resourceGroup = XmlUtilities::GetStringAttribute(childElement, "group", "Characters");
			std::string resourceType = XmlUtilities::GetStringAttribute(childElement, "type", Types::FileSystem.c_str());
			std::string fileName = XmlUtilities::GetStringAttribute(childElement, "file", "");

			std::string fullPath = FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), location);

			fullPath = FileUtilities::JoinPath(fullPath, fileName);
			//CRendererComponent::addResourceLocation(location, resourceGroup, resourceType);

			OgreMax::OgreMaxModel *model = CRendererComponent::GetModel(fullPath, resourceGroup);

			if (model != 0)
			{
				OgreMax::OgreMaxModel::InstanceOptions options;
				options |= OgreMax::OgreMaxModel::NO_ANIMATION_STATES;
				//Create the instance
				m_Node = model->CreateInstance(
					Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager,
					(QueryMask)m_pOwner->GetQueryMask(),
					NULL,
					options,
					Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->getRootSceneNode(),
					resourceGroup
					);
				m_Node->setInheritScale(true);
				m_Node->setVisible(true);
				m_Node->setUserAny(Ogre::Any(m_pOwner));
			}
		}
		if("scale" == elementName)
			m_Node->setScale(ClientUtilities::LoadXYZ(childElement));
	}
}

void CRendererComponent::AddBillboardSet(const std::string& i_material, Ogre::uchar i_stacks, Ogre::uchar i_slices)
{
  assert(m_Node != nullptr);
  
  if(nullptr != mp_billboard_set)
    return;

  m_billboard_data.material_name = i_material;
  m_billboard_data.stacks = i_stacks;
  m_billboard_data.slices = i_slices;
  m_billboard_data.uv_index = 0;

  mp_billboard_set = Singleton<OgreFramework>::GetInstance().m_pSceneManager->createBillboardSet(1);
  mp_billboard_set->setMaterialName(i_material);
  mp_billboard_set->setDefaultDimensions(75, 5);//m_Node->_getWorldAABB().getMaximum().x, m_Node->_getWorldAABB().getMaximum().y*0.2);
  mp_billboard_set->setTextureStacksAndSlices(i_stacks, i_slices);
  mp_billboard_set->createBillboard(0, 75, 5);

  m_Node->attachObject(mp_billboard_set);
}

void CRendererComponent::SetBillboard(Ogre::uchar i_uv_index)
{
  assert(mp_billboard_set != nullptr && m_Node != nullptr);

  mp_billboard_set->getBillboard(0)->setTexcoordIndex(i_uv_index);
  m_billboard_data.uv_index = i_uv_index;
}

bool CRendererComponent::HandleMessage(const Telegram& msg)
{
	return true;
}

Ogre::SceneNode& CRendererComponent::GetRootSceneNode() const
{
	assert(m_Node);
	return *m_Node;
}

void CRendererComponent::ApplyState(const GameObjectState& newState)
{
	if(NULL != newState.pPosition)
		m_Node->setPosition(newState.pPosition->x, 0, newState.pPosition->y);
}

void CRendererComponent::ChangeModel(const TiXmlElement *modelElement)
{ 
  m_Node->removeAndDestroyAllChildren();
	Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->destroySceneNode(m_Node);
	ParseElelement(modelElement);

  if(nullptr != mp_billboard_set)
  {
    mp_billboard_set = nullptr;
    AddBillboardSet(m_billboard_data.material_name,m_billboard_data.stacks, m_billboard_data.slices);
    SetBillboard(m_billboard_data.uv_index);
  }
}