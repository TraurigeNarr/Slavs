#pragma once

class BaseEvent
  {
  private:
    int m_event_type;
    bool m_data_ownage;

  protected:
    void* mp_data;

  public:
    BaseEvent(int i_event, void* ip_owned_data = nullptr, bool i_ownage = true)
      : m_event_type(i_event)
      , mp_data(ip_owned_data)
      , m_data_ownage(ip_owned_data)
      {
      }

    virtual ~BaseEvent()
      {
      if (m_data_ownage && mp_data != nullptr)
        delete mp_data;
      }

    int GetType() const
      {
      return m_event_type;
      }

    template <typename T>
    T* GetData()
      {
      return (T*)mp_data;
      }
  };