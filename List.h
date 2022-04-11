#pragma once

#include <Python.h>

#include "./PythonCore.h"

#include "./Iterator.h"
#include "./Object.h"

namespace xpo::python {

//template <typename T = Object>
////requires (std::is_base_of<Object, T>)
//struct ListItem final : T {
//	ListItem(List<T>& list, Py_ssize_t index)
//		: m_list{ list }
//		, m_index { index }
//	{
//
//	}
//
//	T operator=(T && item) {
//		// todo: check errors
//		m_list.set(m_index, item);
//		return *item;
//	}
//
//private:
//	List<T>& m_list;
//
//	Py_ssize_t const index;
//};

template <typename T = Object>
requires (std::is_base_of_v<Object, T>)
struct List : public Object {
	List(PyObject* pyObject)
		: Object(PyList_Check(pyObject) ? pyObject : nullptr)
	{

	}

	List(Py_ssize_t size = 0) : List(PyList_New(size)) {}

	template <Py_ssize_t Size>
	List() : List(Size) {}

	Py_ssize_t size() const {
		return PyList_Size(m_pyObject);
	}

	List get_slice(Py_ssize_t begin, Py_ssize_t end) const {
		return List(PyList_GetSlice(m_pyObject, begin, end));
	}

	template <typename U = T>
	requires (std::convertible_to<T, U>)
	U get(Py_ssize_t index) {
		return static_cast<U>(Object(PyList_GetItem(m_pyObject, index)));
	}

	bool set(Py_ssize_t index, T item) {
		return !PyList_SetItem(m_pyObject, index, item);
	}

	bool insert(Py_ssize_t index, T item) {
		return !PyList_Insert(m_pyObject, index, item);
	}

	bool append(T item) {
		return !PyList_Append(m_pyObject, item);
	}

	template <typename U>
	requires (std::is_base_of_v<T, U>)
	bool set_slice(Py_ssize_t begin, Py_ssize_t end, List<U> items) {
		return !PyList_SetSlice(m_pyObject, begin, end, items);
	}

	bool delete_slice(Py_ssize_t begin, Py_ssize_t end) { return set_slice(begin, end, nullptr); }

	bool sort() { return !PyList_Sort(m_pyObject); }

	bool reverse() { return !PyList_Reverse(m_pyObject); }

	Iterator<T> begin() const {
		return Iterator<T>(PyObject_GetIter(m_pyObject));
	}

	Iterator<T> end() const {
		return Iterator<T>::end();
	}

	operator Tuple() {
		return Tuple(PyList_AsTuple(m_pyObject));
	}
};

}
