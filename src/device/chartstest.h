/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMutex>
#include <QThread>
#include <QVariantMap>

#include "device.h"

struct ChartsTestDataKeys
{
    const QString dataFormat{"dataFormat"};
    const QString channels{"channels"};
};

class ChartsTest : public Device
{
    Q_OBJECT
public:
    explicit ChartsTest(QObject *parent = nullptr);
    ~ChartsTest();

protected:
    void run() override;

private:
    QByteArray generateBinaryY(int channels);
    QByteArray generateTextY(int channels);
    QByteArray generateBinaryXY(int channels);
    QByteArray generateTextXY(int channels);

private:
    const QByteArray m_binaryTail;
    int m_testAngle{0};
    int m_channels{16};
};
