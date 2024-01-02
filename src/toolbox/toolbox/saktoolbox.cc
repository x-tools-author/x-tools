/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saktoolbox.h"

#include "saktoolfactory.h"

SAKToolBox::SAKToolBox(QObject* parent)
    : QObject{parent}
{
    auto createTool = [](int type) -> SAKBaseTool* {
        auto toolFactory = SAKToolFactory::instance();
        return toolFactory->createTool(type);
    };

    // clang-format off
    mTxMaskerTool = qobject_cast<SAKMaskerTool*>(createTool(SAKToolFactory::MaskerTool));
    mRxMaskerTool = qobject_cast<SAKMaskerTool*>(createTool(SAKToolFactory::MaskerTool));
    mTxAnalyzerTool = qobject_cast<SAKAnalyzerTool*>(createTool(SAKToolFactory::AnalyzerTool));
    mRxAnalyzerTool = qobject_cast<SAKAnalyzerTool*>(createTool(SAKToolFactory::AnalyzerTool));
    mEmitterTool = qobject_cast<SAKEmitterTool*>(createTool(SAKToolFactory::EmitterTool));
    mResponserTool = qobject_cast<SAKResponserTool*>(createTool(SAKToolFactory::ResponserTool));
    mStorerTool = qobject_cast<SAKStorerTool*>(createTool(SAKToolFactory::StorerTool));
    mPrestorerTool = qobject_cast<SAKPrestorerTool*>(createTool(SAKToolFactory::PrestoreTool));
    mRxVelometerTool = qobject_cast<SAKVelometerTool*>(createTool(SAKToolFactory::VelometerTool));
    mTxVelometerTool = qobject_cast<SAKVelometerTool*>(createTool(SAKToolFactory::VelometerTool));
    mRxStatisticianTool = qobject_cast<SAKStatisticianTool*>(createTool(SAKToolFactory::StatistiticianTool));
    mTxStatisticianTool = qobject_cast<SAKStatisticianTool*>(createTool(SAKToolFactory::StatistiticianTool));

    mUdpTransmitterTool = qobject_cast<SAKUdpTransmitterTool*>(createTool(SAKToolFactory::UdpTransmitterTool));
    mTcpTransmitterTool = qobject_cast<SAKTcpTransmitterTool*>(createTool(SAKToolFactory::TcpTransmitterTool));
    mWebSocketTransmitterTool = qobject_cast<SAKWebSocketTransmitterTool*>( createTool(SAKToolFactory::WebSocketTransmitterTool));
    mSerialPortTransmitterTool = qobject_cast<SAKSerialPortTransmitterTool*>( createTool(SAKToolFactory::SerialPortTransmitterTool));
    // clang-format on

    mToolList << mTxMaskerTool << mRxMaskerTool << mTxAnalyzerTool << mRxAnalyzerTool
              << mEmitterTool << mResponserTool << mStorerTool << mPrestorerTool << mRxVelometerTool
              << mTxVelometerTool << mRxStatisticianTool << mTxStatisticianTool
              << mUdpTransmitterTool << mTcpTransmitterTool << mWebSocketTransmitterTool
              << mSerialPortTransmitterTool;

    int flag = Qt::AutoConnection | Qt::UniqueConnection;
    for (auto tool : mToolList) {
        connect(tool,
                &SAKBaseTool::errorOccured,
                this,
                &SAKToolBox::errorOccurred,
                Qt::ConnectionType(flag));
    }

    connect(this, &SAKToolBox::errorOccurred, this, [=]() { this->close(); });
}

SAKToolBox::~SAKToolBox()
{
    if (mComunicationTool) {
        mComunicationTool->exit();
        mComunicationTool->wait();
        mComunicationTool->deleteLater();
    }

    for (auto tool : mToolList) {
        tool->exit();
        tool->wait();
        tool->deleteLater();
    }
}

