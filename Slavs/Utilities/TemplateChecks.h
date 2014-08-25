#pragma once

template <typename Base, typename Derived>
struct DerivedFrom
  {
  static void constraints (Derived* ip_derived) { Base* p_base = ip_derived; }
  DerivedFrom() { void(*p)(Derived*) = constraints; }
  };