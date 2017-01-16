#ifndef RINGBUF_GLOBAL_H
#define RINGBUF_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RINGBUF_LIBRARY)
#  define RINGBUFSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RINGBUFSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RINGBUF_GLOBAL_H
