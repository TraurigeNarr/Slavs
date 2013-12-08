#include "ClientUtilities.h"

#include <Utilities/XmlUtilities.h>
#include <Utilities/StringUtilities.h>
#include <Utilities/GameUtilities.h>

using namespace Types;

CEGUI::MouseButton ClientUtilities::ConvertButton(OIS::MouseButtonID buttonID)
{
	switch (buttonID)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;
		break;

	case OIS::MB_Right:
		return CEGUI::RightButton;
		break;

	case OIS::MB_Middle:
		return CEGUI::MiddleButton;
		break;

	default:
		return CEGUI::LeftButton;
		break;
	}
}

Ogre::Vector3 ClientUtilities::LoadXYZ(const TiXmlElement* objectElement)
{
	Ogre::Vector3 xyz;
	xyz.x = XmlUtilities::GetRealAttribute(objectElement, "x", 0);
	xyz.y = XmlUtilities::GetRealAttribute(objectElement, "y", 0);
	xyz.z = XmlUtilities::GetRealAttribute(objectElement, "z", 0);

	return xyz;
}

Ogre::Quaternion ClientUtilities::LoadRotation(const TiXmlElement* objectElement)
{
	Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;

	if (objectElement->Attribute("qx") != 0)
	{
		//The rotation is specified as a quaternion
		rotation.x = XmlUtilities::GetRealAttribute(objectElement, "qx", 0);
		rotation.y = XmlUtilities::GetRealAttribute(objectElement, "qy", 0);
		rotation.z = XmlUtilities::GetRealAttribute(objectElement, "qz", 0);
		rotation.w = XmlUtilities::GetRealAttribute(objectElement, "qw", 0);

		if (rotation == Ogre::Quaternion::ZERO)
			rotation = Ogre::Quaternion::IDENTITY;
	}
	else if (objectElement->Attribute("axisX") != 0)
	{
		//The rotation is specified as an axis and angle
		Ogre::Real angle = XmlUtilities::GetRealAttribute(objectElement, "angle", 0);

		Ogre::Vector3 axis;
		axis.x = XmlUtilities::GetRealAttribute(objectElement, "axisX", 0);
		axis.y = XmlUtilities::GetRealAttribute(objectElement, "axisY", 0);
		axis.z = XmlUtilities::GetRealAttribute(objectElement, "axisZ", 0);

		//Convert the angle and axis into the rotation quaternion
		rotation.FromAngleAxis(Ogre::Radian(angle), axis);
	}
	else if (objectElement->Attribute("angleX") != 0)
	{
		//Assume the rotation is specified as three Euler angles
		Ogre::Vector3 euler;
		euler.x = XmlUtilities::GetRealAttribute(objectElement, "angleX", 0);
		euler.y = XmlUtilities::GetRealAttribute(objectElement, "angleY", 0);
		euler.z = XmlUtilities::GetRealAttribute(objectElement, "angleZ", 0);
		Ogre::String order = XmlUtilities::GetStringAttribute(objectElement, "order");

		//Convert Euler angles to a matrix
		Ogre::Matrix3 rotationMatrix;
		if (order.length() < 2)
			rotationMatrix.FromEulerAnglesXYZ(Ogre::Radian(euler.x), Ogre::Radian(euler.y), Ogre::Radian(euler.z));
		else
		{
			if (order[0] == 'x')
			{
				if (order[1] == 'y')
					rotationMatrix.FromEulerAnglesXYZ(Ogre::Radian(euler.x), Ogre::Radian(euler.y), Ogre::Radian(euler.z));
				else
					rotationMatrix.FromEulerAnglesXZY(Ogre::Radian(euler.x), Ogre::Radian(euler.y), Ogre::Radian(euler.z));
			}
			else if (order[0] == 'y')
			{
				if (order[1] == 'x')
					rotationMatrix.FromEulerAnglesYXZ(Ogre::Radian(euler.x), Ogre::Radian(euler.y), Ogre::Radian(euler.z));
				else
					rotationMatrix.FromEulerAnglesYZX(Ogre::Radian(euler.x), Ogre::Radian(euler.y), Ogre::Radian(euler.z));
			}
			else
			{
				if (order[1] == 'x')
					rotationMatrix.FromEulerAnglesZXY(Ogre::Radian(euler.x), Ogre::Radian(euler.y), Ogre::Radian(euler.z));
				else
					rotationMatrix.FromEulerAnglesZYX(Ogre::Radian(euler.x), Ogre::Radian(euler.y), Ogre::Radian(euler.z));
			}
		}

		//Convert the matrix into the rotation quaternion
		rotation.FromRotationMatrix(rotationMatrix);
	}

	return rotation;
}

