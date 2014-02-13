#pragma once

#ifdef SLAVSCOMMONEXPORTING
#   define COMMON_EXPORT __declspec(dllexport)
#   define COMMON_EXPORT_TEMPLATE
#else
#   define COMMON_EXPORT __declspec(dllimport)
#   define COMMON_EXPORT_TEMPLATE extern
#endif