void SAKToolBox::initialize(int type)
{
    if (mComunicationTool) {
        mComunicationTool->exit();
        mComunicationTool->wait();
        mComunicationTool->deleteLater();
        mComunicationTool = nullptr;
    }

    auto toolFactory = SAKToolFactory::instance();
    auto tool = toolFactory->createTool(type);
    mComunicationTool = qobject_cast<SAKCommunicationTool*>(tool);
    if (!mComunicationTool) {
        qWarning() << "mComunicationTool is nullptr, type:" << type;
        return;
    }

    // clang-format off
    // rx->output_masker->output_analyzer->responser
    connect(mComunicationTool, &SAKBaseTool::bytesOutput, mRxMaskerTool, &SAKBaseTool::inputBytes);
    connect(mRxMaskerTool, &SAKBaseTool::bytesOutput, mRxAnalyzerTool, &SAKBaseTool::inputBytes);
    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutput, mResponserTool, &SAKBaseTool::inputBytes);
    // emiiter,responser,prestorer->input_analyzer->input_masker->tx
    connect(mEmitterTool, &SAKBaseTool::bytesOutput, mTxAnalyzerTool, &SAKBaseTool::inputBytes);
    connect(mResponserTool, &SAKBaseTool::bytesOutput, mTxAnalyzerTool, &SAKBaseTool::inputBytes);
    connect(mPrestorerTool, &SAKBaseTool::bytesOutput, mTxAnalyzerTool, &SAKBaseTool::inputBytes);
    connect(mTxAnalyzerTool, &SAKBaseTool::bytesOutput, mTxMaskerTool, &SAKBaseTool::inputBytes);
    connect(mTxMaskerTool, &SAKBaseTool::bytesOutput, mComunicationTool, &SAKBaseTool::inputBytes);
    // rx->storer; tx->storer
    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutput, mStorerTool, &SAKBaseTool::inputBytes);
    connect(mTxMaskerTool, &SAKBaseTool::bytesOutput, mStorerTool, &SAKBaseTool::inputBytes);
    // rx->velometer; tx->velometer
    connect(mComunicationTool, &SAKBaseTool::bytesOutput, mRxVelometerTool, &SAKBaseTool::inputBytes);
    connect(mComunicationTool, &SAKBaseTool::bytesOutput, mTxVelometerTool, &SAKBaseTool::inputBytes);
    // rx->statistician; tx->statistician
    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutput, mRxStatisticianTool, &SAKBaseTool::inputBytes);
    connect(mComunicationTool, &SAKBaseTool::bytesOutput, mTxStatisticianTool, &SAKBaseTool::inputBytes);
    // rx->serialport transmition; serialport transmition->Tx analyzer
    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutput, mSerialPortTransmitterTool, &SAKBaseTool::inputBytes);
    connect(mSerialPortTransmitterTool, &SAKBaseTool::bytesOutput, mTxAnalyzerTool, &SAKBaseTool::inputBytes);
    // rx->udp transmition; udp transmition->Tx analyzer
    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutput, mUdpTransmitterTool, &SAKBaseTool::inputBytes);
    connect(mUdpTransmitterTool, &SAKBaseTool::bytesOutput, mTxAnalyzerTool, &SAKBaseTool::inputBytes);
    // rx->tcp transmition; tcp transmition->Tx analyzer
    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutput, mTcpTransmitterTool, &SAKBaseTool::inputBytes);
    connect(mTcpTransmitterTool, &SAKBaseTool::bytesOutput, mTxAnalyzerTool, &SAKBaseTool::inputBytes);
    // rx->websocket transmition; websocket transmition->Tx analyzer
    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutput, mWebSocketTransmitterTool, &SAKBaseTool::inputBytes);
    connect(mWebSocketTransmitterTool, &SAKBaseTool::bytesOutput, mTxAnalyzerTool, &SAKBaseTool::inputBytes);
    
    connect(mComunicationTool, &SAKCommunicationTool::errorOccured, this, &SAKToolBox::errorOccurred);
    // clang-format on

    emit communicatonChanged();
}

void SAKToolBox::open()
{
    if (mComunicationTool) {
        for (auto tool : mToolList) {
            tool->start();
        }

        mIsWorking = true;
        emit isWorkingChanged();
        mComunicationTool->start();
    } else {
        uninitializedTips();
    }
}

void SAKToolBox::close()
{
    if (mComunicationTool) {
        mComunicationTool->exit();
        mComunicationTool->wait();

        for (auto tool : mToolList) {
            tool->exit();
            tool->wait();
        }

        mIsWorking = false;
        emit isWorkingChanged();
    } else {
        uninitializedTips();
    }
}

void SAKToolBox::send(const QByteArray& bytes)
{
    mTxAnalyzerTool->inputBytes(bytes);
}

bool SAKToolBox::isWorking()
{
    return mIsWorking;
}

