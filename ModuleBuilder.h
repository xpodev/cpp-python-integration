#pragma once

#include "./PythonCore.h"

#include "./Module.h"
#include "./Utils.h"

#include <functional>
#include <vector>


namespace xpo {
	namespace python {
		struct ModuleBuilder {
			ModuleBuilder(char const* name, char const* doc = nullptr, Py_ssize_t size = -1)
				: m_name(name)
				, m_doc(doc)
				, m_size(size)
			{

			}

			void add_method(char const* name, PyCFunction function, char const* doc = nullptr) {
				PyMethodDef method_def = {
					name, function, METH_VARARGS, doc
				};
				m_methods.push_back(method_def);
			}

			template <IVarArgsFunction F>
			void add_method(char const* name, F && function, char const* doc = nullptr) {
				add_method(name, static_cast<PyCFunction>(function), doc);
			}

			template <IKWArgsFunction F>
			void add_method(char const* name, F && function, char const* doc = nullptr) {
				PyMethodDef method_def = {
					// usually, casting to void* and then to PyCFunction is dangerous, but here we rely on the Python
					// language to make sure the function is being called properly
					name, static_cast<PyCFunction>(static_cast<void*>(function)), METH_VARARGS | METH_KEYWORDS, doc
				};
				m_methods.push_back(method_def);
			}

			template <PackedVarArgsFunctionObject F>
			requires (IPackedVarArgsFunctionCall<F>)
			void add_method(char const* name, char const* doc = nullptr) {
				auto wrapper = [](PyObject* self, PyObject* args) {
					Object result = F.m_function(Module(self), F.make_tuple(Tuple::from_args(args)));
					if (result.is_none()) {
						Py_RETURN_NONE;
					}
					return result.ptr();
				};

				add_method(name, (PyCFunction)wrapper, doc);
			}

			template <VarArgsFunctionObject F>
			requires (IVarArgsFunctionCall<F>)
			void add_method(char const* name, char const* doc = nullptr) {
				auto wrapper = [](PyObject* self, PyObject* args) {
					Object result = std::apply(F.m_function, std::tuple_cat(std::make_tuple(Module(self)), F.make_tuple(Tuple::from_args(args))));
					if (result.is_none()) {
						Py_RETURN_NONE;
					}
					return result.ptr();
					Py_RETURN_NONE;
				};

				add_method(name, (PyCFunction)wrapper, doc);
			}

			template <Object(*function)(Module, Tuple)>
			void add_method(char const* name, char const* doc = nullptr) {
				auto wrapper = [](PyObject* self, PyObject* args) {
					Object result = function(Module(self), Tuple::from_args(args));
					if (result.is_none()) {
						Py_RETURN_NONE;
					}
					return result.ptr();
				};

				add_method(name, (PyCFunction)wrapper, doc);
			}

			template <typename ...Ts>
			void def(Ts && ...args) {
				add_method(std::forward<Ts>(args)...);
			}

			char const* name() const {
				return m_name;
			}

			void name(char const* new_name) {
				m_name = new_name;
			}

			char const* documentation() const {
				return m_doc;
			}

			void documentation(char const* new_documentation) {
				m_doc = new_documentation;
			}

			Py_ssize_t size() const {
				return m_size;
			}

			void name(Py_ssize_t new_size) {
				m_size = new_size;
			}

			void finalize_module() {
				m_methods.push_back({ nullptr, nullptr, 0, nullptr });
				m_module.m_methods = m_methods.data();
				m_module.m_name = m_name;
				m_module.m_doc = m_doc;
				m_module.m_size = m_size;
			}

			PyModuleDef& module_definition(bool finalize = true) {
				if (finalize) {
					finalize_module();
				}
				return m_module;
			}

		private:
			char const* m_name;
			char const* m_doc = nullptr;
			Py_ssize_t m_size = -1;

			std::vector<PyMethodDef> m_methods;
			PyModuleDef m_module = {
				PyModuleDef_HEAD_INIT,
				nullptr,
				nullptr,
				-1,
				nullptr
			};
		};
	}
}

#define DEFINE_MODULE_ALL_ARGS(name, varname, doc) \
static ModuleBuilder varname{ #name, doc }; \
void PyInit_ModuleInit_##name(); \
PyMODINIT_FUNC PyInit_##name() { \
	PyInit_ModuleInit_##name(); \
	return PyModule_Create(&##varname.module_definition()); \
} \
void PyInit_ModuleInit_##name()


// WTF is this? (it's a "macro overloading" thingy)
#define DEFINE_MODULE_2(name, varname) DEFINE_MODULE_ALL_ARGS(name, varname, nullptr)
#define DEFINE_MODULE_1(name) DEFINE_MODULE_2(name, m)

#define FUNC_CHOOSER(_f1, _f2, _f3, _f4, ...) _f4
#define FUNC_RECOMPOSER(argsWithParentheses) FUNC_CHOOSER argsWithParentheses
#define CHOOSE_FROM_ARG_COUNT(...) FUNC_RECOMPOSER((__VA_ARGS__, DEFINE_MODULE_ALL_ARGS, DEFINE_MODULE_2, DEFINE_MODULE_1, ))
#define MACRO_CHOOSER(...) CHOOSE_FROM_ARG_COUNT(__VA_ARGS__)

#define DEFINE_MODULE(...) MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define MODULE_INIT_FUNCTION_NAME(name) PyInit_##name

#define DECALRE_MODULE_INIT(name) \
PyMODINIT_FUNC MODULE_INIT_FUNCTION_NAME(name)()

