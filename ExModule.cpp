#define X_PY_VERSION 37

#include "PythonCore.h"

#include "Callable.h"
#include "ModuleBuilder.h"
#include "Object.h"
#include "./Utils.h"

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

static Object something(Module self, Tuple args) {
    Tuple inner = Tuple(Tuple(args[0].ptr())[0].ptr());
    TypedTuple<int, int, float> typed(inner);
    auto [x, y, z] = typed.unpack();
    std::cout << args.size() << x << ' ' << y << ' ' << z << std::endl;
    return Object::none();
}

static Object something2(Module self, TypedTuple<int, int, float> args) {
    auto [x, y, z] = args;
    std::cout << args.size() << x << '|' << y << '|' << z << std::endl;
    return Object::none();
}

static Object something3(Module self, int x, int y, float z) {
    std::cout << x << '<' << y << '<' << z << std::endl;
    return Object::none();
}

ModuleBuilder muonline{ "muonline" };


//typedef Object(*Ft)(Module, int, int, float);
//typedef Function2<Ft> Ft2;


PyMODINIT_FUNC PyInit_muonline() {
    muonline.add_method("test1", m_test);
    muonline.add_method("test2", say_hello);
    muonline.add_method<something>("something");
    muonline.add_method<PackedVarArgsFunctionObject(something2)>("something2");
    muonline.add_method<something3>("something3");
    return PyModule_Create(&muonline.module_definition());
}
