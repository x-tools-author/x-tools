/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKVELOMETERTOOL_H
#define SAKVELOMETERTOOL_H

#include "sakbasetool.h"
#include <QList>
#include <QMutex>

class SAKVelometerTool : public SAKBaseTool
{
    Q_OBJECT
    Q_PROPERTY(QString velocity READ velocity NOTIFY velocityChanged)
public:
    explicit SAKVelometerTool(QObject *parent = nullptr);

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

#endif // SAKVELOMETERTOOL_H