NodeVisibility ClientUtilities::ParseNodeVisibility(const Ogre::String& visibility)
{
	Ogre::String visibilityLower = visibility;
	Ogre::StringUtil::toLowerCase(visibilityLower);

	if (visibilityLower == "visible")
		return NODE_VISIBLE;
	else if (visibilityLower == "hidden")
		return NODE_HIDDEN;
	else if (visibilityLower == "treevisible")
		return NODE_TREE_VISIBLE;
	else if (visibilityLower == "treehidden")
		return NODE_TREE_HIDDEN;
	else
		return NODE_VISIBILITY_DEFAULT;
}

Ogre::SkeletonAnimationBlendMode ClientUtilities::ParseSkeletonAnimationBlendMode(const Ogre::String& mode)
{
	Ogre::String modeLower = mode;
	Ogre::StringUtil::toLowerCase(modeLower);

	if (modeLower.empty() || modeLower == "average")
		return Ogre::ANIMBLEND_AVERAGE;
	else if (modeLower == "cumulative")
		return Ogre::ANIMBLEND_CUMULATIVE;

	Ogre::StringUtil::StrStreamType errorMessage;
	errorMessage << "Invalid skeleton animation blend mode specified: " << mode;

	OGRE_EXCEPT
		(
		Ogre::Exception::ERR_INVALIDPARAMS,
		errorMessage.str(),
		"OgreMaxUtilities::ParseSkeletonAnimationBlendMode"
		);
}

Ogre::HardwareBuffer::Usage ClientUtilities::ParseHardwareBufferUsage(const Ogre::String& usage)
{
	Ogre::String usageLower = usage;
	Ogre::StringUtil::toLowerCase(usageLower);

	if (usageLower == "static")
		return Ogre::HardwareBuffer::HBU_STATIC;
	else if (usageLower == "dynamic")
		return Ogre::HardwareBuffer::HBU_DYNAMIC;
	else if (usageLower == "writeonly")
		return Ogre::HardwareBuffer::HBU_WRITE_ONLY;
	else if (usageLower == "staticwriteonly")
		return Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY;
	else if (usageLower == "dynamicwriteonly")
		return Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY;

	Ogre::StringUtil::StrStreamType errorMessage;
	errorMessage << "Invalid hardware buffer usage specified: " << usage;

	OGRE_EXCEPT
		(
		Ogre::Exception::ERR_INVALIDPARAMS,
		errorMessage.str(),
		"OgreMaxUtilities::ParseHardwareBufferUsage"
		);
}

Ogre::Animation::InterpolationMode ClientUtilities::ParseAnimationInterpolationMode(const Ogre::String& mode)
{
	Ogre::String modeLower = mode;
	Ogre::StringUtil::toLowerCase(modeLower);

	if (modeLower == "linear")
		return Ogre::Animation::IM_LINEAR;
	else if (modeLower == "spline")
		return Ogre::Animation::IM_SPLINE;

	Ogre::StringUtil::StrStreamType errorMessage;
	errorMessage << "Invalid animation interpolation mode specified: " << mode;

	OGRE_EXCEPT
		(
		Ogre::Exception::ERR_INVALIDPARAMS,
		errorMessage.str(),
		"OgreMaxUtilities::ParseAnimationInterpolationMode"
		);
}

Ogre::Animation::RotationInterpolationMode ClientUtilities::ParseAnimationRotationInterpolationMode(const Ogre::String& mode)
{
	Ogre::String modeLower = mode;
	Ogre::StringUtil::toLowerCase(modeLower);

	if (modeLower == "linear")
		return Ogre::Animation::RIM_LINEAR;
	else if (modeLower == "spherical")
		return Ogre::Animation::RIM_SPHERICAL;

	Ogre::StringUtil::StrStreamType errorMessage;
	errorMessage << "Invalid animation rotation interpolation mode specified: " << mode;

	OGRE_EXCEPT
		(
		Ogre::Exception::ERR_INVALIDPARAMS,
		errorMessage.str(),
		"OgreMaxUtilities::ParseAnimationRotationInterpolationMode"
		);
}

