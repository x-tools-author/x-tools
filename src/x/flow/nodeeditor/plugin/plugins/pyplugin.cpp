/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <QApplication>
#include <QFileInfo>

#include "pyplugin.h"

PyPlugin::PyPlugin(const QString &libFile, QObject *parent)
    : AbstractPlugin(libFile, parent)
{}

PyPlugin::~PyPlugin()
{
    if (isRunning()) {
        quit();
        wait();
    }
}

bool PyPlugin::testLib(const QString &libFile)
{
    if (!QFile::exists(libFile)) {
        return false;
    }

    Py_SetPythonHome(PY_HOME);
    Py_Initialize();

    QFileInfo fileInfo(libFile);
    QString path = fileInfo.absolutePath();
    PyRun_SimpleString("import sys");
    QString pluginPath = QString("sys.path.append('%1')").arg(path);
    PyRun_SimpleString(pluginPath.toStdString().c_str());

    QString baseName = fileInfo.baseName();
    PyObject *pModule = PyImport_ImportModule(baseName.toStdString().c_str());
    if (!pModule) {
        qWarning() << "Failed to load module" << fileInfo.baseName();
        PyErr_Print();
        Py_Finalize();
        return false;
    }

    PyPluginApiV1 apis;
    PyObject *pFunc = PyObject_GetAttrString(pModule, apis.apiVersion.toStdString().c_str());
    if (!pFunc || !PyCallable_Check(pFunc)) {
        qWarning() << "Failed to get function" << apis.apiVersion;
        PyErr_Print();
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return false;
    }

    PyObject *pValue = PyObject_CallObject(pFunc, nullptr);
    if (!pValue) {
        qWarning() << "Failed to call function" << apis.apiVersion;
        PyErr_Print();
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return false;
    }

    if (!PyUnicode_Check(pValue)) {
        PyErr_Print();
        Py_XDECREF(pValue);
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return false;
    }

    QString version = QString::fromStdString(PyUnicode_AsUTF8(pValue));
    if (version != "1.0") {
        Py_XDECREF(pValue);
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return false;
    }

    Py_XDECREF(pValue);
    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);
    Py_Finalize();

    return true;
}

QString PyPlugin::caption() const
{
    Py_SetPythonHome(PY_HOME);
    Py_Initialize();

    QFileInfo fileInfo(m_libFile);
    QString path = fileInfo.absolutePath();
    PyRun_SimpleString("import sys");
    QString pluginPath = QString("sys.path.append('%1')").arg(path);
    PyRun_SimpleString(pluginPath.toStdString().c_str());

    QString baseName = fileInfo.baseName();
    PyObject *pModule = PyImport_ImportModule(baseName.toStdString().c_str());
    if (!pModule) {
        qWarning() << "Failed to load module" << fileInfo.baseName();
        PyErr_Print();
        Py_Finalize();
        return QString(__FUNCTION__);
    }

    PyPluginApiV1 apis;
    PyObject *pFunc = PyObject_GetAttrString(pModule, apis.pluginCaption.toStdString().c_str());
    if (!pFunc || !PyCallable_Check(pFunc)) {
        qWarning() << "Failed to get function" << apis.pluginCaption;
        PyErr_Print();
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return QString(__FUNCTION__);
    }

    PyObject *pValue = PyObject_CallObject(pFunc, nullptr);
    if (!pValue) {
        qWarning() << "Failed to call function" << apis.pluginCaption;
        PyErr_Print();
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return QString(__FUNCTION__);
    }

    if (!PyUnicode_Check(pValue)) {
        PyErr_Print();
        Py_XDECREF(pValue);
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return QString(__FUNCTION__);
    }

    QString version = QString::fromStdString(PyUnicode_AsUTF8(pValue));
    Py_XDECREF(pValue);
    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);
    Py_Finalize();

    return version;
}

QString PyPlugin::name() const
{
    Py_SetPythonHome(PY_HOME);
    Py_Initialize();

    QFileInfo fileInfo(m_libFile);
    QString path = fileInfo.absolutePath();
    PyRun_SimpleString("import sys");
    QString pluginPath = QString("sys.path.append('%1')").arg(path);
    PyRun_SimpleString(pluginPath.toStdString().c_str());

    QString baseName = fileInfo.baseName();
    PyObject *pModule = PyImport_ImportModule(baseName.toStdString().c_str());
    if (!pModule) {
        qWarning() << "Failed to load module" << fileInfo.baseName();
        PyErr_Print();
        Py_Finalize();
        return QString(__FUNCTION__);
    }

    PyPluginApiV1 apis;
    PyObject *pFunc = PyObject_GetAttrString(pModule, apis.pluginName.toStdString().c_str());
    if (!pFunc || !PyCallable_Check(pFunc)) {
        qWarning() << "Failed to get function" << apis.pluginName;
        PyErr_Print();
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return QString(__FUNCTION__);
    }

    PyObject *pValue = PyObject_CallObject(pFunc, nullptr);
    if (!pValue) {
        qWarning() << "Failed to call function" << apis.pluginName;
        PyErr_Print();
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return QString(__FUNCTION__);
    }

    if (!PyUnicode_Check(pValue)) {
        PyErr_Print();
        Py_XDECREF(pValue);
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return QString(__FUNCTION__);
    }

    QString name = QString::fromStdString(PyUnicode_AsUTF8(pValue));
    Py_XDECREF(pValue);
    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);
    Py_Finalize();

    return name;
}

