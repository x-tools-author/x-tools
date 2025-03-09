/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

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

    void inputBytes(const QByteArray &bytes);
    QVariantMap save();
    void load(const QVariantMap &parameters);

signals:
    void newValues(const QList<double> &values);
    void newPoints(const QList<QPointF> &points);
    void outputBytes(const QByteArray &bytes);

protected:
    void run() override;

    void handleBinaryY(QByteArray &bytes);
    void handleTextY(QByteArray &bytes);
    void handleBinaryXY(QByteArray &bytes);
    void handleTextXY(QByteArray &bytes);

private:
    Q_SIGNAL void input2run(const QByteArray &bytes);

private:
    QByteArray m_binaryTail;
    QByteArray m_cache;
};
