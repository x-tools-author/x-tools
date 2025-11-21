/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "javascriptthread.h"

#include <QFile>
#include <QJSEngine>
#include <QThread>

#include "javascript.h"
#include "javascriptui.h"

JavaScriptThread::JavaScriptThread(JavaScript *js)
    : QThread(js)
    , m_js(js)
{}

JavaScriptThread::~JavaScriptThread()
{
    if (isRunning()) {
        quit();
        wait();
    }
}

void JavaScriptThread::run()
{
    QObject *obj = new QObject();
    QJSEngine *engine = new QJSEngine();
    connect(m_js,
            &JavaScript::input2thread,
            obj,
            [=](std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex const portIndex) {
                this->onInput2Thread(engine, nodeData, portIndex);
            });

    exec();
    engine->deleteLater();
    obj->deleteLater();
}

void JavaScriptThread::onInput2Thread(QJSEngine *engine,
                                      std::shared_ptr<QtNodes::NodeData> nodeData,
                                      QtNodes::PortIndex const)
{
    auto baseNodeData = std::dynamic_pointer_cast<BaseNodeData>(nodeData);
    if (!baseNodeData) {
        return;
    }

    QByteArray bytes = baseNodeData->bytes();
    JavaScriptParametersKeys keys;
    QJsonObject parameters = m_js->save();
    const QString script = parameters[keys.script].toString();
    if (script.isEmpty()) {
        qWarning("The java script is empty.");
        return;
    }

    QString tmp = QString("(%1)").arg(script);
    QJSValue func = engine->evaluate(tmp);
    if (func.isError() || func.isUndefined() || func.isNull()) {
        qInfo() << "The script is invalid.";
    }

    QJSValueList args;
    args << QString::fromLatin1(bytes.toHex());
    QJSValue ret = func.call(args);
    if (ret.isError()) {
        qWarning() << script;
        m_js->setMessage(tr("The script is invalid."));
        return;
    }

    QByteArray result = QByteArray::fromHex(ret.toString().toLatin1());
    if (result.isEmpty()) {
        qWarning("The script output is empty.");
        return;
    }

    m_js->setOutData(std::make_shared<BaseNodeData>(result));
    emit m_js->dataUpdated(0);
}