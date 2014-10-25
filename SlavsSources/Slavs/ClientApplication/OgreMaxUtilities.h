#pragma once

#include "OgreMaxTypes.h"

#include <OgreString.h>
#include <OgreHardwareBuffer.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include <boost\utility.hpp>

class TiXmlElement;

namespace OgreMax
  {
  class ClientUtilities : boost::noncopyable
    {
    public:
      ClientUtilities(){}
      ~ClientUtilities(){}

      static Ogre::Vector3								              LoadXYZ(const TiXmlElement* objectElement);
      static Ogre::Quaternion								            LoadRotation(const TiXmlElement* objectElement);

      static Types::NodeVisibility						          ParseNodeVisibility(const Ogre::String& visibility);
      static Ogre::SkeletonAnimationBlendMode			      ParseSkeletonAnimationBlendMode(const Ogre::String& mode);
      static Ogre::HardwareBuffer::Usage					      ParseHardwareBufferUsage(const Ogre::String& usage);

      static Ogre::Animation::InterpolationMode			    ParseAnimationInterpolationMode(const Ogre::String& mode);
      static Ogre::Animation::RotationInterpolationMode	ParseAnimationRotationInterpolationMode(const Ogre::String& mode);
      static Ogre::uint8									              ParseRenderQueue(const Ogre::String& renderQueue);

      static Types::NodeVisibility		                  GetNodeVisibilityAttribute(const TiXmlElement* xmlElement, const char* name);
      static ObjectVisibility					                  GetObjectVisibilityAttribute(const TiXmlElement* xmlElement, const char* name);

      static void											                  LoadBufferUsage(const TiXmlElement* objectElement, Ogre::HardwareBuffer::Usage& usage, bool& shadowed);
      static void											                  LoadSubentities(const TiXmlElement* objectElement, std::vector<Types::EntityParameters::Subentity>& subentities);
      static void											                  LoadManualLods(const TiXmlElement* objectElement, std::vector<Types::EntityParameters::ManualLod>& manualLods);
      /**
      * Sets the node's initial state (position/rotation/scale) to identity while
      * preserving the current state. This is used when setting up node animations
      */
      static void											                  SetIdentityInitialState(Ogre::SceneNode* node);
      static void											                  SetSubentities(Ogre::Entity* entity, const std::vector<Types::EntityParameters::Subentity>& subentities);
      static void											                  SetManualLods(Ogre::Entity* entity, const std::vector<Types::EntityParameters::ManualLod>& manualLods);
      static void											                  SetNodeVisibility(Ogre::SceneNode* node, Types::NodeVisibility visibility);
    };

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
  } // OgreMax