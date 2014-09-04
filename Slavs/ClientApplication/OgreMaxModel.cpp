#include "stdafx.h"

#include "OgreMaxModel.h"
#include "OgreMaxUtilities.h"

#include <Common/Utilities/XmlUtilities.h>

#include <OgreSubEntity.h>


using namespace OgreMax;
using namespace Types;

OgreMaxModel::OgreMaxModel()
  {
  }

OgreMaxModel::~OgreMaxModel()
  {
  }

void OgreMaxModel::Load(const Ogre::String& fileName, const Ogre::String& resourceGroupName)
  {
  //Load from the XML document
  TiXmlDocument document;
  bool succsess = XmlUtilities::LoadXmlDocument(fileName, document);
  LoadNode(document.RootElement(), this->m_RootNode);
  }

void OgreMaxModel::LoadNode(const TiXmlElement* objectElement, NodeParameters& node)
  {
  node.name = XmlUtilities::GetStringAttribute(objectElement, "name");
  node.modelFile = XmlUtilities::GetStringAttribute(objectElement, "modelFile");
  if (node.modelFile.empty())
    node.modelFile = XmlUtilities::GetStringAttribute(objectElement, "modelName");
  node.visibility = ClientUtilities::GetNodeVisibilityAttribute(objectElement, "visibility");
  node.childNodes.resize(XmlUtilities::GetChildElementCount(objectElement, "node"));

  //Iterate over all the node children
  size_t childNodeIndex = 0;
  Ogre::String elementName;
  const TiXmlElement* childElement = 0;
  while ((childElement = XmlUtilities::IterateChildElements(objectElement, childElement)))
    {
    elementName = childElement->Value();

    if (elementName == "position")
      node.position = ClientUtilities::LoadXYZ(childElement);
    else if (elementName == "rotation")
      node.orientation = ClientUtilities::LoadRotation(childElement);
    else if (elementName == "scale")
      node.scale = ClientUtilities::LoadXYZ(childElement);
    else if (elementName == "node")
      LoadNode(childElement, node.childNodes[childNodeIndex++]);
    else if (elementName == "entity")
      node.objects.push_back(LoadEntity(childElement));
    else if (elementName == "animations")
      LoadNodeAnimations(childElement, node);
    }
  }

EntityParameters* OgreMaxModel::LoadEntity(const TiXmlElement* objectElement)
  {
  EntityParameters* parameters = new EntityParameters;
  parameters->name = XmlUtilities::GetStringAttribute(objectElement, "name");
  parameters->queryFlags = XmlUtilities::GetIntAttribute(objectElement, "queryFlags", 0);
  parameters->visibilityFlags = XmlUtilities::GetIntAttribute(objectElement, "visibilityFlags", 0);
  parameters->visibility = GameUtilities::GetObjectVisibilityAttribute(objectElement, "visible");
  parameters->meshFile = XmlUtilities::GetStringAttribute(objectElement, "meshFile");
  parameters->materialFile = XmlUtilities::GetStringAttribute(objectElement, "materialFile");

  Ogre::String skeletonAnimationBlendMode = XmlUtilities::GetStringAttribute(objectElement, "skeletonAnimationBlendMode");
  parameters->skeletonAnimationBlendMode = ClientUtilities::ParseSkeletonAnimationBlendMode(skeletonAnimationBlendMode);

  parameters->castShadows = XmlUtilities::GetBoolAttribute(objectElement, "castShadows", true);

  Ogre::String renderQueue = XmlUtilities::GetStringAttribute(objectElement, "renderQueue");
  parameters->renderQueue = ClientUtilities::ParseRenderQueue(renderQueue);

  parameters->renderingDistance = XmlUtilities::GetRealAttribute(objectElement, "renderingDistance", 0);



  //Parse child elements
  const TiXmlElement* boneAttachmentsElement = 0;
  Ogre::String elementName;
  const TiXmlElement* childElement = 0;
  while ((childElement = XmlUtilities::IterateChildElements(objectElement, childElement)))
    {
    elementName = childElement->Value();

    if (elementName == "vertexBuffer")
      ClientUtilities::LoadBufferUsage(childElement, parameters->vertexBufferUsage, parameters->vertexBufferShadowed);
    else if (elementName == "indexBuffer")
      ClientUtilities::LoadBufferUsage(childElement, parameters->indexBufferUsage, parameters->indexBufferShadowed);
    else if (elementName == "subentities")
      ClientUtilities::LoadSubentities(childElement, parameters->subentities);
    else if (elementName == "manualLods")
      ClientUtilities::LoadManualLods(childElement, parameters->manualLods);
    else if (elementName == "boneAttachments")
      boneAttachmentsElement = childElement;
    }

  //Load bone attachments
  if (boneAttachmentsElement != 0)
    LoadBoneAttachments(boneAttachmentsElement, *parameters);

  return parameters;
  }

