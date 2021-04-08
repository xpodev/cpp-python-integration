#define PY_VERSION 37

#include "PythonCore.h"

#include "Callable.h"
#include "ModuleBuilder.h"
#include "Object.h"

#include <iostream>

using namespace xpo::python;


static PyObject* m_test(PyObject* self, PyObject* args) {
	PyObject_Print(PyUnicode_FromString("Hello, MuOnline!"), stdout, 0);
	return args;
}

static PyObject* say_hello(PyObject* self, PyObject* args, PyObject* kwargs) {
    PyObject* callback;

    if (!PyArg_ParseTuple(args, "O", &callback))
    {
        return NULL;
    }

    Callable callable{ callback };
    callable(kwargs);

    Py_RETURN_NONE;
}

ModuleBuilder muonline{ "muonline" };


PyMODINIT_FUNC PyInit_muonline() {
    muonline.add_method("test1", m_test);
    muonline.add_method("test2", say_hello);
    return PyModule_Create(&muonline.module_definition());
}
