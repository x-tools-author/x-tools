/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMutex>

#include "page/common/abstractio.h"

class Statistician : public AbstractIO
{
    Q_OBJECT
    Q_PROPERTY(int frames READ frames NOTIFY framesChanged)
    Q_PROPERTY(int bytes READ bytes NOTIFY bytesChanged)
    Q_PROPERTY(int speed READ speed NOTIFY speedChanged)
public:
    explicit Statistician(QObject *parent = nullptr);

    void inputBytes(const QByteArray &bytes) override;

    int frames();
    int bytes();
    int speed();

    QString framesString();
    QString bytesString();
    QString speedString();

signals:
    void framesChanged();
    void bytesChanged();
    void speedChanged();

protected:
    virtual void run() final;

private:
    int m_frames{0};
    int m_bytes{0};
    int m_speed{0};

    QByteArray m_tempBytes;

private:
    void updateSpeed();
};