unsigned int PyPlugin::inPorts() const
{
    Py_SetPythonHome(PY_HOME);
    Py_Initialize();

    QFileInfo fileInfo(m_libFile);
    QString path = fileInfo.absolutePath();
    PyRun_SimpleString("import sys");
    QString pluginPath = QString("sys.path.append('%1')").arg(path);
    PyRun_SimpleString(pluginPath.toStdString().c_str());

    QString baseName = fileInfo.baseName();
    PyObject *pModule = PyImport_ImportModule(baseName.toStdString().c_str());
    if (!pModule) {
        qWarning() << "Failed to load module" << fileInfo.baseName();
        PyErr_Print();
        Py_Finalize();
        return 0;
    }

    PyPluginApiV1 apis;
    PyObject *pFunc = PyObject_GetAttrString(pModule, apis.pluginInPorts.toStdString().c_str());
    if (!pFunc || !PyCallable_Check(pFunc)) {
        qWarning() << "Failed to get function" << apis.pluginInPorts;
        PyErr_Print();
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return 0;
    }

    PyObject *pValue = PyObject_CallObject(pFunc, nullptr);
    if (!pValue) {
        qWarning() << "Failed to call function" << apis.pluginInPorts;
        PyErr_Print();
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return 0;
    }

    int ports = PyLong_AsLong(pValue);
    Py_XDECREF(pValue);
    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);
    Py_Finalize();

    return ports;
}

unsigned int PyPlugin::outPorts() const
{
    Py_SetPythonHome(PY_HOME);
    Py_Initialize();

    QFileInfo fileInfo(m_libFile);
    QString path = fileInfo.absolutePath();
    PyRun_SimpleString("import sys");
    QString pluginPath = QString("sys.path.append('%1')").arg(path);
    PyRun_SimpleString(pluginPath.toStdString().c_str());

    QString baseName = fileInfo.baseName();
    PyObject *pModule = PyImport_ImportModule(baseName.toStdString().c_str());
    if (!pModule) {
        qWarning() << "Failed to load module" << fileInfo.baseName();
        PyErr_Print();
        Py_Finalize();
        return 0;
    }

    PyPluginApiV1 apis;
    PyObject *pFunc = PyObject_GetAttrString(pModule, apis.pluginOutPorts.toStdString().c_str());
    if (!pFunc || !PyCallable_Check(pFunc)) {
        qWarning() << "Failed to get function" << apis.pluginOutPorts;
        PyErr_Print();
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return 0;
    }

    PyObject *pValue = PyObject_CallObject(pFunc, nullptr);
    if (!pValue) {
        qWarning() << "Failed to call function" << apis.pluginOutPorts;
        PyErr_Print();
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return 0;
    }

    int ports = PyLong_AsLong(pValue);
    Py_XDECREF(pValue);
    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);
    Py_Finalize();

    return ports;
}

QByteArray PyPlugin::handleData(QByteArray const &data, int const index)
{
    Py_SetPythonHome(PY_HOME);
    Py_Initialize();

    QFileInfo fileInfo(m_libFile);
    QString path = fileInfo.absolutePath();
    PyRun_SimpleString("import sys");
    QString pluginPath = QString("sys.path.append('%1')").arg(path);
    PyRun_SimpleString(pluginPath.toStdString().c_str());

    QString baseName = fileInfo.baseName();
    PyObject *pModule = PyImport_ImportModule(baseName.toStdString().c_str());
    if (!pModule) {
        qWarning() << "Failed to load module" << fileInfo.baseName();
        PyErr_Print();
        Py_Finalize();
        return QByteArray();
    }

    PyPluginApiV1 apis;
    PyObject *pFunc = PyObject_GetAttrString(pModule, apis.pluginHandle.toStdString().c_str());
    if (!pFunc || !PyCallable_Check(pFunc)) {
        qWarning() << "Failed to get function" << apis.pluginHandle;
        PyErr_Print();
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return QByteArray();
    }

    PyObject *pArgs = PyTuple_Pack(2, PyBytes_FromString(data.constData()), PyLong_FromLong(index));
    PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
    if (!pValue) {
        qWarning() << "Failed to call function" << apis.pluginHandle;
        PyErr_Print();
        Py_XDECREF(pArgs);
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return QByteArray();
    }

    if (!PyBytes_Check(pValue)) {
        qWarning() << "Invalid return type from function" << apis.pluginHandle;
        PyErr_Print();
        Py_XDECREF(pArgs);
        Py_XDECREF(pValue);
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
        Py_Finalize();
        return QByteArray();
    }

    QByteArray result = QByteArray(PyBytes_AsString(pValue));
    Py_XDECREF(pArgs);
    Py_XDECREF(pValue);
    Py_XDECREF(pFunc);
    Py_XDECREF(pModule);
    Py_Finalize();

    return result;
}
