#pragma once

#include <QtGlobal>

#if defined(ENABLE_SHARED_LIBRARY)
#  define SHARED_EXPORT Q_DECL_EXPORT
#else
#  define SHARED_EXPORT Q_DECL_IMPORT
#endif
