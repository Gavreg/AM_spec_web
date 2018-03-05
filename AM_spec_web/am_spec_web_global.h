#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(AM_SPEC_WEB_LIB)
#  define AM_SPEC_WEB_EXPORT Q_DECL_EXPORT
# else
#  define AM_SPEC_WEB_EXPORT Q_DECL_IMPORT
# endif
#else
# define AM_SPEC_WEB_EXPORT
#endif
