/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMutex>

#include "../AbstractIO.h"

class Counter : public AbstractIO
{
    Q_OBJECT
    Q_PROPERTY(int frames READ frames NOTIFY framesChanged)
    Q_PROPERTY(int bytes READ bytes NOTIFY bytesChanged)
    Q_PROPERTY(int speed READ speed NOTIFY speedChanged)

    Q_PROPERTY(QString framesString READ framesString NOTIFY framesStringChanged FINAL)
    Q_PROPERTY(QString bytesString READ bytesString NOTIFY bytesStringChanged FINAL)
    Q_PROPERTY(QString speedString READ speedString NOTIFY speedStringChanged FINAL)

    Q_PROPERTY(QString info READ info NOTIFY infoChanged FINAL)
public:
    explicit Counter(QObject *parent = nullptr);

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

    void framesStringChanged();
    void bytesStringChanged();
    void speedStringChanged();

    void infoChanged();

protected:
    virtual void run() final;

private:
    int m_frames{0};
    int m_bytes{0};
    int m_speed{0};

    QString m_framesString;
    QString m_bytesString;
    QString m_speedString;

    QString m_info;
    QMutex m_speedMutex;
    QByteArray m_tempBytes;

private:
    void updateSpeed();
};
