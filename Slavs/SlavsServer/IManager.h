#pragma once

#include "../SlavsServerAPI.h"

//////////////////////////////////////////////////////////////////////////

class SLAVS_SERVER_EXPORT IManager
  {
  protected:
    Slavs::TGoverment m_goverment;

    bool              m_has_changes;

  protected:
    void _Invalidate();

  public:
    virtual ~IManager();

    void              SetGoverment(Slavs::TGoverment ip_goverment);
    Slavs::TGoverment GetGoverment() const;
    bool              HasChanges();

  };