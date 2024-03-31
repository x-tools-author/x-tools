/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsCommunicationTool.h"

#include <QDebug>
#include <QTimer>

xToolsCommunicationTool::xToolsCommunicationTool(QObject *parent)
    : xToolsBaseTool{parent}
{}

xToolsCommunicationTool::~xToolsCommunicationTool() {}

void xToolsCommunicationTool::inputBytes(const QByteArray &bytes)
{
    if (!isEnable()) {
        return;
    }

    m_inputBytesMutex.lock();
    m_inputBytesList.append(bytes);
    m_inputBytesMutex.unlock();
}

void xToolsCommunicationTool::run()
{
    QString errStr;
    if (!initialize(errStr)) {
        qWarning() << errStr;
        emit errorOccurred(errStr);
        return;
    }

    QTimer *txTimer = new QTimer();
    txTimer->setInterval(5);
    txTimer->setSingleShot(true);
    connect(txTimer, &QTimer::timeout, txTimer, [=]() {
        this->m_inputBytesMutex.lock();
        if (!m_inputBytesList.isEmpty()) {
            auto bytes = m_inputBytesList.takeFirst();
            writeBytes(bytes);
        }
        this->m_inputBytesMutex.unlock();
        txTimer->start();
    });
    txTimer->start();

    exec();
    txTimer->deleteLater();
    txTimer = nullptr;
    uninitialize();
}
