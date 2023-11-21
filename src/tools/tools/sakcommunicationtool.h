/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKCOMMUNICATIONTOOL_H
#define SAKCOMMUNICATIONTOOL_H

#include "sakbasetool.h"
#include <atomic>
#include <QMutex>

class SAKCommunicationTool : public SAKBaseTool
{
    Q_OBJECT
public:
    explicit SAKCommunicationTool(const char *logCategory, QObject *parent = nullptr);
    virtual void inputBytes(const QByteArray &bytes, const QVariant &context = QJsonObject()) final;

protected:
    struct InputDataContext
    {
        QByteArray bytes;
        QVariant context;
    };

protected:
    virtual void run() final;

    virtual bool initialize(QString &errStr) = 0;
    virtual void writeBytes(const QByteArray &bytes, const QVariant &context = QJsonObject()) = 0;
    virtual void uninitialize() = 0;

    QJsonObject rxJsonObject() const;
    QJsonObject txJsonObject() const;

private:
    QList<InputDataContext> mInputDataList;
    QMutex mInputDataMutex;
};

#endif // SAKCOMMUNICATIONTOOL_H
