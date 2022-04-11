#pragma once

#include <Python.h>

#include "./Object.h"
#include "./Tuple.h"

#include <concepts>

namespace xpo::python {

struct Module;

template <typename... Ts>
using PackedVarArgsFunction = Object(*)(Module, TypedTuple<Ts...>);


template <typename... Ts>
using VarArgsFunction = Object(*)(Module, Ts...);


template <typename... Ts>
requires (sizeof...(Ts) > 0)
struct VarArgsFunctionObject {
	using typed_tuple = TypedTuple<Ts...>;
	using function_type = VarArgsFunction<Ts...>;

	constexpr VarArgsFunctionObject(function_type function)
		: m_function{ function }
	{

	}

	static constexpr inline std::tuple<Ts...> args() { return std::declval<std::tuple<Ts...>>(); }

	inline std::tuple<Ts...> make_tuple(Tuple const&& tuple) const { return tuple.to_tuple<Ts...>(); }

	function_type m_function;
};


template <typename... Ts>
requires (sizeof...(Ts) > 0)
struct PackedVarArgsFunctionObject {
	using typed_tuple = TypedTuple<Ts...>;
	using function_type = PackedVarArgsFunction<Ts...>;

	constexpr PackedVarArgsFunctionObject(function_type function)
		: m_function{ function }
	{

	}

	static constexpr inline typed_tuple args() { return std::declval<typed_tuple>(); }

	inline typed_tuple make_tuple(Tuple const&& tuple) const { return typed_tuple(tuple.ptr()); }

	function_type m_function;
};


template <class T>
concept IVarArgsFunction = requires (T t, PyObject * p) {
	{ t(p, p) } -> std::convertible_to<PyObject*>;
};

template <class T>
concept IKWArgsFunction = requires (T t, PyObject * p) {
	{ t(p, p, p) } -> std::convertible_to<PyObject*>;
};

template <VarArgsFunctionObject P>
concept IVarArgsFunctionCall = requires (Module m) {
	{ std::apply(P.m_function, std::tuple_cat(std::make_tuple(m), P.args())) } -> std::convertible_to<Object>;
};

template <PackedVarArgsFunctionObject P>
concept IPackedVarArgsFunctionCall = requires (Module m) {
	{ P.m_function(m, P.args()) } -> std::convertible_to<Object>;
};


struct Module;

}
