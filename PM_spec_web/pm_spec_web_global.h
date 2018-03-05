#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PM_SPEC_WEB_LIB)
#  define PM_SPEC_WEB_EXPORT Q_DECL_EXPORT
# else
#  define PM_SPEC_WEB_EXPORT Q_DECL_IMPORT
# endif
#else
# define PM_SPEC_WEB_EXPORT
#endif
