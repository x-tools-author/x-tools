/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QByteArray>
#include <QComboBox>
#include <QList>
#include <QMutex>
#include <QPointF>
#include <QThread>
#include <QVariantMap>

class PlotDataHandler : public QThread
{
    Q_OBJECT
public:
    enum class DataFormat { Binary, Text };
    Q_ENUM(DataFormat);

public:
    PlotDataHandler(QObject *parent = Q_NULLPTR);
    virtual ~PlotDataHandler() override;

    int dataFormat() const;
    void setDataFormat(int type);

    void inputBytes(const QByteArray &bytes);
    void setupDataFormat(QComboBox *comboBox);

signals:
    void newValues(const QList<double> &values);

protected:
    void run() override;

    void handleBinary(QByteArray &bytes);
    void handleText(QByteArray &bytes);

private:
    const QByteArray m_binaryTail;
    QByteArray m_cache;
    QMutex m_cacheMutex;
    int m_dataFormat;
    mutable QMutex m_dataFormatMutex;
};
