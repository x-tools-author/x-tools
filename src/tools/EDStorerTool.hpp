/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDSTORERTOOL_HPP
#define EDSTORERTOOL_HPP

#include <QTimer>
#include <QMutex>

#include "EDBaseTool.hpp"

class EDStorerTool : public EDBaseTool
{
    Q_OBJECT
public:
    explicit EDStorerTool(QObject *parent = nullptr);
    ~EDStorerTool();

    virtual void inputBytes(
            const QByteArray &bytes,
            const QJsonObject &context = QJsonObject()) override;

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

#endif // EDSTORERTOOL_HPP
