#pragma once

namespace UI
  {
  class   IMessageProvider;
  class   ScreenManager;
  struct  ControlData;

  class Screen
    {
    private:
      Screen*        mp_owner;

    protected:
      ScreenManager&              m_screen_manager;
      std::weak_ptr<IMessageProvider> mp_message_provider;

    private:
      virtual void  SetRootWindow(void* ip_window) = 0;
      virtual void* GetRootWindow() = 0;

      virtual void Initialize() = 0;

    public:
      Screen(ScreenManager& i_screen_manager, Screen* ip_owner = nullptr);
      virtual ~Screen();

      void                              Create (Screen* ip_owner = nullptr);

      Screen*                           GetOwner();
      void                              SetMessageProvider(std::shared_ptr<IMessageProvider> ip_message_provider);

      template <typename ProviderType>
      std::shared_ptr<ProviderType> GetMessageProvider() const;
    public:
      virtual void Destroy() = 0;
      virtual void Update(long i_elapsed_time) {}

      virtual bool ControlChanged(const ControlData& i_control_data) {return false;}
    };

  //////////////////////////////////////////////////////////////////////////

  inline Screen* Screen::GetOwner()
    {
    return mp_owner;
    }

  inline void Screen::SetMessageProvider(std::shared_ptr<IMessageProvider> ip_message_provider)
    {
    mp_message_provider = ip_message_provider;
    }

  template <typename ProviderType>
  std::shared_ptr<ProviderType> Screen::GetMessageProvider() const
    {
    if (auto p_provider = mp_message_provider.lock())
      return std::dynamic_pointer_cast<ProviderType>(p_provider);
    return nullptr;
    }
  
  } // UI