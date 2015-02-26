#ifndef QASIOSOCKET_H
#define QASIOSOCKET_H
#include <QtGlobal>

#if defined(QASIOSOCKET_LIBRARY)

#if defined(QASIOSOCKET_STATIC)
#define QASIOSOCKET_EXPORT Q_DECL_IMPORT
#else
#  define QASIOSOCKET_EXPORT Q_DECL_EXPORT
#endif

#else
#  define QASIOSOCKET_EXPORT
#endif

#endif