void OgreMaxModel::LoadBoneAttachments(const TiXmlElement* objectElement, EntityParameters& entity)
  {
  entity.boneAttachments.resize(XmlUtilities::GetChildElementCount(objectElement, "boneAttachment"));

  size_t index = 0;
  const TiXmlElement* childElement = 0;
  while ((childElement = XmlUtilities::IterateChildElements(objectElement, childElement)))
    LoadBoneAttachment(childElement, entity.boneAttachments[index++]);
  }

void OgreMaxModel::LoadBoneAttachment(const TiXmlElement* objectElement, EntityParameters::BoneAttachment& boneAttachment)
  {
  boneAttachment.name = XmlUtilities::GetStringAttribute(objectElement, "name");
  boneAttachment.boneName = XmlUtilities::GetStringAttribute(objectElement, "bone");

  Ogre::String elementName;
  const TiXmlElement* childElement = 0;
  while ((childElement = XmlUtilities::IterateChildElements(objectElement, childElement)))
    {
    elementName = childElement->Value();
    if (elementName == "position")
      boneAttachment.attachPosition = ClientUtilities::LoadXYZ(childElement);
    else if (elementName == "rotation")
      boneAttachment.attachRotation = ClientUtilities::LoadRotation(childElement);
    else if (elementName == "scale")
      boneAttachment.attachScale = ClientUtilities::LoadXYZ(childElement);
    else if (elementName == "entity")
      boneAttachment.object = LoadEntity(childElement);
    }
  }

void OgreMaxModel::LoadNodeAnimations(const TiXmlElement* objectElement, NodeParameters& node)
  {
  node.animations.resize(XmlUtilities::GetChildElementCount(objectElement, "animation"));

  //Parse child elements
  int animationIndex = 0;
  Ogre::String elementName;
  const TiXmlElement* childElement = 0;
  while ((childElement = XmlUtilities::IterateChildElements(objectElement, childElement)))
    {
    elementName = childElement->Value();

    if (elementName == "animation")
      LoadNodeAnimation(childElement, node.animations[animationIndex++]);
    }
  }

void OgreMaxModel::LoadNodeAnimation(const TiXmlElement* objectElement, NodeAnimationParameters& animation)
  {
  //Ogre::Animation name
  animation.name = XmlUtilities::GetStringAttribute(objectElement, "name");

  //Length
  animation.length = XmlUtilities::GetRealAttribute(objectElement, "length", 0);

  //Interpolation mode
  Ogre::String interpolationModeText = XmlUtilities::GetStringAttribute(objectElement, "interpolationMode");
  if (!interpolationModeText.empty())
    animation.interpolationMode = ClientUtilities::ParseAnimationInterpolationMode(interpolationModeText);

  //Rotation interpolation mode
  Ogre::String rotationInterpolationModeText = XmlUtilities::GetStringAttribute(objectElement, "rotationInterpolationMode");
  if (!rotationInterpolationModeText.empty())
    animation.rotationInterpolationMode = ClientUtilities::ParseAnimationRotationInterpolationMode(rotationInterpolationModeText);

  //Get enabled and looping states
  animation.enable = XmlUtilities::GetBoolAttribute(objectElement, "enable", true);
  animation.looping = XmlUtilities::GetBoolAttribute(objectElement, "loop", true);

  //Load animation keyframes
  animation.keyframes.resize(XmlUtilities::GetChildElementCount(objectElement, "keyframe"));

  int keyframeIndex = 0;
  Ogre::String elementName;
  const TiXmlElement* childElement = 0;
  while ((childElement = XmlUtilities::IterateChildElements(objectElement, childElement)))
    {
    elementName = childElement->Value();

    if (elementName == "keyframe")
      LoadNodeAnimationKeyFrame(childElement, animation.keyframes[keyframeIndex++]);
    }
  }

void OgreMaxModel::LoadNodeAnimationKeyFrame(const TiXmlElement* objectElement, NodeAnimationParameters::KeyFrame& keyframe)
  {
  //Key time
  keyframe.time = XmlUtilities::GetRealAttribute(objectElement, "time", 0);

  //Parse child elements
  Ogre::String elementName;
  const TiXmlElement* childElement = 0;
  while ((childElement = XmlUtilities::IterateChildElements(objectElement, childElement)))
    {
    elementName = childElement->Value();

    if (elementName == "translation")
      keyframe.translation = ClientUtilities::LoadXYZ(childElement);
    else if (elementName == "rotation")
      keyframe.rotation = ClientUtilities::LoadRotation(childElement);
    else if (elementName == "scale")
      keyframe.scale = ClientUtilities::LoadXYZ(childElement);
    }
  }

