#pragma once

namespace BasePlugin
  {
  enum class ObjectType : char
    {
    OT_NONE,
    OT_HUMAN,
    OT_HIZHINA,
    OT_MILL,
    OT_PRODUCTION_STORE
    };

  enum class ComponentType : char
    {
    CT_NONE,
    CT_STATIC_OBJECT,
    CT_DYNAMIC_OBJECT,
    CT_HUMAN,
    CT_HOUSE,
    CT_STORE
    };
  }
