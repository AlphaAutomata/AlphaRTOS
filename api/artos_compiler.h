#ifndef ARTOS_COMPILER_H
#define ARTOS_COMPILER_H

#ifdef _MSC_VER
#define NORETURN __declspec(noreturn)
#else
#include <stdnoreturn.h>
#endif

#endif // #ifndef ARTOS_COMPILER_H
