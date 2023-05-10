/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef EDDEVICE_HPP
#define EDDEVICE_HPP

#include <QObject>
#include <QVariant>
#include "tools/SAKBaseTool.hh"

class EDDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant communication READ communication
               NOTIFY comunicationToolChanged)
    Q_PROPERTY(QVariant inputMasker READ inputMasker CONSTANT)
    Q_PROPERTY(QVariant outputMasker READ outputMasker CONSTANT)
    Q_PROPERTY(QVariant analyzer READ analyzer CONSTANT)
    Q_PROPERTY(QVariant emitter READ emitter CONSTANT)
    Q_PROPERTY(QVariant responser READ responser CONSTANT)
    Q_PROPERTY(QVariant storer READ storer CONSTANT)
    Q_PROPERTY(QVariant prestorer READ prestorer CONSTANT)

    Q_PROPERTY(bool isWorking READ isWorking NOTIFY isWorkingChanged)
public:
    explicit EDDevice(QObject *parent = nullptr);

    /// You must call the interface before useing the call object.
    Q_INVOKABLE void setupComunicationTool(int type);

    Q_INVOKABLE SAKBaseTool *comunicationTool();
    Q_INVOKABLE SAKBaseTool *inputMaskerTool();
    Q_INVOKABLE SAKBaseTool *outputMaskerTool();
    Q_INVOKABLE SAKBaseTool *analyerTool();
    Q_INVOKABLE SAKBaseTool *emiterTool();
    Q_INVOKABLE SAKBaseTool *responserTool();
    Q_INVOKABLE SAKBaseTool *prestorerTool();

    Q_INVOKABLE void open();
    Q_INVOKABLE void close();
    Q_INVOKABLE void send(const QByteArray &bytes);

private:
    SAKBaseTool *mComunicationTool{nullptr};
    SAKBaseTool *mInputMaskerTool{nullptr};
    SAKBaseTool *mOutputMaskerTool{nullptr};
    SAKBaseTool *mAnalyzerTool{nullptr};
    SAKBaseTool *mEmitterTool{nullptr};
    SAKBaseTool *mResponserTool{nullptr};
    SAKBaseTool *mStorerTool{nullptr};
    SAKBaseTool *mPrestorerTool{nullptr};

private:
    void uninitializedTips();

private:
    QVariant communication(){return QVariant::fromValue(mComunicationTool);}
    Q_SIGNAL void comunicationToolChanged();

    QVariant inputMasker(){return QVariant::fromValue(mInputMaskerTool);}
    QVariant outputMasker(){return QVariant::fromValue(mOutputMaskerTool);}
    QVariant analyzer(){return QVariant::fromValue(mAnalyzerTool);}
    QVariant emitter(){return QVariant::fromValue(mEmitterTool);}
    QVariant responser(){return QVariant::fromValue(mResponserTool);}
    QVariant storer(){return QVariant::fromValue(mStorerTool);}
    QVariant prestorer(){return QVariant::fromValue(mPrestorerTool);}

    bool mIsWorking{false};
    bool isWorking(){return mIsWorking;}
    Q_SIGNAL void isWorkingChanged();
};

#endif // EDDEVICE_HPP
