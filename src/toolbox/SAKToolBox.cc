/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKToolBox.hh"
#include "SAKToolFactory.hh"

SAKToolBox::SAKToolBox(QObject *parent)
    : QObject{parent}
{
    auto createTool = [](int type)->SAKBaseTool*{
        auto toolFactory = SAKToolFactory::instance();
        return toolFactory->createTool(type);
    };

    SAKBaseTool *tool = createTool(SAKToolFactory::MaskerTool);
    mTxMaskerTool = qobject_cast<SAKMaskerTool*>(tool);
    tool = createTool(SAKToolFactory::MaskerTool);
    mRxMaskerTool = qobject_cast<SAKMaskerTool*>(tool);
    tool = createTool(SAKToolFactory::AnalyzerTool);
    mTxAnalyzerTool = qobject_cast<SAKAnalyzerTool*>(tool);
    tool = createTool(SAKToolFactory::AnalyzerTool);
    mRxAnalyzerTool = qobject_cast<SAKAnalyzerTool*>(tool);
    tool = createTool(SAKToolFactory::EmitterTool);
    mEmitterTool = qobject_cast<SAKEmitterTool*>(tool);
    tool = createTool(SAKToolFactory::ResponserTool);
    mResponserTool = qobject_cast<SAKResponserTool*>(tool);
    tool = createTool(SAKToolFactory::StorerTool);
    mStorerTool = qobject_cast<SAKStorerTool*>(tool);
    tool = createTool(SAKToolFactory::PrestoreTool);
    mPrestorerTool = qobject_cast<SAKPrestorerTool*>(tool);
    tool = createTool(SAKToolFactory::VelometerTool);
    mRxVelometerTool = qobject_cast<SAKVelometerTool*>(tool);
    tool = createTool(SAKToolFactory::VelometerTool);
    mTxVelometerTool = qobject_cast<SAKVelometerTool*>(tool);

    tool = createTool(SAKToolFactory::StatistiticianTool);
    mRxStatisticianTool = qobject_cast<SAKStatisticianTool*>(tool);
    tool = createTool(SAKToolFactory::StatistiticianTool);
    mTxStatisticianTool = qobject_cast<SAKStatisticianTool*>(tool);
    tool = createTool(SAKToolFactory::UdpTransmitterTool);
    mUdpTransmitterTool =
        qobject_cast<SAKUdpTransmitterTool*>(tool);
    tool = createTool(SAKToolFactory::TcpTransmitterTool);
    mTcpTransmitterTool =
        qobject_cast<SAKTcpTransmitterTool*>(tool);
    tool = createTool(SAKToolFactory::WebSocketTransmitterTool);
    mWebSocketTransmitterTool =
        qobject_cast<SAKWebSocketTransmitterTool*>(tool);
    tool = createTool(SAKToolFactory::SerialPortTransmitterTool);
    mSerialPortTransmitterTool =
        qobject_cast<SAKSerialPortTransmitterTool*>(tool);

    mToolList << mTxMaskerTool
              << mRxMaskerTool
              << mTxAnalyzerTool
              << mRxAnalyzerTool
              << mEmitterTool
              << mResponserTool
              << mStorerTool
              << mPrestorerTool
              << mRxVelometerTool
              << mTxVelometerTool
              << mRxStatisticianTool
              << mTxStatisticianTool
              << mUdpTransmitterTool
              << mTcpTransmitterTool
              << mWebSocketTransmitterTool
              << mSerialPortTransmitterTool;

    int flag = Qt::AutoConnection|Qt::UniqueConnection;
    for (auto tool : mToolList) {
        connect(tool, &SAKBaseTool::errorOccured,
                this, &SAKToolBox::errorOccurred, Qt::ConnectionType(flag));
    }

    connect(this, &SAKToolBox::errorOccurred, this, [=](){
        this->close();
    });
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
        qCWarning(mLoggingCategory) << "mComunicationTool is nullptr, type:"
                                    << type;
        return;
    }

    // rx->output_masker->output_analyzer->emmitter,responser
    connect(mComunicationTool, &SAKBaseTool::bytesOutputted,
            mRxMaskerTool, &SAKBaseTool::inputBytes);
    connect(mRxMaskerTool, &SAKBaseTool::bytesOutputted,
            mRxAnalyzerTool, &SAKBaseTool::inputBytes);
    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutputted,
            mEmitterTool, &SAKBaseTool::inputBytes);
    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutputted,
            mResponserTool, &SAKBaseTool::inputBytes);

    // emiiter,responser,prestorer->input_analyzer->input_masker->tx
    connect(mEmitterTool, &SAKBaseTool::bytesOutputted,
            mTxAnalyzerTool, &SAKBaseTool::inputBytes);
    connect(mResponserTool, &SAKBaseTool::bytesOutputted,
            mTxAnalyzerTool, &SAKBaseTool::inputBytes);
    connect(mPrestorerTool, &SAKBaseTool::bytesOutputted,
            mTxAnalyzerTool, &SAKBaseTool::inputBytes);
    connect(mTxAnalyzerTool, &SAKBaseTool::bytesOutputted,
            mTxMaskerTool, &SAKBaseTool::inputBytes);
    connect(mTxMaskerTool, &SAKBaseTool::bytesOutputted,
            mComunicationTool, &SAKBaseTool::inputBytes);

    // rx->storer; tx->storer
    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutputted,
            mStorerTool, &SAKBaseTool::inputBytes);
    connect(mTxMaskerTool, &SAKBaseTool::bytesInputted,
            mStorerTool, &SAKBaseTool::inputBytes);

    // rx->velometer; tx->velometer
    connect(mComunicationTool, &SAKBaseTool::bytesOutputted,
            mRxVelometerTool, &SAKBaseTool::inputBytes);
    connect(mComunicationTool, &SAKBaseTool::bytesInputted,
            mTxVelometerTool, &SAKBaseTool::inputBytes);

    // rx->statistician; tx->statistician
    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutputted,
            mRxStatisticianTool, &SAKBaseTool::inputBytes);
    connect(mComunicationTool, &SAKBaseTool::bytesInputted,
            mTxStatisticianTool, &SAKBaseTool::inputBytes);

    // rx->serialport transmition; serialport transmition->Tx analyzer
    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutputted,
            mSerialPortTransmitterTool, &SAKBaseTool::inputBytes);
    connect(mSerialPortTransmitterTool, &SAKBaseTool::bytesOutputted,
            mTxAnalyzerTool, &SAKBaseTool::inputBytes);

    // rx->udp transmition; udp transmition->Tx analyzer
    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutputted,
            mUdpTransmitterTool, &SAKBaseTool::inputBytes);
    connect(mUdpTransmitterTool, &SAKBaseTool::bytesOutputted,
            mTxAnalyzerTool, &SAKBaseTool::inputBytes);

    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutputted,
            mTcpTransmitterTool, &SAKBaseTool::inputBytes);
    connect(mTcpTransmitterTool, &SAKBaseTool::bytesOutputted,
            mTxAnalyzerTool, &SAKBaseTool::inputBytes);

    connect(mRxAnalyzerTool, &SAKBaseTool::bytesOutputted,
            mWebSocketTransmitterTool, &SAKBaseTool::inputBytes);
    connect(mWebSocketTransmitterTool, &SAKBaseTool::bytesOutputted,
            mTxAnalyzerTool, &SAKBaseTool::inputBytes);


    connect(mComunicationTool, &SAKCommunicationTool::errorOccured,
            this, &SAKToolBox::errorOccurred);

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

void SAKToolBox::send(const QByteArray &bytes, const QVariant &context)
{
    mTxAnalyzerTool->inputBytes(bytes, context);
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

SAKSerialPortTransmitterTool *SAKToolBox::getSerialPortTransmitterTool()
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
