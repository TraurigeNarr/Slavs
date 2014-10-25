#include "stdafx.h"

#include "SceneController.h"

#include "OgreFramework.h"

#include <OgreLight.h>

namespace ClientGame
  {

  SceneController::SceneController(OgreFramework& i_framework)
    : m_framework(i_framework)
    , mp_light(nullptr)
    {    }

  SceneController::~SceneController()
    {    }

  void SceneController::Initialize()
    {
    mp_light = m_framework.GetSceneManager()->createLight("MainLight"); //create pointer to light object
    mp_light->setType(Ogre::Light::LT_POINT);                           // make this light a point light <=> sun like
    mp_light->setDiffuseColour(1.0, .5, 0.0);                           //color the light orange 
    mp_light->setSpecularColour(1.0, 1.0, 0.0);                         //yellow highlights
    }

  void SceneController::Update(long i_elapsed_time)
    {

    }

  } // ClientGame