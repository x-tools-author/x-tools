/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QComboBox>
#include <QMutex>
#include <QThread>
#include <QVariantMap>

class ChartDataHandler : public QThread
{
    Q_OBJECT
public:
    enum class DataFormat { BinaryY, BinaryXY, TextY, TextXY };
    Q_ENUM(DataFormat);

public:
    ChartDataHandler(QObject *parent = Q_NULLPTR);
    virtual ~ChartDataHandler() override;

    int dataFormat() const;
    void setDataFormat(int type);

    void inputBytes(const QByteArray &bytes);
    void setupDataFormat(QComboBox *comboBox);

signals:
    void newValues(const QList<double> &values);
    void newPoints(const QList<QPointF> &points);

protected:
    void run() override;

    void handleBinaryY(QByteArray &bytes);
    void handleTextY(QByteArray &bytes);
    void handleBinaryXY(QByteArray &bytes);
    void handleTextXY(QByteArray &bytes);

private:
    const QByteArray m_binaryTail;
    QByteArray m_cache;
    QMutex m_cacheMutex;
    int m_dataFormat;
    mutable QMutex m_dataFormatMutex;
};
