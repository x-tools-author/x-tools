/*********************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include <QtGlobal>
#include <QDateTime>

#include "SAKBaseTool.h"

SAKBaseTool::SAKBaseTool(const char *logCategory, QObject *parent)
    : QThread{parent}
    , mLoggingCategory(logCategory)
{
    connect(this, &SAKBaseTool::started, this, [=](){
        this->mIsWorking = true;
        emit this->isWorkingChanged();
    });
    connect(this, &SAKBaseTool::finished, this, [=](){
        this->mIsWorking = false;
        emit this->isWorkingChanged();
    });
    connect(this, &SAKBaseTool::errorOccured, this, [=](){
        exit();
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
        qCInfo(mLoggingCategory) << qPrintable(info);
    } else if (type == QtDebugMsg) {
        qCDebug(mLoggingCategory) << qPrintable(info);
    } else if (type == QtWarningMsg) {
        qCWarning(mLoggingCategory) << qPrintable(info);
    } else {
        qCCritical(mLoggingCategory) << qPrintable(info);
    }
}
