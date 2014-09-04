#pragma once

class InputManager;
class OgreFramework;

namespace ClientGame
  {

  class Camera
    {
    private:
      class CameraImpl;

    private:
      OgreFramework&                m_framework;
      InputManager&                 m_input_manager;
      std::unique_ptr<CameraImpl>   mp_impl;

    public:
      Camera(OgreFramework& i_framework, InputManager& i_input_manager);
      ~Camera();
    };

  } // ClientGame