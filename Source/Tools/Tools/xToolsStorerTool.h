/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMutex>
#include <QTimer>

#include "xToolsBaseTool.h"

class xToolsStorerTool : public xToolsBaseTool
{
    Q_OBJECT
    Q_PROPERTY(bool saveRx READ saveRx WRITE setSaveRx NOTIFY saveRxChanged)
    Q_PROPERTY(bool saveTx READ saveTx WRITE setSaveTx NOTIFY saveTxChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
public:
    explicit xToolsStorerTool(QObject *parent = nullptr);
    ~xToolsStorerTool();

    virtual void inputBytes(const QByteArray &bytes) override;

public:
    bool saveRx();
    void setSaveRx(bool save);
    bool saveTx();
    void setSaveTx(bool save);
    QString fileName();
    void setFileName(const QString &name);

signals:
    void saveRxChanged();
    void saveTxChanged();
    void fileNameChanged();

protected:
    virtual void run() final;

private:
    struct Parameters
    {
        QString file;
        bool saveRx;
        bool saveTx;
    } m_parameters;
    QMutex m_parametersMutex;

    QList<QByteArray> m_inputBytesList;
    QMutex m_inputBytesListMutex;

private:
    void write2file();
};