SAKCommunicationTool* SAKToolBox::getCommunicationTool()
{
    return mComunicationTool;
}

SAKMaskerTool* SAKToolBox::getTxMaskerTool()
{
    return mTxMaskerTool;
}

SAKMaskerTool* SAKToolBox::getRxMaskerTool()
{
    return mRxMaskerTool;
}

SAKAnalyzerTool* SAKToolBox::getTxAnalyzerTool()
{
    return mTxAnalyzerTool;
}

SAKAnalyzerTool* SAKToolBox::getRxAnalyzerTool()
{
    return mRxAnalyzerTool;
}

SAKEmitterTool* SAKToolBox::getEmitterTool()
{
    return mEmitterTool;
}

SAKResponserTool* SAKToolBox::getResponserTool()
{
    return mResponserTool;
}

SAKStorerTool* SAKToolBox::getStorerTool()
{
    return mStorerTool;
}

SAKPrestorerTool* SAKToolBox::getPrestorerTool()
{
    return mPrestorerTool;
}

SAKVelometerTool* SAKToolBox::getRxVelometerTool()
{
    return mRxVelometerTool;
}

SAKVelometerTool* SAKToolBox::getTxVelometerTool()
{
    return mTxVelometerTool;
}

SAKStatisticianTool* SAKToolBox::getRxStatisticianTool()
{
    return mRxStatisticianTool;
}

SAKStatisticianTool* SAKToolBox::getTxStatisticianTool()
{
    return mTxStatisticianTool;
}

SAKUdpTransmitterTool* SAKToolBox::getUdpTransmitterTool()
{
    return mUdpTransmitterTool;
}

SAKTcpTransmitterTool* SAKToolBox::getTcpTransmitterTool()
{
    return mTcpTransmitterTool;
}

SAKWebSocketTransmitterTool* SAKToolBox::getWebSocketTransmitterTool()
{
    return mWebSocketTransmitterTool;
}

SAKSerialPortTransmitterTool* SAKToolBox::getSerialPortTransmitterTool()
{
    return mSerialPortTransmitterTool;
}

void SAKToolBox::uninitializedTips()
{
    static QByteArray tips("You must call the interface name initialize()"
                           " before using the object.");
    Q_ASSERT_X(false, __FUNCTION__, tips.constData());

    emit errorOccurred(QString::fromLatin1(tips));
}

QVariant SAKToolBox::communicaton()
{
    return QVariant::fromValue(mComunicationTool);
}

QVariant SAKToolBox::txMasker()
{
    return QVariant::fromValue(mTxMaskerTool);
}

QVariant SAKToolBox::rxMasker()
{
    return QVariant::fromValue(mRxMaskerTool);
}

QVariant SAKToolBox::txAnalyzer()
{
    return QVariant::fromValue(mTxAnalyzerTool);
}

QVariant SAKToolBox::rxAnalyzer()
{
    return QVariant::fromValue(mRxAnalyzerTool);
}

QVariant SAKToolBox::emitter()
{
    return QVariant::fromValue(mEmitterTool);
}

QVariant SAKToolBox::responser()
{
    return QVariant::fromValue(mResponserTool);
}

QVariant SAKToolBox::storer()
{
    return QVariant::fromValue(mStorerTool);
}

QVariant SAKToolBox::prestorer()
{
    return QVariant::fromValue(mPrestorerTool);
}

QVariant SAKToolBox::rxVelometer()
{
    return QVariant::fromValue(mRxVelometerTool);
}

QVariant SAKToolBox::txVelometer()
{
    return QVariant::fromValue(mTxVelometerTool);
}

QVariant SAKToolBox::rxStatistician()
{
    return QVariant::fromValue(mRxStatisticianTool);
}

QVariant SAKToolBox::txStatistician()
{
    return QVariant::fromValue(mTxStatisticianTool);
}

QVariant SAKToolBox::udpTransmitter()
{
    return QVariant::fromValue(mUdpTransmitterTool);
}

QVariant SAKToolBox::tcpTransmitter()
{
    return QVariant::fromValue(mTcpTransmitterTool);
}

QVariant SAKToolBox::webSocketTransmitter()
{
    return QVariant::fromValue(mWebSocketTransmitterTool);
}

QVariant SAKToolBox::serialPortTransmitter()
{
    return QVariant::fromValue(mSerialPortTransmitterTool);
}
