/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
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
        this->mIsWorking = true;
        emit this->isWorkingChanged();
    });
    connect(this, &SAKBaseTool::finished, this, [=]() {
        this->mIsWorking = false;
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
        outputMessage(QtDebugMsg, __FUNCTION__);
    }
}

void SAKBaseTool::inputBytes(const QByteArray &bytes, const QVariant &context)
{
    emit bytesInputted(bytes, context);
}

void SAKBaseTool::outputMessage(int type, const QString &info) const
{
    if (type == QtInfoMsg) {
        qInfo() << qPrintable(info);
    } else if (type == QtDebugMsg) {
        qDebug() << qPrintable(info);
    } else if (type == QtWarningMsg) {
        qWarning() << qPrintable(info);
    } else {
        qCritical() << qPrintable(info);
    }
}
