#include "GameUtilities.h"
#include "StringUtilities.h"
#include "XmlUtilities.h"

GameUtilities::GameUtilities()
{
}

GameUtilities::~GameUtilities()
{
}

ObjectVisibility GameUtilities::ParseObjectVisibility(const std::string& visibility)
{
  if (visibility.empty())
    return OBJECT_VISIBILITY_DEFAULT;
  else
    return StringUtilities::ParseBool(visibility) ? OBJECT_VISIBLE : OBJECT_HIDDEN;
}

ObjectVisibility GameUtilities::GetObjectVisibilityAttribute(const TiXmlElement* xmlElement, const char* name)
{
  std::string visibility = XmlUtilities::GetStringAttribute(xmlElement, name);
  return ParseObjectVisibility(visibility);
}