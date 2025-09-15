/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "scriptrunnerjs.h"

#include <QDebug>
#include <QEventLoop>
#include <QFile>
#include <QTimer>

ScriptRunnerJs::ScriptRunnerJs(QObject *parent)
    : ScriptRunner(parent)
{}

ScriptRunnerJs::~ScriptRunnerJs() {}

void ScriptRunnerJs::onBytesRead(const QByteArray &data)
{
    if (data.isEmpty()) {
        return;
    }

    if (m_engine == nullptr) {
        return;
    }

    QJSValue jsFunc = m_engine->globalObject().property("onBytesRead");
    if (jsFunc.isCallable()) {
        QJSValueList args;
        args << QString::fromUtf8(data);
        QJSValue result = jsFunc.call(args);
        if (result.isError()) {
            QString errorMsg = tr("Uncaught exception at line %1: %2")
                                   .arg(result.property("lineNumber").toInt())
                                   .arg(result.toString());
            emit logOutput(errorMsg);
        }
    }
}

void ScriptRunnerJs::xWriteBytes(const QJSValue &value)
{
    if (value.isString()) {
        emit invokeWrite(value.toString().toUtf8());
    } else if (value.isArray()) {
        QJSValue lengthValue = value.property("length");
        if (lengthValue.isNumber()) {
            int length = lengthValue.toInt();
            QByteArray byteArray;
            byteArray.reserve(length);
            for (int i = 0; i < length; ++i) {
                QJSValue element = value.property(i);
                if (element.isNumber()) {
                    int byte = element.toInt();
                    if (byte < 0 || byte > 255) {
                        emit logOutput(tr("Array element at index %1 is out of byte range: %2")
                                           .arg(i)
                                           .arg(byte));
                        return;
                    }
                    byteArray.append(static_cast<char>(byte));
                } else {
                    emit logOutput(tr("Array element at index %1 is not a number").arg(i));
                    return;
                }
            }
            emit invokeWrite(byteArray);
        } else {
            emit logOutput(tr("The 'length' property of the array is not a number"));
        }
    } else {
        emit logOutput(tr("xWriteBytes expects a string or an array of bytes"));
    }
}

bool ScriptRunnerJs::xIsInterruptionRequested()
{
    return isInterruptionRequested();
}

void ScriptRunnerJs::xSleep(int ms)
{
    if (ms <= 0) {
        ms = 10;
    }

    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(ms);
    loop.exec();
}

void ScriptRunnerJs::xPrint(const QString &text)
{
    emit logOutput(text);
}

void ScriptRunnerJs::run()
{
    QFile scriptFile(m_scriptFile);
    if (!scriptFile.open(QIODevice::ReadOnly)) {
        emit logOutput(tr("Cannot open script file: %1").arg(m_scriptFile));
        return;
    }

    QString script = QString::fromUtf8(scriptFile.readAll());
    scriptFile.close();

    m_engine = new QJSEngine();
    m_engine->installExtensions(QJSEngine::AllExtensions);
    QJSValue obj = m_engine->newQObject(this);
    m_engine->globalObject().setProperty("jsRunner", obj);
    QStringList exceptionStack;
    QJSValue result = m_engine->evaluate(script, m_scriptFile, 1, &exceptionStack);
    if (result.isError()) {
        QString errorMsg = tr("Uncaught exception at line %1: %2")
                               .arg(result.property("lineNumber").toInt())
                               .arg(result.toString());
        emit logOutput(errorMsg);
        if (!exceptionStack.isEmpty()) {
            emit logOutput(tr("Stack trace:"));
            for (const QString &line : qAsConst(exceptionStack)) {
                emit logOutput(line);
            }
        }
    } else {
        emit logOutput(tr("Script executed successfully."));
    }

    m_engine->deleteLater();
    m_engine = nullptr;
}