#define PY_VERSION 37

#include "PythonCore.h"

#include "Python37.h"

#include "Tuple.h"
#include "Integer.h"
#include "Float.h"
#include "String.h"

using namespace xpo::python;

PyMODINIT_FUNC PyInit_muonline();

int main() {
	//Initialize the python instance
	/*Py_Initialize();*/
	//Python python;
	//python.add_module("muonline", PyInit_muonline);

	//python.start();

	//auto m = PyImport_AddModule("muonline");
	//if (!(PyModule_Check(m))) {
	//	std::cout << "not a module" << std::endl;
	//	return 5;
	//}
	//if (PyObject_SetAttrString(PyEval_GetGlobals(), "muonline", m)) {
	//	python.run_file("test.py");
	//}

	//python.stop();

	//Close the python instance
	//Py_Finalize();

	Python python;
	python.add_module("muonline", PyInit_muonline);

	/* Add a built-in module, before Py_Initialize */
	//PyImport_AppendInittab("muonline", PyInit_muonline);

	/* Initialize the Python interpreter.  Required. */
	python.start();
	//Py_Initialize();

	/* Optionally import the module; alternatively,
	import can be deferred until the embedded script
	imports it. */
	python.run_string("import muonline");

	python.set_global("valueA", Tuple(Integer(123), Integer(234)));
	char const* b = String(Tuple(Integer(123), Integer(234)));
	std::cout << b << std::endl;
	std::cout << "valueA [global]: " << python.get_global("valueA") << std::endl;

	int result = python.run_file("test.py");
	std::cout << "Python result: " << result << std::endl;

	std::cout << python.main_module().name() << std::endl;
	/*FILE* file = fopen("test.py", "r");
	if (file) {
		PyRun_SimpleFileEx(file, "test.py", 1);
	}*/

	python.stop();
	//Py_Finalize();

	return 0;
}
