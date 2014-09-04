#pragma once

namespace OgreMax
  {
  class OgreMaxModel;
  }

class OgreFramework;

namespace ClientGame
  {

  class ModelController
    {
    private:
      std::map< std::string, std::shared_ptr<OgreMax::OgreMaxModel> > m_models;
      OgreFramework&                                                  m_ogre_framework;

    public:
      ModelController(OgreFramework& i_ogre_framework);
      ~ModelController();

      std::shared_ptr<OgreMax::OgreMaxModel>  GetModel (const std::string& i_path, const std::string& i_resource_group);
      OgreFramework&                          GetOgreFramework();
    };

  //////////////////////////////////////////////////////////////////////////

  inline OgreFramework& ModelController::GetOgreFramework()
    {
    return m_ogre_framework;
    }

  } // ClientGame