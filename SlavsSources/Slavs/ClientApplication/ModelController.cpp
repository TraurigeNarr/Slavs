#include "stdafx.h"

#include "ModelController.h"

#include "OgreMaxModel.h"
#include "OgreFramework.h"

namespace ClientGame
  {

  ModelController::ModelController(OgreFramework& i_ogre_framework)
    : m_ogre_framework(i_ogre_framework)
    {    }

  ModelController::~ModelController()
    {    }


  std::shared_ptr<OgreMax::OgreMaxModel> ModelController::GetModel (const std::string& i_path, const std::string& i_resource_group)
    {
    auto it_model = m_models.find(i_path);
    if (it_model != m_models.end())
      *it_model;

    std::shared_ptr<OgreMax::OgreMaxModel> p_model = std::make_shared<OgreMax::OgreMaxModel>();
    try
      {
      p_model->Load(i_path, i_resource_group);
      }
    catch (...)
      {
      p_model = nullptr;
      throw;
      }

    m_models[i_path] = p_model;
    return p_model;
    }

  } // ClientGame