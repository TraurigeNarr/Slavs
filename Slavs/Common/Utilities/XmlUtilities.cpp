#include "XmlUtilities.h"
#include "StringUtilities.h"

#include <fstream>

///////////////////////////////////////////////////

XmlUtilities::XmlUtilities()
{
}

XmlUtilities::~XmlUtilities()
{
}

bool XmlUtilities::LoadXmlDocument(const std::string& fileName, TiXmlDocument& document)
{
	std::fstream f;
	f.open(fileName.c_str());

	if(!f.good())
		return false;
	document.Parse(StringUtilities::GetAsString(f).c_str());
	f.close();
	if(document.Error())
	{
		return false;
	}
	return true;
}

const TiXmlElement* XmlUtilities::IterateChildElements(const TiXmlElement* i_parent_element, const TiXmlElement* i_child_element)
{
  if (i_parent_element != 0)
  {
	  if (i_child_element == nullptr)
		  i_child_element = i_parent_element->FirstChildElement();
	  else
		  i_child_element = i_child_element->NextSiblingElement();

	  return i_child_element;
  }

  return nullptr;
}

std::pair<double, double> XmlUtilities::GetXY(const TiXmlElement* i_xml_element)
{
	std::pair<double, double> xy;
  xy.first = GetRealAttribute(i_xml_element, "x", 0.0);
	xy.second = GetRealAttribute(i_xml_element, "y", 0.0);

	return xy;
}

float XmlUtilities::GetRealAttribute(const TiXmlElement* i_xml_element, const char* i_name, float i_default_value)
{
	std::string value = GetStringAttribute(i_xml_element, i_name);
	return value.empty() ? i_default_value : static_cast<float>(atof(value.c_str()));
}

std::string XmlUtilities::GetStringAttribute(const TiXmlElement* i_xml_element, const char* i_name)
{
	const char* value = i_xml_element->Attribute(i_name);
	if (value != 0)
		return value;
	else
		return std::string();	
}

std::string XmlUtilities::GetStringAttribute(const TiXmlElement* i_xml_element, const char* i_name, const char* i_default_value)
{
	std::string value = GetStringAttribute(i_xml_element, i_name);
	return value.empty() ? i_default_value : value;
}

int XmlUtilities::GetIntAttribute(const TiXmlElement* i_xml_element, const char* i_name, int i_default_value)
{
	std::string value = GetStringAttribute(i_xml_element, i_name);
	return value.empty() ? i_default_value : atoi(value.c_str());
}

bool XmlUtilities::GetBoolAttribute(const TiXmlElement* i_xml_element, const char* i_name, bool i_default_value)
{
	std::string value = GetStringAttribute(i_xml_element, i_name);
	if(!value.empty())
		return value == "true" ? true : false;
	return i_default_value;
}

size_t XmlUtilities::GetElementCount(const TiXmlElement* i_xml_element, const std::string& i_element_name)
{
	size_t count = 0;

	//Check name
	if (i_element_name == i_xml_element->Value())
		count++;

	//Recurse into children
	const TiXmlElement* childElement = 0;
	while ((childElement = IterateChildElements(i_xml_element, childElement)))
		count += GetElementCount(childElement, i_element_name);

	return count;
}

size_t XmlUtilities::GetChildElementCount(const TiXmlElement* i_xml_element, const std::string& i_element_name)
{
	size_t count = 0;

	//Check children
	const TiXmlElement* childElement = 0;
	while ((childElement = IterateChildElements(i_xml_element, childElement)))
	{
		if (i_element_name == childElement->Value())
			count++;
	}

	return count;
}