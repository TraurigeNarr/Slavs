#include "misc\SceneLoader.h"
//server
#include "SGameContext.h"
//common
#include <Game/IGameObject.h>
#include <Math/Vector2D.h>
#include <Utilities/XmlUtilities.h>


SceneLoader::SceneLoader(SGameContext* context, std::ostream *stream /* = NULL */)
	: m_pGameContext(context), m_pLogStream(stream)
{}

SceneLoader::~SceneLoader()
{}

void SceneLoader::LoadScene(std::string sceneName)
{
	//open document
	TiXmlDocument document;
	if(!XmlUtilities::LoadXmlDocument(sceneName, document))
		WriteToStream("Loading scene file crashed.");
	else
	{
		Parse(document.RootElement());
	}
}

void SceneLoader::Parse(TiXmlElement *rootElement)
{
	const TiXmlElement* childElement = 0;
	std::string elementName;
	while ((childElement = XmlUtilities::IterateChildElements(rootElement, childElement)))
	{
		elementName = childElement->Value();
		if ("GameObjects" == elementName)
			ParseObject(childElement);
	}
}

void SceneLoader::ParseObject(const TiXmlElement* objectElement)
{
	std::string elementName;
	const TiXmlElement* childElement = 0;
	while ((childElement = XmlUtilities::IterateChildElements(objectElement, childElement)))
	{
		elementName = childElement->Value();

		std::string type = XmlUtilities::GetStringAttribute(childElement, "type", "");
		int otype = IGameObject::GetGameObjectType(type);

		if(OT_None == otype)
		{
			std::string s = childElement->Attribute("name");
			WriteToStream("Error in loading node: " + s);
			continue;
		}

		const TiXmlElement *sceneElement = NULL;

		Vector2D position;
		while (sceneElement = XmlUtilities::IterateChildElements(childElement, sceneElement))
		{
			elementName = sceneElement->Value();

			if("position" == elementName)
			{
        std::pair<double, double> xy_pair = XmlUtilities::GetXY(sceneElement);
				position.x = xy_pair.first;
        position.y = xy_pair.second;
				//position = new object on scene
				switch(otype)
				{
				case OT_Spawn:
					m_pGameContext->AddSpawn(position);
					break;
				case OT_Terrain:
				case OT_Tree:
				case OT_GoldMine:
					m_pGameContext->AddObject(otype, position);
					break;
				}
			}
		}
	}
}

void SceneLoader::WriteToStream(const std::string& message) const
{
	if(NULL != m_pLogStream)
		(*m_pLogStream) << message;
}