#pragma once
/*
	All enumerations and properties structures that has influence with ogreMax
*/
#include "ObjectVisibility.h"

#include <list>
#include <map>

#include <Ogre.h>
#include <OgreAnimation.h>
#include <OgreAnimationState.h>
#include <OgreBillboardSet.h>
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreHardwareBuffer.h>
#include <OgreLight.h>
#include <OgreMaterial.h>
#include <OgrePixelFormat.h>
#include <OgrePlane.h>
#include <OgreQuaternion.h>
#include <OgreRenderQueue.h>
#include <OgreSceneNode.h>
#include <OgreSkeletonInstance.h>
#include <OgreString.h>
#include <OgreStringConverter.h>
#include <OgreTagPoint.h>
#include <OgreTexture.h>
#include <OgreVector3.h>
#include <OgreVector4.h>

using namespace OgreMax;

namespace Types
{
	const Ogre::String FileSystem = "FileSystem";
	const Ogre::String ZipArchive = "Zip";

	enum NodeVisibility
	{
		NODE_VISIBILITY_DEFAULT,
		NODE_VISIBLE,
		NODE_HIDDEN,
		NODE_TREE_VISIBLE,
		NODE_TREE_HIDDEN
	};

	

	//for OGREmax
	struct NodeAnimationParameters
	{
		NodeAnimationParameters()
		{
			this->length = 0;
			this->interpolationMode = Ogre::Animation::IM_LINEAR;
			this->rotationInterpolationMode = Ogre::Animation::RIM_LINEAR;
			this->enable = true;
			this->looping = true;
		}

		Ogre::String name;
		Ogre::Real length;
		Ogre::Animation::InterpolationMode interpolationMode;
		Ogre::Animation::RotationInterpolationMode rotationInterpolationMode;
		bool enable;
		bool looping;

		struct KeyFrame
		{
			KeyFrame()
			{
				this->time = 0;
				this->translation = Ogre::Vector3::ZERO;
				this->rotation = Ogre::Quaternion::IDENTITY;
				this->scale = Ogre::Vector3::UNIT_SCALE;
			}

			Ogre::Real time;
			Ogre::Vector3 translation;
			Ogre::Quaternion rotation;
			Ogre::Vector3 scale;
		};

		std::vector<KeyFrame> keyframes;
	};

	struct ObjectParameters
    {
        enum ObjectType
        {
            NONE,
            ENTITY,
            LIGHT,
            CAMERA,
            PARTICLE_SYSTEM,
            BILLBOARD_SET,
            PLANE
        };

        ObjectParameters()
        {
            this->objectType = NONE;
            this->renderQueue = Ogre::RENDER_QUEUE_MAIN;
            this->renderingDistance = 0;
            this->queryFlags = 0;
            this->visibilityFlags = 0;
            this->visibility = OBJECT_VISIBILITY_DEFAULT;
        }

        virtual ~ObjectParameters()
        {
        }

        /** Name of the object */
        Ogre::String name;

        /**
            * The object type.
            * This can be used to determine which ObjectParameters subclass can be used
            */
        ObjectType objectType;

        /** Object query flags */
        Ogre::uint32 queryFlags;

        /** Object visibility flags */
        Ogre::uint32 visibilityFlags;

        /** Indicates whether object is visible */
        ObjectVisibility visibility;

        /** Rendering queue. Not used by all types */
        Ogre::uint8 renderQueue;

        /** Rendering distance. Not used by all types */
        Ogre::Real renderingDistance;
    };

    struct EntityParameters : ObjectParameters
    {
        EntityParameters()
        {
            this->objectType = ENTITY;

            this->skeletonAnimationBlendMode = Ogre::ANIMBLEND_AVERAGE;

            this->castShadows = true;

            this->vertexBufferUsage = Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY;
	        this->indexBufferUsage = Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY;
	        this->vertexBufferShadowed = true;
            this->indexBufferShadowed = true;
        }

        Ogre::String meshFile;
        Ogre::String materialFile;

        Ogre::SkeletonAnimationBlendMode skeletonAnimationBlendMode;

        bool castShadows;

        Ogre::HardwareBuffer::Usage vertexBufferUsage;
	    Ogre::HardwareBuffer::Usage indexBufferUsage;
	    bool vertexBufferShadowed;
        bool indexBufferShadowed;

        Ogre::String resourceGroupName;

        struct Subentity
        {
            Ogre::String materialName;
        };
        std::vector<Subentity> subentities;

        struct ManualLod
        {
            std::vector<Subentity> subentities;
        };
        std::vector<ManualLod> manualLods;

        struct BoneAttachment
        {
            BoneAttachment()
            {
                this->object = 0;
                this->attachPosition = Ogre::Vector3::ZERO;
                this->attachScale = Ogre::Vector3::UNIT_SCALE;
                this->attachRotation = Ogre::Quaternion::IDENTITY;
            }

