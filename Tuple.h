#pragma once

#include "./AutoObject.h"
#include "./Object.h"


namespace GameCore {
	namespace Python {
		struct Tuple;

		template <Py_ssize_t C, typename T, typename... Ts>
		struct TupleUnpacker {
			TupleUnpacker(Tuple& tuple)
				: m_tuple(tuple)
			{

			}

			bool unpack(T ptr, Ts... rest) {
				*ptr = m_tuple.get(m_tuple.size() - C).to_native<std::remove_pointer_t<T>>();
				return TupleUnpacker<C - 1, Ts...>(m_tuple).unpack(rest...);
			}

		private:
			Tuple& m_tuple;
		};

		/// Why do we need to specialize for both 0 and 1?
		template <typename T>
		struct TupleUnpacker<1, T> {
			TupleUnpacker(Tuple& tuple)
				: m_tuple(tuple)
			{

			}

			bool unpack(T ptr) {
				*ptr = m_tuple.get(m_tuple.size() - 1).to_native<std::remove_pointer_t<T>>();
				return true;
			}

		private:
			Tuple& m_tuple;
		};

		template <typename T>
		struct TupleUnpacker<0, T> {
			TupleUnpacker(Tuple& tuple)
			{}

			bool unpack() {
				return true;
			}
		};

		struct Tuple : public Object
		{
			Tuple(PyObject* pyObject)
				: Object(PyTuple_Check(pyObject) ? pyObject : nullptr)
				, m_size(PyTuple_Size(pyObject))
			{

			}

			template <Py_ssize_t Size>
			Tuple()
				: Object(PyTuple_New(Size))
				, m_size(Size)
			{

			}

			template <std::convertible_to<Object>... Ts>
			Tuple(Ts ...args)
				: Object(PyTuple_Pack(static_cast<Py_ssize_t>(sizeof...(Ts)), std::forward<PyObject*>(args)...))
				, m_size(sizeof...(Ts))
			{
				incref_all();
			}

			template <typename... Ts>
			Tuple(Ts ...args)
				: Tuple(AutoObject(args)...)
			{

			}

			~Tuple() {
				decref_all();
			}

			template <std::bidirectional_iterator... Ts>
			requires (sizeof...(Ts) > 0)
			bool unpack(Ts ...args) {
				return TupleUnpacker<sizeof...(Ts), Ts...>(*this).unpack(args...);
			}

			template <std::invocable<Object> T>
			Tuple& foreach(T && function) {
				for (int i = 0; i < m_size; ++i) {
					function(get(i));
				}
				return *this;
			}

			Tuple& incref_all() {
				return foreach([](Object object) { object.incref(); });
			}

			Tuple& decref_all() {
				return foreach([](Object object) { object.decref(); });
			}

			Py_ssize_t size() {
				return m_size;
			}

			Tuple slice(Py_ssize_t begin, Py_ssize_t end) {
				return Tuple(PyTuple_GetSlice(m_pyObject, begin, end));
			}

			Object get(Py_ssize_t index) {
				return Object(PyTuple_GetItem(m_pyObject, index));
			}

			Object operator[](Py_ssize_t index) {
				return Object(PyTuple_GetItem(m_pyObject, index));
			}

			static Tuple from_args(PyObject* args) {
				return Tuple(Object(args).incref().ptr());
			}

		private:
			Py_ssize_t m_size;
		};
	}
}
