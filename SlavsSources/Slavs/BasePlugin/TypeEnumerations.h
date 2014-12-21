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
    OT_MANUFACTURE,
		OT_TREE,
		OT_LESOVIK
    };

  enum class ComponentType : char
    {
    CT_NONE,
    CT_STATIC_COMPONENT,
    CT_DYNAMIC_COMPONENT,
    CT_HUMAN,
    CT_HOUSE,
    CT_STORE,
    CT_MANUFACTURE,
    CT_PRIMITIVE_BRAIN,
		CT_RESOURCE,
		CT_LESOVIK
    };
  }
