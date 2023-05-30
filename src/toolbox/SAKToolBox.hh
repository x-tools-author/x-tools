/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTOOLBOX_HH
#define SAKTOOLBOX_HH

#include <QObject>
#include <QVariant>

#include "SAKBaseTool.hh"
#include "SAKMaskerTool.hh"
#include "SAKStorerTool.hh"
#include "SAKEmitterTool.hh"
#include "SAKAnalyzerTool.hh"
#include "SAKPrestorerTool.hh"
#include "SAKResponserTool.hh"
#include "SAKCommunicationTool.hh"

class SAKToolBox : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant communicaton READ communicaton
                   NOTIFY communicatonChanged)
    Q_PROPERTY(QVariant inputMasker READ inputMasker CONSTANT)
    Q_PROPERTY(QVariant outputMasker READ outputMasker CONSTANT)
    Q_PROPERTY(QVariant inputAnalyzer READ inputAnalyzer CONSTANT)
    Q_PROPERTY(QVariant outputAnalyzer READ outputAnalyzer CONSTANT)
    Q_PROPERTY(QVariant emitter READ emitter CONSTANT)
    Q_PROPERTY(QVariant responser READ responser CONSTANT)
    Q_PROPERTY(QVariant storer READ storer CONSTANT)
    Q_PROPERTY(QVariant prestorer READ prestorer CONSTANT)

    Q_PROPERTY(bool isWorking READ isWorking NOTIFY isWorkingChanged)
public:
    explicit SAKToolBox(QObject *parent = nullptr);

    /// You must call the interface before useing.
    Q_INVOKABLE void initialize(int type);

    Q_INVOKABLE void open();
    Q_INVOKABLE void close();
    Q_INVOKABLE void send(const QByteArray &bytes,
                          const QVariant &context = QJsonObject());
    bool isWorking(){return mIsWorking;}

    SAKCommunicationTool*getCommunicationTool(){return mComunicationTool;}
    SAKMaskerTool       *getInputMaskerTool(){return mInputMaskerTool;}
    SAKMaskerTool       *getOutputMaskerTool(){return mOutputMaskerTool;}
    SAKAnalyzerTool     *getInputAnalyzerTool(){return mInputAnalyzerTool;}
    SAKAnalyzerTool     *getOutputAnalyzerTool(){return mOutputAnalyzerTool;}
    SAKEmitterTool      *getEmitterTool(){return mEmitterTool;}
    SAKResponserTool    *getResponserTool(){return mResponserTool;}
    SAKStorerTool       *getStorerTool(){return mStorerTool;}
    SAKPrestorerTool    *getPrestorerTool(){return mPrestorerTool;}

private:
    SAKCommunicationTool*mComunicationTool{nullptr};
    SAKMaskerTool       *mInputMaskerTool{nullptr};
    SAKMaskerTool       *mOutputMaskerTool{nullptr};
    SAKAnalyzerTool     *mInputAnalyzerTool{nullptr};
    SAKAnalyzerTool     *mOutputAnalyzerTool{nullptr};
    SAKEmitterTool      *mEmitterTool{nullptr};
    SAKResponserTool    *mResponserTool{nullptr};
    SAKStorerTool       *mStorerTool{nullptr};
    SAKPrestorerTool    *mPrestorerTool{nullptr};

    QList<SAKBaseTool*> mToolList;
    const QLoggingCategory mLoggingCategory{"SAK.CustomBox"};
    bool mIsWorking{false};

private:
    void uninitializedTips();

    // Properties ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
private:
    QVariant communicaton(){return QVariant::fromValue(mComunicationTool);}
    QVariant inputMasker(){return QVariant::fromValue(mInputMaskerTool);}
    QVariant outputMasker(){return QVariant::fromValue(mOutputMaskerTool);}
    QVariant inputAnalyzer(){return QVariant::fromValue(mInputAnalyzerTool);}
    QVariant outputAnalyzer(){return QVariant::fromValue(mOutputAnalyzerTool);}
    QVariant emitter(){return QVariant::fromValue(mEmitterTool);}
    QVariant responser(){return QVariant::fromValue(mResponserTool);}
    QVariant storer(){return QVariant::fromValue(mStorerTool);}
    QVariant prestorer(){return QVariant::fromValue(mPrestorerTool);}

signals:
    void isWorkingChanged();
    void communicatonChanged();
};

#endif // SAKTOOLBOX_H