Ogre::uint8 ClientUtilities::ParseRenderQueue(const Ogre::String& renderQueue)
{
	static std::map<Ogre::String, Ogre::uint8> nameToNumber;
	if (nameToNumber.empty())
	{
		nameToNumber["background"] = Ogre::RENDER_QUEUE_BACKGROUND;
		nameToNumber["skiesearly"] = Ogre::RENDER_QUEUE_SKIES_EARLY;
		nameToNumber["queue1"] = Ogre::RENDER_QUEUE_1;
		nameToNumber["queue2"] = Ogre::RENDER_QUEUE_2;
		nameToNumber["worldgeometry1"] = Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1;
		nameToNumber["queue3"] = Ogre::RENDER_QUEUE_3;
		nameToNumber["queue4"] = Ogre::RENDER_QUEUE_4;
		nameToNumber["main"] = Ogre::RENDER_QUEUE_MAIN;
		nameToNumber["queue6"] = Ogre::RENDER_QUEUE_6;
		nameToNumber["queue7"] = Ogre::RENDER_QUEUE_7;
		nameToNumber["worldgeometry2"] = Ogre::RENDER_QUEUE_WORLD_GEOMETRY_2;
		nameToNumber["queue8"] = Ogre::RENDER_QUEUE_8;
		nameToNumber["queue9"] = Ogre::RENDER_QUEUE_9;
		nameToNumber["skieslate"] = Ogre::RENDER_QUEUE_SKIES_LATE;
		nameToNumber["overlay"] = Ogre::RENDER_QUEUE_OVERLAY;
		nameToNumber["max"] = Ogre::RENDER_QUEUE_MAX;
	}

	if (renderQueue.empty())
		return Ogre::RENDER_QUEUE_MAIN;
	else if (StringUtilities::AllDigits(renderQueue))
		return (Ogre::uint8)Ogre::StringConverter::parseUnsignedInt(renderQueue);
	else
	{
		//The render queue name, lowercase
		Ogre::String renderQueueLower;

		//Get the offset that comes after the +, if any
		Ogre::uint8 offset = 0;
		size_t plusFoundAt = renderQueue.find('+');
		if (plusFoundAt != Ogre::String::npos)
		{
			//Parse the number
			Ogre::String offsetText = renderQueue.substr(plusFoundAt + 1);
			Ogre::StringUtil::trim(offsetText);

			offset = (Ogre::uint8)Ogre::StringConverter::parseUnsignedInt(offsetText);

			//Remove the "+offset" substring from the render queue name
			renderQueueLower = renderQueue.substr(0, plusFoundAt);
			Ogre::StringUtil::trim(renderQueueLower);
		}
		else
			renderQueueLower = renderQueue;
		Ogre::StringUtil::toLowerCase(renderQueueLower);

		//Look up the render queue and return it
		std::map<Ogre::String, Ogre::uint8>::iterator item = nameToNumber.find(renderQueueLower);
		if (item != nameToNumber.end())
		{
			//Don't let the render queue exceed the maximum
			return std::min((Ogre::uint8)(item->second + offset), (Ogre::uint8)Ogre::RENDER_QUEUE_MAX);
		}
		else
		{
			Ogre::StringUtil::StrStreamType errorMessage;
			errorMessage << "Invalid render queue specified: " << renderQueue;

			OGRE_EXCEPT
				(
				Ogre::Exception::ERR_INVALIDPARAMS,
				errorMessage.str(),
				"OgreMaxUtilities::ParseRenderQueue"
				);
		}
	}
}

NodeVisibility ClientUtilities::GetNodeVisibilityAttribute(const TiXmlElement* xmlElement, const char* name)
{
	Ogre::String visibility = XmlUtilities::GetStringAttribute(xmlElement, name);
	return ParseNodeVisibility(visibility);
}

