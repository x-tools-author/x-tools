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
    Q_PROPERTY(int saveFormat READ saveFormat WRITE setSaveFormat NOTIFY saveFormatChanged)
    Q_PROPERTY(bool saveRx READ saveRx WRITE setSaveRx NOTIFY saveRxChanged)
    Q_PROPERTY(bool saveTx READ saveTx WRITE setSaveTx NOTIFY saveTxChanged)
    Q_PROPERTY(bool saveDate READ saveDate WRITE setSaveDate NOTIFY saveDateChanged)
    Q_PROPERTY(bool saveTime READ saveTime WRITE setSaveTime NOTIFY saveTimeChanged)
    Q_PROPERTY(bool saveMs READ saveMs WRITE setSaveMs NOTIFY saveMsChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
public:
    explicit xToolsStorerTool(QObject *parent = nullptr);
    ~xToolsStorerTool();

    virtual void inputBytes(const QByteArray &bytes) override;

public:
    int saveFormat();
    void setSaveFormat(int format);
    bool saveRx();
    void setSaveRx(bool save);
    bool saveTx();
    void setSaveTx(bool save);
    bool saveDate();
    void setSaveDate(bool save);
    bool saveTime();
    void setSaveTime(bool save);
    bool saveMs();
    void setSaveMs(bool save);
    QString fileName();
    void setFileName(const QString &name);

signals:
    void saveFormatChanged();
    void saveRxChanged();
    void saveTxChanged();
    void saveDateChanged();
    void saveTimeChanged();
    void saveMsChanged();
    void fileNameChanged();

protected:
    virtual void run() final;

private:
    struct Parameters
    {
        QString file;
        int format;
        bool saveRx;
        bool saveTx;
        bool saveDate;
        bool saveTime;
        bool saveMs;
    } mParameters;
    QMutex mParametersMutex;

    QList<QByteArray> mInputContextList;
    QMutex mInputContextListMutex;

private:
    void write2file();
};
