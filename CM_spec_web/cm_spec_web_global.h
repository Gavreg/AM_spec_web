#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CM_SPEC_WEB_LIB)
#  define CM_SPEC_WEB_EXPORT Q_DECL_EXPORT
# else
#  define CM_SPEC_WEB_EXPORT Q_DECL_IMPORT
# endif
#else
# define CM_SPEC_WEB_EXPORT
#endif