Ogre::SceneNode* OgreMaxModel::CreateInstance
  (
  Ogre::SceneManager* sceneManager,
  int queryMask,
  OgreMaxModelInstanceCallback* callback,
  InstanceOptions options,
  Ogre::SceneNode* parentNode,
  const Ogre::String& defaultResourceGroupName,
  Ogre::SceneNode* node
  ) const
  {
  if (parentNode == 0)
    parentNode = sceneManager->getRootSceneNode();

  return CreateInstance
    (
    sceneManager,
    queryMask,
    callback,
    options,
    this->m_RootNode,
    parentNode,
    defaultResourceGroupName,
    node
    );
  }

Ogre::SceneNode* OgreMaxModel::CreateInstance
  (
  Ogre::SceneManager* sceneManager,
  int queryMask,
  OgreMaxModelInstanceCallback* callback,
  InstanceOptions options,
  const NodeParameters& nodeParams,
  Ogre::SceneNode* parentNode,
  const Ogre::String& defaultResourceGroupName,
  Ogre::SceneNode* node
  ) const
  {
  //Create the node if necessary
  bool newNode = false;
  if (node == 0)
    {
    newNode = true;
    // 		Ogre::String nodeName = baseName + nodeParams.name;
    node = parentNode->createChildSceneNode();
    }

  //Notify callback
  if (newNode && callback != 0)
    callback->StartedCreatingNode(this, node);

  //Set initial transformation
  if ((options & NO_INITIAL_TRANSFORMATION) == 0)
    {
    node->setPosition(nodeParams.position);
    node->setOrientation(nodeParams.orientation);
    node->setScale(nodeParams.scale);
    }

  MovableObjectOwner newOwner(node);
  for (NodeParameters::Objects::const_iterator objectIterator = nodeParams.objects.begin();
    objectIterator != nodeParams.objects.end();
    ++objectIterator)
    {
    CreateMovableObject(sceneManager, queryMask, *objectIterator, newOwner, callback, defaultResourceGroupName);
    }

  //Handle node animations
  bool isInitialStateSet = false;
  if (!nodeParams.animations.empty())
    {
    for (size_t animationIndex = 0; animationIndex < nodeParams.animations.size(); animationIndex++)
      {
      const NodeAnimationParameters& animationParams = nodeParams.animations[animationIndex];
      Ogre::String animationName = animationParams.name;

      //Create new animation if it doesn't already exist. It shouldn't
      Ogre::Animation* animation;
      bool animationAlreadyExists = sceneManager->hasAnimation(animationName);
      if (animationAlreadyExists)
        animation = sceneManager->getAnimation(animationName);
      else
        {
        //Create animation
        animation = sceneManager->createAnimation(animationName, animationParams.length);
        animation->setInterpolationMode(animationParams.interpolationMode);
        animation->setRotationInterpolationMode(animationParams.rotationInterpolationMode);
        }

      //Create animation track for node
      Ogre::NodeAnimationTrack* animationTrack = animation->createNodeTrack(animation->getNumNodeTracks() + 1, node);

      //Load animation keyframes
      for (size_t keyframeIndex = 0; keyframeIndex < animationParams.keyframes.size(); keyframeIndex++)
        {
        const NodeAnimationParameters::KeyFrame& keyframeParams = animationParams.keyframes[keyframeIndex];
        Ogre::TransformKeyFrame* keyFrame = animationTrack->createNodeKeyFrame(keyframeParams.time);

        keyFrame->setTranslate(keyframeParams.translation);
        keyFrame->setRotation(keyframeParams.rotation);
        keyFrame->setScale(keyframeParams.scale);
        }

      //Notify callback
      if (callback != 0)
        callback->CreatedNodeAnimationTrack(this, node, animationTrack, animationParams.enable, animationParams.looping);

      if ((options & NO_ANIMATION_STATES) == 0)
        {
        //Create a new animation state to track the animation
        if (!animationAlreadyExists)
          {
          //Create a new animation state to track the animation
          Ogre::AnimationState* animationState = sceneManager->createAnimationState(animationName);
          animationState->setEnabled(animationParams.enable);
          animationState->setLoop(animationParams.looping);

          //Notify callback
          if (callback != 0)
            callback->CreatedNodeAnimationState(this, node, animationState);
          }
        else if (callback != 0)
          {
          if (sceneManager->hasAnimationState(animationName))
            callback->ReferencedNodeAnimationState(this, node, sceneManager->getAnimationState(animationName));
          }
        }
      }

    ClientUtilities::SetIdentityInitialState(node);
    isInitialStateSet = true;
    }

  //Iterate over all the node children
  for (size_t childNodeIndex = 0; childNodeIndex < nodeParams.childNodes.size(); childNodeIndex++)
    {
    CreateInstance
      (
      sceneManager,
      queryMask,
      callback,
      options & ~NO_INITIAL_TRANSFORMATION,
      nodeParams.childNodes[childNodeIndex],
      parentNode,
      defaultResourceGroupName,
      node
      );
    }

  //Set the initial state if it hasn't already been set
  if (!isInitialStateSet)
    node->setInitialState();

  //Set the node's visibility
  ClientUtilities::SetNodeVisibility(node, nodeParams.visibility);

  //Notify callback
  if (newNode && callback != 0)
    callback->FinishedCreatingNode(this, node);

  return node;
  }

