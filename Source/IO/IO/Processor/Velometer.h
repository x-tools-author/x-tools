/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QList>
#include <QMutex>

#include "../AbstractIO.h"

class Velometer : public AbstractIO
{
    Q_OBJECT
    Q_PROPERTY(QString speed READ speed NOTIFY speedChanged)
public:
    explicit Velometer(QObject *parent = nullptr);

    void inputBytes(const QByteArray &bytes) override;

protected:
    void run() override;

private:
    QByteArray m_inputBytes;
    QMutex mInputBytesContextListMutex;
    QString mVelocity;
    QMutex mVelocityMutex;

private:
    QString speed();

signals:
    void speedChanged(const QString &velocity);
};
