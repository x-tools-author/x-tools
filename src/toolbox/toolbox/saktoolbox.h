/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKTOOLBOX_HH
#define SAKTOOLBOX_HH

#include <QObject>
#include <QVariant>

#include "SAKBaseTool.h"
#include "SAKMaskerTool.h"
#include "SAKStorerTool.h"
#include "SAKEmitterTool.h"
#include "SAKAnalyzerTool.h"
#include "SAKPrestorerTool.h"
#include "SAKResponserTool.h"
#include "SAKVelometerTool.h"
#include "SAKStatisticianTool.h"
#include "SAKCommunicationTool.h"
#include "SAKUdpTransmitterTool.h"
#include "SAKTcpTransmitterTool.h"
#include "SAKWebSocketTransmitterTool.h"
#include "SAKSerialPortTransmitterTool.h"

class SAKToolBox : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant communicaton READ communicaton
                   NOTIFY communicatonChanged)
    Q_PROPERTY(QVariant txMasker READ txMasker CONSTANT)
    Q_PROPERTY(QVariant rxMasker READ rxMasker CONSTANT)
    Q_PROPERTY(QVariant txAnalyzer READ txAnalyzer CONSTANT)
    Q_PROPERTY(QVariant rxAnalyzer READ rxAnalyzer CONSTANT)
    Q_PROPERTY(QVariant emitter READ emitter CONSTANT)
    Q_PROPERTY(QVariant responser READ responser CONSTANT)
    Q_PROPERTY(QVariant storer READ storer CONSTANT)
    Q_PROPERTY(QVariant prestorer READ prestorer CONSTANT)
    Q_PROPERTY(QVariant rxVelometer READ rxVelometer CONSTANT)
    Q_PROPERTY(QVariant txVelometer READ txVelometer CONSTANT)
    Q_PROPERTY(QVariant rxStatistician READ rxStatistician CONSTANT)
    Q_PROPERTY(QVariant txStatistician READ txStatistician CONSTANT)

    Q_PROPERTY(QVariant udpTransmitter READ udpTransmitter CONSTANT)
    Q_PROPERTY(QVariant tcpTransmitter READ tcpTransmitter CONSTANT)
    Q_PROPERTY(QVariant webSocketTransmitter READ webSocketTransmitter
                   CONSTANT)
    Q_PROPERTY(QVariant serialPortTransmitter READ serialPortTransmitter
                   CONSTANT)

    Q_PROPERTY(bool isWorking READ isWorking NOTIFY isWorkingChanged)
public:
    explicit SAKToolBox(QObject *parent = nullptr);
    ~SAKToolBox();

    /// You must call the interface before useing.
    Q_INVOKABLE void initialize(int type);

    Q_INVOKABLE void open();
    Q_INVOKABLE void close();
    Q_INVOKABLE void send(const QByteArray &bytes,
                          const QVariant &context = QJsonObject());
    bool isWorking();

    SAKCommunicationTool* getCommunicationTool();
    SAKMaskerTool* getTxMaskerTool();
    SAKMaskerTool* getRxMaskerTool();
    SAKAnalyzerTool* getTxAnalyzerTool();
    SAKAnalyzerTool* getRxAnalyzerTool();
    SAKEmitterTool* getEmitterTool();
    SAKResponserTool* getResponserTool();
    SAKStorerTool* getStorerTool();
    SAKPrestorerTool* getPrestorerTool();
    SAKVelometerTool* getRxVelometerTool();
    SAKVelometerTool* getTxVelometerTool();
    SAKStatisticianTool* getRxStatisticianTool();
    SAKStatisticianTool* getTxStatisticianTool();
    SAKUdpTransmitterTool * getUdpTransmitterTool();
    SAKTcpTransmitterTool * getTcpTransmitterTool();
    SAKWebSocketTransmitterTool* getWebSocketTransmitterTool();
    SAKSerialPortTransmitterTool* getSerialPortTransmitterTool();

signals:
    void errorOccurred(const QString &errorString);

private:   
    QList<SAKBaseTool*> mToolList;
    const QLoggingCategory mLoggingCategory{"sak.toolbox"};

private:
    void uninitializedTips();

// Properties ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
private:
    SAKCommunicationTool* mComunicationTool{nullptr};
    SAKMaskerTool* mTxMaskerTool{nullptr};
    SAKMaskerTool* mRxMaskerTool{nullptr};
    SAKAnalyzerTool* mTxAnalyzerTool{nullptr};
    SAKAnalyzerTool* mRxAnalyzerTool{nullptr};
    SAKEmitterTool* mEmitterTool{nullptr};
    SAKResponserTool* mResponserTool{nullptr};
    SAKStorerTool* mStorerTool{nullptr};
    SAKPrestorerTool* mPrestorerTool{nullptr};
    SAKVelometerTool* mRxVelometerTool{nullptr};
    SAKVelometerTool* mTxVelometerTool{nullptr};
    SAKStatisticianTool* mRxStatisticianTool{nullptr};
    SAKStatisticianTool* mTxStatisticianTool{nullptr};
    SAKUdpTransmitterTool* mUdpTransmitterTool{nullptr};
    SAKTcpTransmitterTool* mTcpTransmitterTool{nullptr};
    SAKWebSocketTransmitterTool* mWebSocketTransmitterTool{nullptr};
    SAKSerialPortTransmitterTool* mSerialPortTransmitterTool{nullptr};
    bool mIsWorking{false};

private:
    QVariant communicaton();
    QVariant txMasker();
    QVariant rxMasker();
    QVariant txAnalyzer();
    QVariant rxAnalyzer();
    QVariant emitter();
    QVariant responser();
    QVariant storer();
    QVariant prestorer();
    QVariant rxVelometer();
    QVariant txVelometer();
    QVariant rxStatistician();
    QVariant txStatistician();
    QVariant udpTransmitter();
    QVariant tcpTransmitter();
    QVariant webSocketTransmitter();
    QVariant serialPortTransmitter();

signals:
    void isWorkingChanged();
    void communicatonChanged();
};

#endif // SAKTOOLBOX_H
