/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef EDSTORERTOOL_H
#define EDSTORERTOOL_H

#include <QTimer>
#include <QMutex>

#include "SAKBaseTool.hh"

class EDStorerTool : public SAKBaseTool
{
    Q_OBJECT
public:
    explicit EDStorerTool(QObject *parent = nullptr);
    ~EDStorerTool();

    virtual void inputBytes(
            const QByteArray &bytes,
            const QVariant &context = QJsonObject()) final;

    Q_INVOKABLE void setPath(const QString &path);
    Q_INVOKABLE void setFormat(int format);
    Q_INVOKABLE void setSaveDate(bool saveDate);
    Q_INVOKABLE void setSaveTime(bool saveTime);
    Q_INVOKABLE void setSaveMs(bool saveMs);

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void outputBytesHandler() final;
    virtual void uninitialize() final;

private:
    struct Parameters {
        QString file;
        int format;
        bool saveDate;
        bool saveTime;
        bool saveMs;
    } mParameters;
    QMutex mParametersMutex;

    QTimer *mWriteTimer{nullptr};

    struct InputContext {
        QByteArray bytes;
        QJsonObject context;
    };
    QList<InputContext> mInputContextList;
    QMutex mInputContextListMutex;
};

#endif // EDSTORERTOOL_H
