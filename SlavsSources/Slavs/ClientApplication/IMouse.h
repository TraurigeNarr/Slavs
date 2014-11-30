#ifndef __IMOUSE_H__
#define __IMOUSE_H__

namespace SDK
  {
  class Task;
  }

namespace GameCore
  {

  namespace UI
    {

    class IMouse
      {
      private:
        const int m_id;
        bool      m_is_active;

      private:
        virtual void          ActivateImpl(SDK::Task* ip_task) = 0;
        virtual void          DeactivateImpl() = 0;

      public:
        IMouse(int i_id);
        virtual ~IMouse();
        
        int                   GetId() const;

        virtual void          Pause(){}
        virtual void          Resume(){}

        void                  Activate(SDK::Task* ip_task);
        void                  Deactivate();

        bool                  IsActive() const;

        virtual std::string   GetName() const = 0;
        virtual void          Update(long i_elapsed_time) = 0;

        //////////////////////////////////////////////////////////////
        // Comparers

        struct Comparer_Id
          {
          const int m_id;
          Comparer_Id(int i_id)
            : m_id(i_id)
            {}

          bool operator () (const IMouse& i_mouse) const
            {
            return i_mouse.GetId() == m_id;
            }

          bool operator () (const std::unique_ptr<IMouse>& ip_mouse) const
            {
            return ip_mouse->GetId() == m_id;
            }

          };

        struct Comparer_Name
          {
          const std::string m_name;
          Comparer_Name(const std::string i_name)
            : m_name(i_name)
            {}

          bool operator () (const IMouse& i_mouse) const
            {
            return i_mouse.GetName() == m_name;
            }

          bool operator () (const std::unique_ptr<IMouse>& ip_mouse) const
            {
            return ip_mouse->GetName() == m_name;
            }
          };
      };

    inline int IMouse::GetId() const
      {
      return m_id;
      }

    inline bool IMouse::IsActive() const
      {
      return m_is_active;
      }

    } // UI

  } // GameCore

#endif