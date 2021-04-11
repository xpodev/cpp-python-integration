#pragma once

#undef _DEBUG
#undef _module

#ifdef X_PY_VERSION

#define STR(x) #x
#define XSTR(x) STR(x)

/* here's the magic */
#define _CONCAT(x,y) x##y
#define CONCAT(x,y) _CONCAT(x,y)

#define PYTHON_LIB_PATH CONCAT(/PythonLibs/python, X_PY_VERSION).lib

//#define AUTO_INCLUDE_LIB

#ifdef AUTO_INCLUDE_LIB
#pragma comment(lib, XSTR(PYTHON_LIB_PATH))
#endif

#undef _CONCAT
#undef CONCAT
#undef XSTR
#undef STR

#endif // PY_VERSION

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "./PythonCore.h"

#include "./Module.h"
#include "./String.h"


namespace xpo {
	namespace python {
		struct Python final {
		private:
			struct SystemInfo {
				char const* version() const {
					return Py_GetVersion();
				}

				char const* platform() const {
					return Py_GetPlatform();
				}

				char const* copyright() const {
					return Py_GetCopyright();
				}

				char const* compiler() const {
					return Py_GetCompiler();
				}

				char const* build_info() const {
					return Py_GetBuildInfo();
				}
			};

		public:
			~Python() {
				if (is_running()) {
					stop();
				}
			}

		public:
			void start() {
				Py_Initialize();
			}

			void start(bool initSignals) {
				Py_InitializeEx(initSignals);
			}

			void stop() {
				Py_Finalize();
			}

			bool is_running() const {
				return Py_IsInitialized();
			}

			void add_module(char const* name, PyObject* (*module_init)()) {
				PyImport_AppendInittab(name, module_init);
			}

			Module main_module() {
				return Module(PyImport_AddModule("__main__"));
			}

			void set_global(char const* name, Object object) {
				main_module().set_attribute(name, object);
			}

			Object get_global(char const* name) {
				return main_module().get_attribute(name);
			}

			void add_module_search_path(char const* path) {
				PyList_Append(PySys_GetObject("path"), String(path));
			}

			int run_string(char const* str) {
				return PyRun_SimpleString(str);
			}

			int run_file(char const* filePath) {
				int result = -1;
				auto file = _Py_fopen(filePath, "rb");
				if (file) {
					result = PyRun_AnyFileEx(file, filePath, 1);
				}
				return result;
			}

			SystemInfo const& system_info() const {
				return m_systemInfo;
			}

		private:
			SystemInfo m_systemInfo;
		};
	}
}
