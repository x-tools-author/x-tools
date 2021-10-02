/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include "SAKUdpTransponder.hh"
#include "SAKUdpClientDevice.hh"
#include "SAKCommonDataStructure.hh"
#include "ui_SAKUdpTransponder.h"

SAKUdpTransponder::SAKUdpTransponder(QWidget *parent)
    :SAKDebuggerPluginTransponder(parent)
    ,mUi(new Ui::SAKUdpTransponder)
    ,mDevice(new SAKUdpClientDevice(Q_NULLPTR, QString(), Q_NULLPTR, Q_NULLPTR))
{
    mUi->setupUi(this);
    setupDevice();
    initSignals();
}

SAKUdpTransponder::SAKUdpTransponder(quint64 id,
                                     SAKUdpClientParametersContext parasCtx,
                                     QWidget *parent)
    :SAKDebuggerPluginTransponder(id, parent)
    ,mUi(new Ui::SAKUdpTransponder)
    ,mDevice(new SAKUdpClientDevice(Q_NULLPTR, QString(), Q_NULLPTR, Q_NULLPTR))
{
    mUi->setupUi(this);
    mUi->peerHostLineEdit->setText(parasCtx.peerHost);
    mUi->peerPortLineEdit->setText(QString::number(parasCtx.peerPort));
    setupDevice();
    initSignals();
}

SAKUdpTransponder::~SAKUdpTransponder()
{
    delete mUi;
    if (mDevice->isRunning()) {
        mDevice->exit();
        mDevice->wait();
    }
}

QVariant SAKUdpTransponder::parametersContext()
{
    SAKUdpClientParametersContext ctx;
    ctx.specifyLocalInfo = false;
    ctx.peerHost = mUi->peerHostLineEdit->text().trimmed();
    ctx.peerPort = mUi->peerPortLineEdit->text().trimmed().toInt();
    ctx.localHost.clear();
    ctx.localPort = 0;

    return QVariant::fromValue(ctx);
}

SAKDebuggerDevice *SAKUdpTransponder::device()
{
    return mDevice;
}

void SAKUdpTransponder::onDeviceStateChanged(bool opened)
{
    mUi->peerHostLineEdit->setEnabled(!opened);
    mUi->peerPortLineEdit->setEnabled(!opened);
}

void SAKUdpTransponder::initSignals()
{
    connect(mDevice, &SAKUdpClientDevice::errorOccurred,
            this, [=](){
        mUi->enableCheckBox->setChecked(false);
    });

    connect(mUi->enableCheckBox, &QCheckBox::clicked, this, [=](){
        setEnable(mUi->enableCheckBox->isChecked());
        if (mUi->enableCheckBox->isChecked()) {
            mDevice->start();
        } else {
            mDevice->exit();
            mDevice->wait();
        }
    });

    connect(mDevice, &SAKUdpClientDevice::clientInfoChanged,
            this, [=](QString info){
        mUi->localInfoLineEdit->setText(info);
    });

    connect(mUi->peerHostLineEdit, &QLineEdit::textChanged,
            this, [=](){
        emit parametersContextChanged();
    });

    connect(mUi->peerPortLineEdit, &QLineEdit::textChanged,
            this, [=](){
        emit parametersContextChanged();
    });
}
