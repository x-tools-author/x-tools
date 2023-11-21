/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKSTORERTOOL_H
#define SAKSTORERTOOL_H

#include <QMutex>
#include <QTimer>

#include "sakbasetool.h"

class SAKStorerTool : public SAKBaseTool
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
    explicit SAKStorerTool(QObject *parent = nullptr);
    ~SAKStorerTool();

    virtual void inputBytes(const QByteArray &bytes, const QVariant &context = QJsonObject()) final;

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

    struct InputContext
    {
        QByteArray bytes;
        QVariant context;
    };
    QList<InputContext> mInputContextList;
    QMutex mInputContextListMutex;

private:
    void write2file();
};

#endif // SAKSTORERTOOL_H
