#pragma once

#include "./Object.h"
#include "./Tuple.h"


namespace xpo {
	namespace python {
		struct Callable : public Object {
			Callable(PyObject* pyObject)
				: Object(PyCallable_Check(pyObject) ? pyObject : nullptr)
			{
				
			}

			template <std::derived_from<Object> ...Ts>
			Object operator()(Ts ...args, Object kwargs = nullptr) {
				return Object(PyObject_Call(m_pyObject, Tuple(args...), kwargs));
			}
		};
	}
}