ObjectVisibility ClientUtilities::GetObjectVisibilityAttribute(const TiXmlElement* xmlElement, const char* name)
{
	Ogre::String visibility = XmlUtilities::GetStringAttribute(xmlElement, name);
	return GameUtilities::ParseObjectVisibility(visibility);
}

void ClientUtilities::LoadBufferUsage(const TiXmlElement* objectElement, Ogre::HardwareBuffer::Usage& usage, bool& shadowed)
{
	Ogre::String usageText = XmlUtilities::GetStringAttribute(objectElement, "usage");
	usage = usageText.empty() ? Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY : ParseHardwareBufferUsage(usageText);

	shadowed = XmlUtilities::GetBoolAttribute(objectElement, "useShadow", true);
}

void ClientUtilities::LoadSubentities(const TiXmlElement* objectElement, std::vector<EntityParameters::Subentity>& subentities)
{
	subentities.resize(XmlUtilities::GetElementCount(objectElement, "subentity"));

	const TiXmlElement* childElement = 0;
	while ((childElement = XmlUtilities::IterateChildElements(objectElement, childElement)))
	{
		int index = XmlUtilities::GetIntAttribute(childElement, "index", 0);
		subentities[index].materialName = XmlUtilities::GetStringAttribute(childElement, "materialName");
	}
}

void ClientUtilities::SetIdentityInitialState(Ogre::SceneNode* node)
{
	//Get the current state
	Ogre::Vector3 position = node->getPosition();
	Ogre::Quaternion orientation = node->getOrientation();
	Ogre::Vector3 scale = node->getScale();

	//Set the initial state to be at identity
	node->setPosition(Ogre::Vector3::ZERO);
	node->setOrientation(Ogre::Quaternion::IDENTITY);
	node->setScale(Ogre::Vector3::UNIT_SCALE);
	node->setInitialState();

	//Set the current state so the node is in the correct position if the node has
	//animations that are initially disabled
	node->setPosition(position);
	node->setOrientation(orientation);
	node->setScale(scale);
}

void ClientUtilities::SetSubentities(Ogre::Entity* entity, const std::vector<EntityParameters::Subentity>& subentities)
{
	size_t subentityCount = std::min(subentities.size(), (size_t)entity->getNumSubEntities());
	for (size_t subentityIndex = 0; subentityIndex < subentityCount; subentityIndex++)
	{
		Ogre::SubEntity* subentity = entity->getSubEntity((unsigned int)subentityIndex);
		if (!subentities[subentityIndex].materialName.empty())
			subentity->setMaterialName(subentities[subentityIndex].materialName);
	}
}

void ClientUtilities::LoadManualLods(const TiXmlElement* objectElement, std::vector<Types::EntityParameters::ManualLod>& manualLods)
{
	manualLods.resize(XmlUtilities::GetElementCount(objectElement, "manualLod"));

	const TiXmlElement* childElement = 0;
	while ((childElement = XmlUtilities::IterateChildElements(objectElement, childElement)))
	{
		int index = XmlUtilities::GetIntAttribute(childElement, "index", 0);

		const TiXmlElement* subentitiesElement = childElement->FirstChildElement("subentities");
		if (subentitiesElement != 0)
			LoadSubentities(subentitiesElement, manualLods[index].subentities);
	}
}

void ClientUtilities::SetManualLods(Ogre::Entity* entity, const std::vector<EntityParameters::ManualLod>& manualLods)
{
	size_t manualLodCount = std::min(manualLods.size(), (size_t)entity->getNumManualLodLevels());
	for (size_t manualLodIndex = 0; manualLodIndex < manualLodCount; manualLodIndex++)
	{
		Ogre::Entity* manualLod = entity->getManualLodLevel(manualLodIndex);
		SetSubentities(manualLod, manualLods[manualLodIndex].subentities);
	}
}

void ClientUtilities::SetNodeVisibility(Ogre::SceneNode* node, NodeVisibility visibility)
{
	switch (visibility)
	{
	case NODE_VISIBLE: node->setVisible(true, false); break;
	case NODE_HIDDEN: node->setVisible(false, false); break;
	case NODE_TREE_VISIBLE: node->setVisible(true, true); break;
	case NODE_TREE_HIDDEN: node->setVisible(false, true); break;
	default: break;
	}
}