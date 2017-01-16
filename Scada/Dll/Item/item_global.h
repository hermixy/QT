#ifndef ITEM_GLOBAL_H
#define ITEM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ITEM_LIBRARY)
#  define ITEMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ITEMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ITEM_GLOBAL_H
