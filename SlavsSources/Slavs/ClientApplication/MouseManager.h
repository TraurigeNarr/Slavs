#ifndef __MOUSEMANAGER_H__
#define __MOUSEMANAGER_H__

namespace SDK
  {
  class Task;
  }

namespace GameCore
  {

  namespace UI
    {

    class IMouse;

    class MouseManager
      {
      public:
        typedef std::unique_ptr<IMouse> MousePtr;
      private:
        std::vector<MousePtr> m_mice;
        // should be changed to stack
        IMouse*               mp_current_mouse;
        IMouse*               mp_previous_mouse;

      public:
        MouseManager();
        ~MouseManager();

        void RegisterMouse(std::unique_ptr<IMouse> ip_mouse);
        void RegisterMouseAndActivate(std::unique_ptr<IMouse> ip_mouse, SDK::Task* ip_task);

        void SetActiveMouse(int i_id, SDK::Task* ip_task);
        void SetActiveMouse(const std::string& i_name, SDK::Task* ip_task);

        void PopMouse();
        void PopAllMice();

        void Update(long i_elapsed_time);
      };

    } // UI

  } // GameCore

#endif