#ifndef JS_GLOBAL_H
#define JS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(JS_LIBRARY)
#  define JSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define JSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // JS_GLOBAL_H
