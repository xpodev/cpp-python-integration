#pragma once

#include "./PythonCore.h"

#include "./Callable.h"
#include "./Object.h"
#include "./Utils.h"


namespace xpo {
	namespace python {
		struct Module : public Object
		{
			Module(PyObject* pyObject)
				: Object(PyModule_Check(pyObject) ? pyObject : nullptr)
			{

			}

			char const* name() const {
				return PyModule_GetName(m_pyObject);
			}

			void add_object(char const* name, Object object) {
				PyModule_AddObject(m_pyObject, name, static_cast<PyObject*>(object));
			}

			template <IVarArgsFunction F>
			void add_method(char const* name, F&& function, char const* doc = nullptr) {
				PyMethodDef method_def = {
					name, static_cast<PyCFunction>(function), METH_VARARGS, doc
				};
				PyModule_AddFunctions(m_pyObject, &method_def);
			}

			template <IKWArgsFunction F>
			void add_method(char const* name, F&& function, char const* doc = nullptr) {
				PyMethodDef method_def = {
					// usually, casting to void* and then to PyCFunction is dangerous, but here we rely on the Python
					// language to make sure the function is being called properly
					name, static_cast<PyCFunction>(static_cast<void*>(function)), METH_VARARGS | METH_KEYWORDS, doc
				};
				PyModule_AddFunctions(m_pyObject, &method_def);
			}
		};
	}
}
