#pragma once

namespace UI
  {

  class IMessageProvider
    {
    protected:
      bool m_valid;
    public:
      IMessageProvider() : m_valid(false) {}
      virtual ~IMessageProvider(){}

      virtual bool IsValid() { return m_valid; }
      virtual void Invalidate() { m_valid = false; }
      virtual void Validate() { m_valid = true; }
    };

  } // UI