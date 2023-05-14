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
#include "tools/SAKBaseTool.hh"
#include "tools/SAKToolFactory.hh"

class SAKToolBox : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant communication READ communication
               NOTIFY communicationChanged)
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

    /// You must call the interface before useing the call object.
    Q_INVOKABLE void setupComunicationTool(int type);

    Q_INVOKABLE void open();
    Q_INVOKABLE void close();
    Q_INVOKABLE void send(const QByteArray &bytes, const QVariant &context = QJsonObject());
    bool isWorking(){return mIsWorking;}

private:
    SAKBaseTool         *mComunicationTool{nullptr};
    SAKMaskerTool       *mInputMaskerTool{nullptr};
    SAKMaskerTool       *mOutputMaskerTool{nullptr};
    SAKAnalyzerTool     *mInputAnalyzerTool{nullptr};
    SAKAnalyzerTool     *mOutputAnalyzerTool{nullptr};
    SAKEmitterTool      *mEmitterTool{nullptr};
    SAKResponserTool    *mResponserTool{nullptr};
    SAKStorerTool       *mStorerTool{nullptr};
    SAKPrestorerTool    *mPrestorerTool{nullptr};

    QList<SAKBaseTool*> mToolList;

private:
    void uninitializedTips();

private:
    QVariant communication(){return QVariant::fromValue(mComunicationTool);}
    Q_SIGNAL void communicationChanged();

    QVariant inputMasker(){return QVariant::fromValue(mInputMaskerTool);}
    QVariant outputMasker(){return QVariant::fromValue(mOutputMaskerTool);}
    QVariant inputAnalyzer(){return QVariant::fromValue(mInputAnalyzerTool);}
    QVariant outputAnalyzer(){return QVariant::fromValue(mOutputAnalyzerTool);}
    QVariant emitter(){return QVariant::fromValue(mEmitterTool);}
    QVariant responser(){return QVariant::fromValue(mResponserTool);}
    QVariant storer(){return QVariant::fromValue(mStorerTool);}
    QVariant prestorer(){return QVariant::fromValue(mPrestorerTool);}

    bool mIsWorking{false};
signals:
    void isWorkingChanged();
};

#endif // SAKTOOLBOX_H
