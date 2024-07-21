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

    if (!isWorking()) {
        return;
    }

    emit invokeWriteBytes(bytes);
}

void xToolsCommunicationTool::run()
{
    QString errStr;
    if (!initialize(errStr)) {
        qWarning() << errStr;
        emit errorOccurred(errStr);
        return;
    }

    QObject *obj = new QObject();
    connect(this, &xToolsCommunicationTool::invokeWriteBytes, obj, [this](const QByteArray &bytes) {
        this->writeBytes(bytes);
    });

    exec();

    obj->deleteLater();
    deinitialize();
}
