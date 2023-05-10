/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKCOMMUNICATIONTOOL_HH
#define SAKCOMMUNICATIONTOOL_HH

#include <atomic>
#include <QMutex>
#include "SAKBaseTool.hh"

class SAKCommunicationTool : public SAKBaseTool
{
    Q_OBJECT
public:
    explicit SAKCommunicationTool(const char *logCategory,
                                  QObject *parent = nullptr);

protected:
    struct InputDataContext {
        QByteArray bytes;
        QVariant context;
    };

protected:
    virtual void run() final;
    virtual void inputBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;

    virtual bool initialize() = 0;
    virtual void readBytes() = 0;
    virtual void writeBytes(const QByteArray &bytes,
                            const QVariant &context) = 0;
    virtual void uninitialize() = 0;

protected:
    // If the value is false,
    // you should use your own way to trigger to call readBytes().
    std::atomic_bool mIsTimerReading{true};

private:
    QList<InputDataContext> mInputDataList;
    QMutex mInputDataMutex;
};

#endif // SAKCOMMUNICATIONTOOL_HH
