#pragma once

namespace Ogre
  {
  class Light;
  }

class OgreFramework;

namespace ClientGame
  {

  class SceneController
    {
    private:
      OgreFramework&  m_framework;
      Ogre::Light*    mp_light;

    public:
      SceneController (OgreFramework& i_framework);
      ~SceneController();

      void Initialize();

      void Update (long i_elapsed_time);
    };

  } // ClientGame