/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include "SAKTcpTransponder.hh"
#include "SAKTcpClientDevice.hh"
#include "SAKCommonDataStructure.hh"
#include "ui_SAKTcpTransponder.h"

SAKTcpTransponder::SAKTcpTransponder(QWidget *parent)
    :SAKDebuggerPluginTransponder(parent)
    ,mUi(new Ui::SAKTcpTransponder)
    ,mDevice(new SAKTcpClientDevice(Q_NULLPTR, QString(), Q_NULLPTR, Q_NULLPTR))
{
    mUi->setupUi(this);
    setupDevice();
    initSignals();
}

SAKTcpTransponder::SAKTcpTransponder(quint64 id,
                                     SAKTcpClientParametersContext parasCtx,
                                     QWidget *parent)
    :SAKDebuggerPluginTransponder(id, parent)
    ,mUi(new Ui::SAKTcpTransponder)
    ,mDevice(new SAKTcpClientDevice(Q_NULLPTR, QString(), Q_NULLPTR, Q_NULLPTR))
{
    mUi->setupUi(this);
    mUi->serverHostLineEdit->setText(parasCtx.serverHost);
    mUi->serverPortLineEdit->setText(QString::number(parasCtx.serverPort));
    setupDevice();
    initSignals();
}

SAKTcpTransponder::~SAKTcpTransponder()
{
    delete mUi;
    if (mDevice) {
        if (mDevice->isRunning()) {
            mDevice->exit();
            mDevice->wait();
        }
    }
}

QVariant SAKTcpTransponder::parametersContext()
{
    SAKTcpClientParametersContext ctx;
    ctx.serverHost = mUi->serverHostLineEdit->text().trimmed();
    ctx.serverPort = mUi->serverPortLineEdit->text().trimmed().toInt();
    ctx.specifyClientAddressAndPort = false;
    ctx.allowAutomaticConnection = false;
    return QVariant::fromValue(ctx);
}

SAKDebuggerDevice *SAKTcpTransponder::device()
{
    return mDevice;
}

void SAKTcpTransponder::onDeviceStateChanged(bool opened)
{
    mUi->serverHostLineEdit->setEnabled(!opened);
    mUi->serverPortLineEdit->setEnabled(!opened);
}

void SAKTcpTransponder::initSignals()
{
    connect(mUi->enableCheckBox, &QCheckBox::clicked, this, [=](){
        if (mUi->enableCheckBox->isChecked()) {
            mDevice->start();
        } else {
            mDevice->exit();
            mDevice->wait();
        }
    });

    connect(mDevice, &SAKTcpClientDevice::errorOccurred, this, [=](){
        mUi->enableCheckBox->setChecked(false);
    });

    connect(mUi->serverHostLineEdit, &QLineEdit::textChanged, this, [=](){
        emit parametersContextChanged();
    });
    connect(mUi->serverPortLineEdit, &QLineEdit::textChanged, this, [=](){
        emit parametersContextChanged();
    });
}
