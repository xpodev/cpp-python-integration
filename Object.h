#pragma once

#include "./PythonCore.h"

#include <iostream>
#include <stdio.h>


namespace GameCore {
	namespace Python {
		// this struct is a wrapper over a PyObject*
		struct Object {
			Object(PyObject* pyObject)
				: m_pyObject(pyObject)
			{
			}

			Object(Object const& pyObject)
				: Object(pyObject.m_pyObject)
			{}

			Object(Object && pyObject) noexcept
				: Object(pyObject.m_pyObject)
			{}

			Object& incref() {
				Py_INCREF(m_pyObject);
				return *this;
			}

			Object& decref() {
				Py_DECREF(m_pyObject);
				return *this;
			}

			PyObject* ptr() const {
				return m_pyObject;
			}

			bool is_null() const {
				return m_pyObject == nullptr;
			}

			bool is_none() const {
				return m_pyObject == Py_None;
			}

			template <std::convertible_to<Object> T>
			T as() {
				return static_cast<T>(*this);
			}

			template <class T>
			T to_native() {
				return static_cast<T>(AutoObject<T>(*this));
			}

			bool has_attribute(Object name) {
				return PyObject_HasAttr(m_pyObject, name.m_pyObject);
			}

			bool has_attribute(char const* name) {
				return PyObject_HasAttrString(m_pyObject, name);
			}

			Object get_attribute(Object name) {
				return Object(PyObject_GetAttr(m_pyObject, name.m_pyObject));
			}

			Object get_attribute(char const* name) {
				return Object(PyObject_GetAttrString(m_pyObject, name));
			}

			bool set_attribute(Object name, Object value) {
				return !PyObject_SetAttr(m_pyObject, name, value);
			}

			bool set_attribute(char const* name, Object value) {
				return !PyObject_SetAttrString(m_pyObject, name, value);
			}

			bool delete_attribute(Object name) {
				return !PyObject_DelAttr(m_pyObject, name);
			}

			bool delete_attribute(char const* name) {
				return !PyObject_DelAttrString(m_pyObject, name);
			}

			Object repr() {
				return Object(PyObject_Repr(m_pyObject));
			}

			Object str() {
				return Object(PyObject_Str(m_pyObject));
			}

			int operator<(Object const& other) {
				return PyObject_RichCompareBool(m_pyObject, other.m_pyObject, Py_LT);
			}

			int operator>(Object const& other) {
				return PyObject_RichCompareBool(m_pyObject, other.m_pyObject, Py_GT);
			}

			int operator<=(Object const& other) {
				return PyObject_RichCompareBool(m_pyObject, other.m_pyObject, Py_LE);
			}

			int operator>=(Object const& other) {
				return PyObject_RichCompareBool(m_pyObject, other.m_pyObject, Py_GE);
			}

			int operator==(Object const& other) {
				return PyObject_RichCompareBool(m_pyObject, other.m_pyObject, Py_EQ);
			}

			int operator!=(Object const& other) {
				return PyObject_RichCompareBool(m_pyObject, other.m_pyObject, Py_NE);
			}

			friend std::ostream& operator<<(std::ostream& os, Object const& pyObject) {
				return os << PyUnicode_AsUTF8(PyObject_Repr(pyObject));
			}

			operator PyObject* () const {
				return m_pyObject;
			}

			static Object none() {
				return Object(Py_None);
			}

		protected:
			PyObject* m_pyObject;
		};
	}
}
