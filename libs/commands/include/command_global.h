#ifndef COMMANDGLOBAL_H
#define COMMANDGLOBAL_H

#include <QtCore/qglobal.h>

#ifdef COMMANDS_LIBRARY
#define COMMANDS_API Q_DECL_EXPORT
#else
#define COMMANDS_API Q_DECL_IMPORT
#endif

#endif // COMMANDGLOBAL_H
