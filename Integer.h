#pragma once

#include "./PythonCore.h"

#include <concepts>

#include "./Object.h"


namespace xpo {
	namespace python {
		struct Integer : public Object {
			Integer(PyObject* pyObject)
				: Object(PyLong_Check(pyObject) ? pyObject : nullptr)
			{

			}

			template <std::integral V>
			Integer(V value)
				: Object(PyLong_FromLongLong(value))
			{

			}

			template <std::floating_point V>
			Integer(V value)
				: Object(PyLong_FromDouble(value))
			{

			}

			Integer(char const* str, char** pend = nullptr, int base = 0)
				: Object(PyLong_FromString(str, pend, base))
			{

			}

			template <class T>
			requires std::is_integral_v<T>
				operator T() {
				return static_cast<T>(PyLong_AsLongLong(m_pyObject));
			}

			template <class T>
			requires std::is_floating_point_v<T>
				operator T() {
				return static_cast<T>(PyLong_AsDouble(m_pyObject));
			}
		};
	}
}