void OgreMaxModel::CreateMovableObject
  (
  Ogre::SceneManager* sceneManager,
  int queryMask,
  ObjectParameters* object,
  const MovableObjectOwner& owner,
  OgreMaxModelInstanceCallback* callback,
  const Ogre::String& defaultResourceGroupName
  ) const
  {
  switch (object->objectType)
    {
    case ObjectParameters::ENTITY:
      {
      CreateEntity(sceneManager, queryMask, (EntityParameters*)object, owner, callback, defaultResourceGroupName);
      break;
      }
    default: break;
    }
  }

void OgreMaxModel::CreateEntity
  (
  Ogre::SceneManager* sceneManager,
  int queryMask,
  EntityParameters* entityParams,
  const MovableObjectOwner& owner,
  OgreMaxModelInstanceCallback* callback,
  const Ogre::String& defaultResourceGroupName
  ) const
  {
  //Load the mesh
  bool isNewMesh = !Ogre::MeshManager::getSingleton().resourceExists(entityParams->meshFile);
  Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().load
    (
    entityParams->meshFile,
    !entityParams->resourceGroupName.empty() ? entityParams->resourceGroupName : defaultResourceGroupName,
    entityParams->vertexBufferUsage, entityParams->indexBufferUsage,
    entityParams->vertexBufferShadowed, entityParams->indexBufferShadowed
    );

  //Notify callback if the mesh was just loaded
  if (isNewMesh && callback != 0)
    callback->CreatedMesh(this, mesh.getPointer());

  //Create entity
  Ogre::Entity* entity = sceneManager->createEntity(entityParams->meshFile);
  if (entityParams->queryFlags != 0)
    entity->setQueryFlags(entityParams->queryFlags);
  if (entityParams->visibilityFlags != 0)
    entity->setVisibilityFlags(entityParams->visibilityFlags);
  GameUtilities::SetObjectVisibility(entity, entityParams->visibility);
  entity->setCastShadows(entityParams->castShadows);
  entity->setRenderQueueGroup(entityParams->renderQueue);
  entity->setRenderingDistance(entityParams->renderingDistance);
  if (!entityParams->materialFile.empty())
    entity->setMaterialName(entityParams->materialFile);

  //Set skeleton blend mode if necessary
  Ogre::SkeletonInstance* skeleton = entity->getSkeleton();
  if (skeleton != 0)
    skeleton->setBlendMode(entityParams->skeletonAnimationBlendMode);

  //Set subentities
  ClientUtilities::SetSubentities(entity, entityParams->subentities);

  //Set manual LODs
  ClientUtilities::SetManualLods(entity, entityParams->manualLods);

  //Create bone attachments
  if (!entityParams->boneAttachments.empty())
    {
    MovableObjectOwner entityOwner(entity);

    for (size_t boneAttachmentIndex = 0; boneAttachmentIndex < entityParams->boneAttachments.size(); boneAttachmentIndex++)
      {
      EntityParameters::BoneAttachment& boneAttachment = entityParams->boneAttachments[boneAttachmentIndex];
      entityOwner.boneName = boneAttachment.boneName;
      entityOwner.attachPosition = boneAttachment.attachPosition;
      entityOwner.attachScale = boneAttachment.attachScale;
      entityOwner.attachRotation = boneAttachment.attachRotation;
      if (boneAttachment.object != 0)
        {
        CreateMovableObject
          (
          sceneManager,
          queryMask,
          boneAttachment.object,
          entityOwner,
          callback,
          defaultResourceGroupName
          );
        }
      else
        entityOwner.AttachEmpty(boneAttachment.name);
      }
    }
  //set query mask
  entity->setQueryFlags(queryMask);
  //Attach entity to the owner
  owner.Attach(entity);

  //Notify callback
  if (callback != 0)
    callback->CreatedEntity(this, entity);
  }

EntityParameters* OgreMaxModel::GetEntityParameters()
  {
  if(!m_RootNode.objects.empty())
    {
    for (Types::NodeParameters::Objects::const_iterator objectIterator = m_RootNode.objects.begin();
      objectIterator != m_RootNode.objects.end();
      ++objectIterator)
      {
      Types::ObjectParameters *object = *objectIterator;
      if(Types::ObjectParameters::ENTITY == object->objectType)
        return (Types::EntityParameters*)object;
      }
    }
  return NULL;
  }