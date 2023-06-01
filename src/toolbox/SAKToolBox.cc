/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
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
              << mTxStatisticianTool;
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
    connect(mComunicationTool, &SAKBaseTool::bytesOutputted,
            mStorerTool, &SAKBaseTool::inputBytes);
    connect(mComunicationTool, &SAKBaseTool::bytesInputted,
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

void SAKToolBox::uninitializedTips()
{
    Q_ASSERT_X(false, __FUNCTION__,
               "You must call the interface name initialize() "
               "before using the object.");
}
