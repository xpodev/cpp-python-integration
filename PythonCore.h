#pragma once

#undef _DEBUG
#undef _module

#ifdef PY_VERSION

#define STR(x) #x
#define XSTR(x) STR(x)

/* here's the magic */
#define _CONCAT(x,y) x##y
#define CONCAT(x,y) _CONCAT(x,y)

#define PYTHON_LIB_PATH CONCAT(CONCAT(./PythonVersions/P, PY_VERSION), CONCAT(/libs/python, PY_VERSION).lib)

#define AUTO_INCLUDE_LIB

#ifdef AUTO_INCLUDE_LIB
#pragma comment(lib, XSTR(PYTHON_LIB_PATH))
#endif

#undef _CONCAT
#undef CONCAT
#undef XSTR
#undef STR

#endif // PY_VERSION

#define PY_SSIZE_T_CLEAN
#include <Python.h>
