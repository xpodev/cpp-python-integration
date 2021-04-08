#pragma once

#include "./PythonCore.h"

#include <concepts>

#include "./Object.h"
#include "./String.h"


namespace GameCore {
	namespace Python {
		struct Float : public Object {
			Float(PyObject* pyObject)
				: Object(PyFloat_Check(pyObject) ? pyObject : nullptr)
			{

			}

			template <std::integral V>
			Float(V value)
				: Object(PyFloat_FromDouble(static_cast<double>(value)))
			{

			}

			template <std::floating_point V>
			Float(V value)
				: Object(PyFloat_FromDouble(value))
			{

			}

			Float(char const* str) 
				: Object(PyFloat_FromString(String(str)))
			{

			}

			template <class T>
			requires std::is_integral_v<T>
				operator T() {
				return static_cast<T>(PyFloat_AsDouble(m_pyObject));
			}

			template <class T>
			requires std::is_floating_point_v<T>
				operator T() {
				return static_cast<T>(PyFloat_AsDouble(m_pyObject));
			}
		};
	}
}
