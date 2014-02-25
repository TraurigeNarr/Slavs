#pragma once
#include "tinyxml.h"
#include "tinystr.h"

#include <boost\utility.hpp>

class XmlUtilities : boost::noncopyable
{
private:
  XmlUtilities();
  ~XmlUtilities();

public:
  static bool					              LoadXmlDocument(const std::string& i_file_name, TiXmlDocument& io_document);
  static const TiXmlElement*	      IterateChildElements(const TiXmlElement* i_parent_element, const TiXmlElement* i_child_element);

	static std::pair<float, float>    GetXY(const TiXmlElement* i_xml_element);
  static float				              GetRealAttribute(const TiXmlElement* i_xml_element, const char* i_name, float i_default_value = .0f);
	static std::string	              GetStringAttribute(const TiXmlElement* i_xml_element, const char* i_name);
	static std::string	              GetStringAttribute(const TiXmlElement* i_xml_element, const char* i_name, const char* i_default_value);
	static int					              GetIntAttribute(const TiXmlElement* i_xml_element, const char* i_name, int i_default_value);
	static bool					              GetBoolAttribute(const TiXmlElement* i_xml_element, const char* i_name, bool i_default_value);
  /*
	* Recursively searches the XML element tree, counting elements with the specified i_name
	* @param element [in] - The start element. This element is included in the count
	* @param elementName [in] - The element i_name to search for
	* @return Returns the number of elements with the specified i_name
	*/
	static size_t                     GetElementCount(const TiXmlElement* i_xml_element, const std::string& i_element_name);
	static size_t                     GetChildElementCount(const TiXmlElement* i_xml_element, const std::string& i_element_name);
};