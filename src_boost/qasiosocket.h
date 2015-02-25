#ifndef QASIOSOCKET_H
#define QASIOSOCKET_H
#include <QtGlobal>

#if defined(QASIOSOCKETH_LIBRARY)
#  define QASIOSOCKET_EXPORT Q_DECL_EXPORT
#else
#  define QASIOSOCKET_EXPORT Q_DECL_IMPORT
#endif

#endif
