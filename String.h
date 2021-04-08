#pragma once

#include "./PythonCore.h"

#include <string>

#include "./Object.h"


namespace xpo {
	namespace python {
		struct String : public Object {
			String(PyObject* pyObject)
				: Object(PyUnicode_Check(pyObject) ? pyObject : nullptr)
			{

			}

			String(Object object)
				: String(PyObject_Str(object))
			{

			}

			String(char const* str)
				: Object(PyUnicode_FromString(str))
			{

			}

			String(char const* str, Py_ssize_t length)
				: Object(PyUnicode_FromStringAndSize(str, length))
			{

			}

			String(std::string const& str)
				: String(str.c_str(), str.size())
			{

			}

			operator char const* () const {
				return PyUnicode_AsUTF8(m_pyObject);
			}
		};
	}
}
