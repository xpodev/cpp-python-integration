#pragma once

#include <concepts>

#include "./PythonCore.h"
#include "./PyTypes.h"


namespace xpo {
	namespace python {
		template <class T = Object>
		struct AutoObject : Object {
			AutoObject(T object)
				: Object(std::move(static_cast<Object>(object)))
			{

			}
		};

		template <std::integral T>
		struct AutoObject<T> : Integer {
			AutoObject(T t)
				: Integer(t)
			{

			}

			AutoObject(PyObject* pyObject)
				: Integer(pyObject)
			{

			}
		};

		template <std::floating_point T>
		struct AutoObject<T> : Float {
			AutoObject(T t)
				: Float(t)
			{

			}

			AutoObject(PyObject* pyObject)
				: Float(pyObject)
			{

			}
		};

		template <>
		struct AutoObject<std::string> : String {
			AutoObject(std::string& t)
				: String(t)
			{

			}

			AutoObject(PyObject* pyObject)
				: String(pyObject)
			{

			}
		};

		template <>
		struct AutoObject<char const*> : String {
			AutoObject(char const* t)
				: String(t)
			{

			}

			AutoObject(PyObject* pyObject)
				: String(pyObject)
			{

			}
		};
	}
}
