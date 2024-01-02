/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QDateTime>
#include <QtGlobal>

#include "sakbasetool.h"

SAKBaseTool::SAKBaseTool(QObject *parent)
    : QThread{parent}
{
    connect(this, &SAKBaseTool::started, this, [=]() {
        this->m_isWorking = true;
        emit this->isWorkingChanged();
    });
    connect(this, &SAKBaseTool::finished, this, [=]() {
        this->m_isWorking = false;
        emit this->isWorkingChanged();
    });
    connect(this, &SAKBaseTool::errorOccured, this, [=](const QString &errorString) {
        qWarning() << "Error occured: " << qPrintable(errorString);
        exit();
        wait();
    });
}

SAKBaseTool::~SAKBaseTool()
{
    if (isRunning()) {
        exit();
        wait();
    }
}

void SAKBaseTool::inputBytes(const QByteArray &bytes)
{
    emit bytesOutput(bytes);
}
