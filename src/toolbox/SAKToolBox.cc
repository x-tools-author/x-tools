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
    mInputMaskerTool = qobject_cast<SAKMaskerTool*>(tool);
    tool = createTool(SAKToolFactory::MaskerTool);
    mOutputMaskerTool = qobject_cast<SAKMaskerTool*>(tool);
    tool = createTool(SAKToolFactory::AnalyzerTool);
    mInputAnalyzerTool = qobject_cast<SAKAnalyzerTool*>(tool);
    tool = createTool(SAKToolFactory::AnalyzerTool);
    mOutputAnalyzerTool = qobject_cast<SAKAnalyzerTool*>(tool);
    tool = createTool(SAKToolFactory::EmitterTool);
    mEmitterTool = qobject_cast<SAKEmitterTool*>(tool);
    tool = createTool(SAKToolFactory::ResponserTool);
    mResponserTool = qobject_cast<SAKResponserTool*>(tool);
    tool = createTool(SAKToolFactory::StorerTool);
    mStorerTool = qobject_cast<SAKStorerTool*>(tool);
    tool = createTool(SAKToolFactory::PrestoreTool);
    mPrestorerTool = qobject_cast<SAKPrestorerTool*>(tool);
    tool = createTool(SAKToolFactory::VelometerTool);
    mVelometerTool = qobject_cast<SAKVelometerTool*>(tool);

    mToolList << mInputMaskerTool
              << mOutputMaskerTool
              << mInputAnalyzerTool
              << mOutputAnalyzerTool
              << mEmitterTool
              << mResponserTool
              << mStorerTool
              << mPrestorerTool
              << mVelometerTool;
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
            mOutputMaskerTool, &SAKBaseTool::inputBytes);
    connect(mOutputMaskerTool, &SAKBaseTool::bytesOutputted,
            mOutputAnalyzerTool, &SAKBaseTool::inputBytes);
    connect(mOutputAnalyzerTool, &SAKBaseTool::bytesOutputted,
            mEmitterTool, &SAKBaseTool::inputBytes);
    connect(mOutputAnalyzerTool, &SAKBaseTool::bytesOutputted,
            mResponserTool, &SAKBaseTool::inputBytes);

    // emiiter,responser,prestorer->input_analyzer->input_masker->tx
    connect(mEmitterTool, &SAKBaseTool::bytesOutputted,
            mInputAnalyzerTool, &SAKBaseTool::inputBytes);
    connect(mResponserTool, &SAKBaseTool::bytesOutputted,
            mInputAnalyzerTool, &SAKBaseTool::inputBytes);
    connect(mPrestorerTool, &SAKBaseTool::bytesOutputted,
            mInputAnalyzerTool, &SAKBaseTool::inputBytes);
    connect(mInputAnalyzerTool, &SAKBaseTool::bytesOutputted,
            mInputMaskerTool, &SAKBaseTool::inputBytes);
    connect(mInputMaskerTool, &SAKBaseTool::bytesOutputted,
            mComunicationTool, &SAKBaseTool::inputBytes);

    // rx->storer; tx->storer
    connect(mComunicationTool, &SAKBaseTool::bytesOutputted,
            mStorerTool, &SAKBaseTool::inputBytes);
    connect(mComunicationTool, &SAKBaseTool::bytesInputted,
            mStorerTool, &SAKBaseTool::inputBytes);

    // rx->velometer; tx->velometer
    connect(mComunicationTool, &SAKBaseTool::bytesOutputted,
            mVelometerTool, &SAKBaseTool::inputBytes);
    connect(mComunicationTool, &SAKBaseTool::bytesInputted,
            mVelometerTool, &SAKBaseTool::inputBytes);



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
    mInputAnalyzerTool->inputBytes(bytes, context);
}

void SAKToolBox::uninitializedTips()
{
    Q_ASSERT_X(false, __FUNCTION__,
               "You must call the interface name initialize() "
               "before using the object.");
}
