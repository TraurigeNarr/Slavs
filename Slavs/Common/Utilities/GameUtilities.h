#pragma once
#include "..\Game\Enumerations.h"
#include "tinyxml.h"
#include "tinystr.h"

#include <boost\utility.hpp>

class GameUtilities : boost::noncopyable
{
private:
  GameUtilities();
  ~GameUtilities();

public:
  template <class T>
  static void SetObjectVisibility(T object, ObjectVisibility visibility)
  {
    switch (visibility)
    {
    case OBJECT_VISIBLE: object->setVisible(true); break;
    case  OBJECT_HIDDEN: object->setVisible(false); break;
    default: break;
    }
  }
  static ObjectVisibility		ParseObjectVisibility(const std::string& visibility);
  static ObjectVisibility		GetObjectVisibilityAttribute(const TiXmlElement* i_xml_element, const char* i_name);
};