#pragma once

#include "ComponentTypes.h"

#include <GameCore/Composer/IObjectComposer.h>

namespace GameCore
  {
  class ObjectSettings;
  } // GameCore

class TiXmlElement;
class ClientGameContext;

namespace ClientGame
  {

  class ClientComposer : public GameCore::IObjectComposer
    {
    private:
      std::map<std::string, int/*global id of object type*/>    m_definitions;

      std::map<int, std::unique_ptr<GameCore::ObjectSettings>>  mp_object_settings;
      ClientGameContext& m_context;

      int                                                       m_undefined_object_id;

    private:
      void _ParseComponent(const TiXmlElement& i_element);

    public:
      ClientComposer (ClientGameContext& i_context);
      virtual ~ClientComposer ();

      void AddObjectDefinition (const std::string& i_object_name, int i_object_id);

      void Initialize (const std::string& i_configuration_file);

    // IObjectComposer
    public:
      virtual void ComposeObject (GameObject* ip_object) override;
      virtual bool Supports (int i_object_type) override;

      virtual void DeclareSupportedTypes() override;

      virtual bool CheckContracts() const override;

      virtual int  GetComponentGlobalID (int i_local_id) const override;
      virtual int  GetObjectGlobalID (int i_local_id) const override;

      int          GetUndefinedObjectId () const;
    };

  //////////////////////////////////////////////////////////////////////////

  inline int ClientComposer::GetUndefinedObjectId() const
    {
    return m_undefined_object_id;
    }

  } // ClientGame