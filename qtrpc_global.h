#ifndef QTRPC_GLOBAL_H
#define QTRPC_GLOBAL_H

#if defined(QTRPC_LIBRARY)
#  define QTRPC_EXPORT Q_DECL_EXPORT
#else
#  define QTRPC_EXPORT Q_DECL_IMPORT
#endif

#endif // QTRPC_GLOBAL_H
