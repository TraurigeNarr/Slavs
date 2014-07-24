#pragma once

namespace BasePlugin
  {
  enum class ObjectType : char
    {
    OT_NONE,
    OT_HUMAN,
    OT_HIZHINA,
    OT_MILL,
    OT_PRODUCTION_STORE,
    OT_MANUFACTURE
    };

  enum class ComponentType : char
    {
    CT_NONE,
    CT_STATIC_COMPONENT,
    CT_DYNAMIC_COMPONENT,
    CT_HUMAN,
    CT_HOUSE,
    CT_STORE,
    CT_MANUFACTURE
    };
  }
