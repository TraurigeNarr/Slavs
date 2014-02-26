#include "GameSerializer.h"

#include "GameContext.h"
#include "GameObject.h"

#include <ServerMain.h>

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
    assert (p_owner);
    assert (p_owner->FirstChild("Name"));
    assert (p_owner->FirstChild("Address"));
    assert (p_selection_mask->FirstChild());
#endif
    //owner -- should be combination of name + address; can be null
    std::string owner_name                = p_owner->FirstChild("Name")->FirstChild()->Value();
    int address                           = boost::lexical_cast<int>(p_owner->FirstChild("Address")->FirstChild()->Value());
    //position -- x and y coordinates
    float pos_x = 0.f;
    float pos_y = 0.f;
    //selection mask -- int
    int selection_mask                    = boost::lexical_cast<int>(p_selection_mask->FirstChild()->Value());
    
    return o_context.AddObject(i_type, Vector2D(pos_x, pos_y), nullptr, selection_mask);
    }
  }

namespace Slavs
  {
  GameSerializer::GameSerializer(GameContext& i_game_context)
    : m_game_context(i_game_context)
    {    }

  GameSerializer::~GameSerializer()
    {    }

  void GameSerializer::Load(const std::string& i_game_name)
    {
    TiXmlDocument document;
    if (!XmlUtilities::LoadXmlDocument(i_game_name, document))
      {
      //write to log about failure
      return;
      }
    else
      Parse(*document.RootElement());
    }

  void GameSerializer::Parse(TiXmlElement& i_root)
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
  }