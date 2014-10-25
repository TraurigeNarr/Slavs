#include "GameSerializer.h"

#include "Game/GameContext.h"
#include "Game/GameObject.h"

#include <Main/ServerMain.h>

#include <tinyxml.h>

#include <Utilities/XmlUtilities.h>
#include <Math/Vector2D.h>


#include <boost/lexical_cast.hpp>

//////////////////////////////////////////////////////////////////////////

namespace
  {

  Slavs::TGameObject _CreateObject(Slavs::GameContext& o_context, const TiXmlElement& i_xml_node, int i_type)
    {
    const TiXmlElement* p_child = 0;
    std::string node_name = "";

    const TiXmlNode* p_owner = i_xml_node.FirstChild("Owner");
    const TiXmlNode* p_position = i_xml_node.FirstChild("Position");
    const TiXmlNode* p_selection_mask = i_xml_node.FirstChild("SelectionMask");

#ifdef _DEBUG
    assert (p_selection_mask->FirstChild());
    assert (p_position->FirstChild("x"));
    assert (p_position->FirstChild("y"));
#endif

    int controller_id = 0;
    try
      {
      controller_id = p_owner == nullptr ? 0 : boost::lexical_cast<int>(p_owner->FirstChild("Address")->FirstChild()->Value());
      }
    catch (boost::bad_lexical_cast&)
      {      }
    
    //position -- x and y coordinates
    float pos_x = boost::lexical_cast<float>(p_position->FirstChild("x")->FirstChild()->Value());
    float pos_y = boost::lexical_cast<float>(p_position->FirstChild("y")->FirstChild()->Value());

    //selection mask -- int
    int selection_mask                    = boost::lexical_cast<int>(p_selection_mask->FirstChild()->Value());
    
    return o_context.AddObject(i_type, Vector2D(pos_x, pos_y), controller_id, selection_mask);
    }

  GameResourceType GetResourceType(const std::string& i_resource_name)
    {
    if (i_resource_name == "tree")
      return GameResourceType::GR_Tree;
    if (i_resource_name == "stone")
      return GameResourceType::GR_Stone;
    if (i_resource_name == "wood")
      return GameResourceType::GR_Wood;
    if (i_resource_name == "iron_ore")
      return GameResourceType::GR_IronOre;
    if (i_resource_name == "wool")
      return GameResourceType::GR_Wool;
    if (i_resource_name == "bread")
      return GameResourceType::GR_Bread;
    return GameResourceType::GR_None;
    }

  void GetResourcesSetForType(Slavs::ResourcesCountSet& o_resources, const TiXmlElement& i_xml_node)
    {
    const TiXmlElement* p_child = nullptr;
    std::string resource_name = "";
    GameResourceType res_type = GameResourceType::GR_None;
    int count = 0;
    while (p_child = XmlUtilities::IterateChildElements(&i_xml_node, p_child))
      {
      resource_name = p_child->Value();
      res_type = GetResourceType(resource_name);
      if (res_type != GameResourceType::GR_None)
        {
        count = XmlUtilities::GetIntAttribute(p_child, "value", 0);
        o_resources.push_back(std::make_pair(static_cast<int>(res_type), count));
        }
      }
    }

  } // namespace

namespace Slavs
  {
  GameSerializer::GameSerializer(GameContext& i_game_context)
    : m_game_context(i_game_context)
    {    }

  GameSerializer::~GameSerializer()
    {    }

  void GameSerializer::LoadGame(const std::string& i_game_name)
    {
    TiXmlDocument document;
    if (!XmlUtilities::LoadXmlDocument(i_game_name, document))
      {
      //write to log about failure
      return;
      }
    else
      ParseGameDocument(*document.RootElement());
    }
  
  void GameSerializer::LoadConfigurations(const std::string& i_configurations_path)
    {
    TiXmlDocument document;
    if (!XmlUtilities::LoadXmlDocument(i_configurations_path, document))
      {
      //write to log about failure
      return;
      }
    else
      ParseConfigurationDocument(*document.RootElement());
    }

  void GameSerializer::ParseGameDocument(TiXmlElement& i_root)
    {
    const TiXmlElement* p_child = 0;
    std::string type_name = "";

    MetaFactory& factory = ServerMain::GetInstance().GetMetaFactory();

    while ((p_child = XmlUtilities::IterateChildElements(&i_root, p_child)))
      {
      type_name = p_child->Value();
      int type_id = factory.GetObjectID(type_name);
      if (type_id != -1)
        {
        _CreateObject(m_game_context, *p_child, type_id);
        }
      }
    }

  void GameSerializer::ParseConfigurationDocument(TiXmlElement& i_root)
    {
    const TiXmlElement* p_child = 0;
    std::string type_name = "";

    MetaFactory& factory = ServerMain::GetInstance().GetMetaFactory();

    while ((p_child = XmlUtilities::IterateChildElements(&i_root, p_child)))
      {
      type_name = p_child->Value();
      int type_id = factory.GetObjectID(type_name);
      if (type_id != -1)
        {
        Slavs::ResourcesCountSet resources_set;
        GetResourcesSetForType(resources_set, *p_child);
        m_game_context.RegisterResources(type_id, resources_set);
        }

      }
    }
  }