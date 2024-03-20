/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "xToolsBaseTool.h"
#include <QList>
#include <QMutex>

class xToolsVelometerTool : public xToolsBaseTool
{
    Q_OBJECT
    Q_PROPERTY(QString velocity READ velocity NOTIFY velocityChanged)
public:
    explicit xToolsVelometerTool(QObject *parent = nullptr);

    void inputBytes(const QByteArray &bytes) override;

protected:
    void run() override;

private:
    QList<QByteArray> mInputBytesContextList;
    QMutex mInputBytesContextListMutex;
    QString mVelocity;
    QMutex mVelocityMutex;

private:
    QString velocity();

signals:
    void velocityChanged(const QString &velocity);
};
