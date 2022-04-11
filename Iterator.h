#pragma once

#include <Python.h>

#include "./Object.h"

#include <concepts>


namespace xpo::python {

template <std::convertible_to<Object> T>
struct Iterator : public Object {
	Iterator(PyObject* pyObject)
		: Object(PyIter_Check(pyObject) ? pyObject : nullptr)
        , m_current{ nullptr }
	{
        next();
	}

	void next() {
		m_current = PyIter_Next(m_pyObject);
	}

    Iterator& operator++() { next(); return *this; }
    Iterator operator++(int) { Iterator retval = *this; ++(*this); return retval; }
    bool operator==(Iterator other) const { return m_current == other.m_current; }
    bool operator!=(Iterator other) const { return !(*this == other); }
    T operator*() { return static_cast<T>(Object(m_current)); }

    // iterator traits
    using difference_type = long;
    using value_type = T;
    using pointer = T const*;
    using reference = T const&;
    using iterator_category = std::forward_iterator_tag;

    static Iterator<T> end() {
        return Iterator<T>();
    }

private:
    Iterator() 
        : Object(nullptr)
        , m_current{ nullptr }
    {

    }

private:
    PyObject* m_current;
};

}
