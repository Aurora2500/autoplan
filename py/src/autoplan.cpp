#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "representation.hpp"

#include <iostream>

typedef struct {
	PyObject_HEAD
	autoplan::domain domain;
} domain;

static PyObject*
hello_world(PyObject *self, PyObject *args) {
	std::cout << "Hello, world!" << std::endl;

	return Py_None;
}

PyObject*
domain_new(PyTypeObject *type, PyObject *args, PyObject *kwargs) {
	domain *self = (domain*)type->tp_alloc(type, 0);
	if (self == nullptr) {
		return nullptr;
	}

	self->domain = autoplan::domain();

	return (PyObject*)self;
}

PyTypeObject domain_type = {
	.ob_base = PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "autoplan.domain",
	.tp_basicsize = sizeof(domain),
	.tp_itemsize = 0,
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_doc = "Autoplan domain",

	.tp_new = domain_new,
};

extern "C" {

static PyMethodDef autoplan_methods[] = {
	{"hello", hello_world, METH_VARARGS},
	{NULL, NULL, 0, NULL}
};

static PyModuleDef autoplan_module = {
	PyModuleDef_HEAD_INIT,
	"autoplan",
	"An automated planning library"
	-1,
	.m_methods = autoplan_methods,
};

PyMODINIT_FUNC
PyInit_autoplan(void) {
	if (PyType_Ready(&domain_type) < 0) {
		return nullptr;
	}
	PyObject *m = PyModule_Create(&autoplan_module);
	if (m == nullptr) {
		return nullptr;
	}

	if (PyModule_AddObject(m, "domain", (PyObject*)&domain_type) < 0) {
		Py_DECREF(&domain_type);
		Py_DECREF(m);
		return nullptr;
	}

	return m;
}

}