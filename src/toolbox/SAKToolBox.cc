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
#include "tools/SAKToolsFactory.hh"

SAKToolBox::SAKToolBox(QObject *parent)
    : QObject{parent}
{
    auto createTool = [](int type)->SAKBaseTool*{
        auto toolFactory = SAKToolsFactory::instance();
        return toolFactory->createTool(type);
    };

    mInputMaskerTool  = createTool(SAKToolsFactory::MaskerTool);
    mOutputMaskerTool = createTool(SAKToolsFactory::MaskerTool);
    mAnalyzerTool     = createTool(SAKToolsFactory::AnalyzerTool);
    mEmitterTool      = createTool(SAKToolsFactory::EmitterTool);
    mResponserTool    = createTool(SAKToolsFactory::ResponserTool);
    mStorerTool       = createTool(SAKToolsFactory::StorerTool);
    mPrestorerTool    = createTool(SAKToolsFactory::PrestoreTool);
}

void SAKToolBox::setupComunicationTool(int type)
{
    if (mComunicationTool) {
        mComunicationTool->exit();
        mComunicationTool->wait();
        mComunicationTool->deleteLater();
    }

    auto toolFactory = SAKToolsFactory::instance();
    mComunicationTool = toolFactory->createTool(type);
    if (mComunicationTool) {
//        connect(mComunicationTool, &SAKBaseTool::bytesOutputted,
//                mOutputMaskerTool,
//                [=](const QString &flag, const QByteArray &bytes){
//            mOutputMaskerTool->setToolFlag(flag);
//            mOutputMaskerTool->inputBytes(bytes);
//        });
//        connect(mComunicationTool, &SAKBaseTool::bytesOutputted,
//                mEmitterTool,
//                [=](const QString &flag, const QByteArray &bytes){
//            mEmitterTool->setToolFlag(flag);
//            mEmitterTool->inputBytes(bytes);
//        });
//        connect(mAnalyzerTool, &SAKBaseTool::bytesOutputted, mResponserTool,
//                [=](const QString &flag, const QByteArray &bytes){
//            mResponserTool->setToolFlag(flag);
//            mResponserTool->inputBytes(bytes);
//        });
//        connect(mOutputMaskerTool, &SAKBaseTool::bytesOutputted,
//                mAnalyzerTool,
//                [=](const QString &flag, const QByteArray &bytes){
//            mAnalyzerTool->setToolFlag(flag);
//            mAnalyzerTool->inputBytes(bytes);
//        });
//        connect(mInputMaskerTool, &SAKBaseTool::bytesOutputted, mComunicationTool,
//                [=](const QString &flag, const QByteArray &bytes){
//            Q_UNUSED(flag);
//            mComunicationTool->inputBytes(bytes);
//        });
//        connect(mEmitterTool, &SAKBaseTool::bytesOutputted, mComunicationTool,
//                [=](const QString &flag, const QByteArray &bytes){
//            Q_UNUSED(flag);
//            mComunicationTool->inputBytes(bytes);
//        });
//        connect(mResponserTool, &SAKBaseTool::bytesOutputted, mComunicationTool,
//                [=](const QString &flag, const QByteArray &bytes){
//            Q_UNUSED(flag);
//            mComunicationTool->inputBytes(bytes);
//        });
//        connect(mComunicationTool, &SAKBaseTool::bytesInputted, mStorerTool,
//                [=](const QString &flag, const QByteArray &bytes){
//            QJsonObject jsonObj;
//            jsonObj.insert("flag", flag);
//            jsonObj.insert("isRx", false);
//            mStorerTool->inputBytes(bytes, jsonObj);
//        });
//        connect(mComunicationTool, &SAKBaseTool::bytesOutputted, mStorerTool,
//                [=](const QString &flag, const QByteArray &bytes){
//            QJsonObject jsonObj;
//            jsonObj.insert("flag", flag);
//            jsonObj.insert("isRx", true);
//            mStorerTool->inputBytes(bytes, jsonObj);
//        });
//        connect(mPrestorerTool, &SAKBaseTool::bytesOutputted, mComunicationTool,
//                [=](const QString &flag, const QByteArray &bytes){
//                    Q_UNUSED(flag)
//                    mComunicationTool->inputBytes(bytes);
//        });
    }

    emit comunicationToolChanged();
}

SAKBaseTool *SAKToolBox::comunicationTool()
{
    return mComunicationTool;
}

SAKBaseTool *SAKToolBox::inputMaskerTool()
{
    return mInputMaskerTool;
}

SAKBaseTool *SAKToolBox::outputMaskerTool()
{
    return mOutputMaskerTool;
}

SAKBaseTool *SAKToolBox::analyerTool()
{
    return mAnalyzerTool;
}

SAKBaseTool *SAKToolBox::emiterTool()
{
    return mEmitterTool;
}

SAKBaseTool *SAKToolBox::responserTool()
{
    return mResponserTool;
}

SAKBaseTool *SAKToolBox::prestorerTool()
{
    return mPrestorerTool;
}

void SAKToolBox::open()
{
    if (mComunicationTool) {
        mInputMaskerTool->start();
        mOutputMaskerTool->start();
        mAnalyzerTool->start();
        mEmitterTool->start();
        mResponserTool->start();
        mStorerTool->start();
        mPrestorerTool->start();
        mComunicationTool->start();

        mIsWorking = true;
        emit isWorkingChanged();
    } else {
        uninitializedTips();
    }
}

void SAKToolBox::close()
{
    if (mComunicationTool) {
        mInputMaskerTool->exit();
        mAnalyzerTool->exit();
        mEmitterTool->exit();
        mResponserTool->exit();
        mStorerTool->exit();
        mPrestorerTool->exit();
        mComunicationTool->exit();

        mInputMaskerTool->wait();
        mOutputMaskerTool->start();
        mAnalyzerTool->wait();
        mEmitterTool->wait();
        mResponserTool->wait();
        mStorerTool->wait();
        mPrestorerTool->exit();
        mComunicationTool->wait();

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
