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

SAKToolBox::SAKToolBox(QObject *parent)
    : QObject{parent}
{
    auto createTool = [](int type)->SAKBaseTool*{
        auto toolFactory = SAKToolFactory::instance();
        return toolFactory->createTool(type);
    };

    mInputMaskerTool    = qobject_cast<SAKMaskerTool*>(createTool(SAKToolFactory::MaskerTool));
    mOutputMaskerTool   = qobject_cast<SAKMaskerTool*>(createTool(SAKToolFactory::MaskerTool));
    mInputAnalyzerTool  = qobject_cast<SAKAnalyzerTool*>(createTool(SAKToolFactory::AnalyzerTool));
    mOutputAnalyzerTool = qobject_cast<SAKAnalyzerTool*>(createTool(SAKToolFactory::AnalyzerTool));
    mEmitterTool        = qobject_cast<SAKEmitterTool*>(createTool(SAKToolFactory::EmitterTool));
    mResponserTool      = qobject_cast<SAKResponserTool*>(createTool(SAKToolFactory::ResponserTool));
    mStorerTool         = qobject_cast<SAKStorerTool*>(createTool(SAKToolFactory::StorerTool));
    mPrestorerTool      = qobject_cast<SAKPrestorerTool*>(createTool(SAKToolFactory::PrestoreTool));

    mToolList << mInputMaskerTool
              << mOutputMaskerTool
              << mInputAnalyzerTool
              << mOutputAnalyzerTool
              << mEmitterTool
              << mResponserTool
              << mStorerTool
              << mPrestorerTool;
}

void SAKToolBox::setupComunicationTool(int type)
{
    if (mComunicationTool) {
        mComunicationTool->exit();
        mComunicationTool->wait();
        mComunicationTool->deleteLater();
    }

    auto toolFactory = SAKToolFactory::instance();
    mComunicationTool = toolFactory->createTool(type);
    if (mComunicationTool) {
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
    }

    emit communicationChanged();
}

void SAKToolBox::open()
{
    if (mComunicationTool) {
        mComunicationTool->start();

        for (auto tool : mToolList) {
            tool->start();
        }

        mIsWorking = true;
        emit isWorkingChanged();
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

void SAKToolBox::send(const QByteArray &bytes)
{
    mInputMaskerTool->inputBytes(bytes, QVariant());
}

void SAKToolBox::uninitializedTips()
{
    Q_ASSERT_X(false, __FUNCTION__,
               "You must call the interface name setupComunicationTool() "
               "before using the object.");
}