            ~BoneAttachment()
            {
                delete this->object;
            }

            /** Gets the name of the attachment itself */
            const Ogre::String& GetName() const
            {
                return this->object != 0 ? this->object->name : this->name;
            }

            Ogre::String name; //Used if object is null
            Ogre::String boneName;
            ObjectParameters* object;
            Ogre::Vector3 attachPosition;
            Ogre::Vector3 attachScale;
            Ogre::Quaternion attachRotation;
        };
        std::vector<BoneAttachment> boneAttachments;
    };

	struct NodeParameters
	 {
		 NodeParameters()
		 {
			 this->visibility = NODE_VISIBILITY_DEFAULT;
			 this->position = Ogre::Vector3::ZERO;
			 this->scale = Ogre::Vector3::UNIT_SCALE;
		 }

		 ~NodeParameters()
		 {
			 for (Objects::iterator objectIterator = this->objects.begin();
				 objectIterator != this->objects.end();
				 ++objectIterator)
			 {
				 delete *objectIterator;
			 }
		 }

		 Ogre::String name;
		 Ogre::String modelFile;

		 NodeVisibility visibility;

		 Ogre::Vector3 position;
		 Ogre::Quaternion orientation;
		 Ogre::Vector3 scale;

		 std::vector<NodeParameters> childNodes;

		 std::vector<NodeAnimationParameters> animations;

		 typedef std::list<ObjectParameters*> Objects;
		 Objects objects;
	 };

	/** Used for attaching MovableObject instances to an owner */
    struct MovableObjectOwner
    {
        /** No owner */
        MovableObjectOwner()
        {
            this->node = 0;
            this->entity = 0;
            this->attachPosition = Ogre::Vector3::ZERO;
            this->attachScale = Ogre::Vector3::UNIT_SCALE;
            this->attachRotation = Ogre::Quaternion::IDENTITY;
        }

        /** The owner is a scene node */
        MovableObjectOwner(Ogre::SceneNode* node)
        {
            this->node = node;
            this->entity = 0;
            this->attachPosition = Ogre::Vector3::ZERO;
            this->attachScale = Ogre::Vector3::UNIT_SCALE;
            this->attachRotation = Ogre::Quaternion::IDENTITY;
        }

        /** The owner is a bone within an entity's skeleton */
        MovableObjectOwner
            (
            Ogre::Entity* entity,
            const Ogre::String& boneName = Ogre::StringUtil::BLANK,
            const Ogre::Vector3& attachPosition = Ogre::Vector3::ZERO,
            const Ogre::Vector3& attachScale = Ogre::Vector3::UNIT_SCALE,
            const Ogre::Quaternion& attachRotation = Ogre::Quaternion::IDENTITY
            )
        {
            this->node = 0;
            this->entity = entity;
            this->boneName = boneName;
            this->attachPosition = attachPosition;
            this->attachScale = attachScale;
            this->attachRotation = attachRotation;
        }

        /** Attaches the movable object to the owner */
        void Attach(Ogre::MovableObject* object) const
        {
            if (this->node != 0)
			{
				movableObject = object;
                this->node->attachObject(movableObject);
			}
            else if (this->entity != 0 && !this->boneName.empty())
            {
                //TODO: Modify Ogre to accept object->getName() when creating TagPoint
                Ogre::TagPoint* tagPoint = this->entity->attachObjectToBone(this->boneName, object);
                tagPoint->setPosition(this->attachPosition);
                tagPoint->setScale(this->attachScale);
                tagPoint->setOrientation(this->attachRotation);
            }
        }

        /**
            * Attaches an empty object to the owner. This has no effect if the owner is a node since
            * there's no notion of an "empty" object for nodes. For entities, an "empty" object corresponds
            * to a tag point that has no attachment
            */
        void AttachEmpty(const Ogre::String& name = Ogre::StringUtil::BLANK) const
        {
            if (this->entity != 0 && !this->boneName.empty())
            {
                Ogre::SkeletonInstance* skeleton = this->entity->getSkeleton();
                Ogre::Bone* bone = skeleton->getBone(this->boneName);
                //TODO: Modify Ogre to accept name when creating TagPoint
                Ogre::TagPoint* tagPoint = skeleton->createTagPointOnBone(bone);
                tagPoint->setPosition(this->attachPosition);
                tagPoint->setScale(this->attachScale);
                tagPoint->setOrientation(this->attachRotation);
            }
        }

        Ogre::SceneNode* node;
        Ogre::Entity* entity;
        Ogre::String boneName;
        Ogre::Vector3 attachPosition;
        Ogre::Vector3 attachScale;
        Ogre::Quaternion attachRotation;
		mutable Ogre::MovableObject* movableObject;
    };
}