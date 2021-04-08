#pragma once

#define PY_VERSION 37

#include "./PythonCore.h"

#include "./Module.h"
#include "./String.h"


namespace GameCore {
	namespace Python {
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
