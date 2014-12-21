#pragma once

#include "../SlavsServerAPI.h"

#include "Types.h"

#include <Network/ISerializable.h>

namespace Network
  {
  class SerializableData;
  }

//////////////////////////////////////////////////////////////////////////

class SLAVS_SERVER_EXPORT IManager
  {
  protected:
    Slavs::TGoverment mp_goverment;

    bool              m_has_changes;

  protected:
    void _Invalidate();

  public:
    IManager();
    virtual ~IManager();

    void              SetGoverment(Slavs::TGoverment ip_goverment);
    Slavs::TGoverment GetGoverment() const;

    bool              HasChanges() const;
    virtual void      Invalidate();
    void              ValidateChanges();

    virtual void Serialize (Network::SerializableData& i_data) const {}
    virtual void Deserialize (const Network::SerializableData& i_data) {}
  };