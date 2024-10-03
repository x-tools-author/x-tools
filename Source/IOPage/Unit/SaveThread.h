/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMutex>
#include <QPair>
#include <QThread>

class SaveThread : public QThread
{
    Q_OBJECT
public:
    struct SaveParameters
    {
        bool saveToFile;
        QString fileName;
        bool saveRx;
        bool saveTx;
        bool saveDate;
        bool saveTime;
        bool saveMs;
        int format;
        int maxKBytes;
    };

    struct SaveContext
    {
        SaveParameters parameters;
        QByteArray data;
        bool isRx;
    };

public:
    explicit SaveThread(QObject *parent = nullptr);
    ~SaveThread();

    void saveData(const SaveParameters &parameters, const QByteArray &data, bool isRx);

private:
    QList<SaveContext> m_ctxList;
    QMutex m_ctxListMutex;
    SaveParameters m_parameters;

protected:
    void run() override;

private:
    void openSaveFile();
};
