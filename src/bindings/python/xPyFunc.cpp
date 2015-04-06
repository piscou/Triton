
#include <iostream>
#include "xPyFunc.h"



static void notEnoughMemory(void)
{
  std::cerr << "[ERROR] Not enough memory for allocation" << std::endl;
  exit(-1);
}


PyObject *xPyDict_New(void)
{
  PyObject *dict = PyDict_New();
  if (!dict)
    notEnoughMemory();
  return dict;
}


PyObject *xPyList_New(Py_ssize_t len)
{
  PyObject *list = PyList_New(len);
  if (!list)
    notEnoughMemory();
  return list;
}


PyObject *xPyTuple_New(Py_ssize_t len)
{
  PyObject *tuple = PyTuple_New(len);
  if (!tuple)
    notEnoughMemory();
  return tuple;
}


PyObject *xPyString_FromString(const char *v)
{
  PyObject *s = PyString_FromString(v);
  if (!s)
    notEnoughMemory();
  return s;
}


PyObject *xPyClass_New(PyObject *h, PyObject *d, PyObject *n)
{
  PyObject *c = PyClass_New(h, d, n);
  if (!c)
    notEnoughMemory();
  return c;
}
