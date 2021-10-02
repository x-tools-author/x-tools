/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include "SAKWebSocketTransponder.hh"
#include "SAKWebSocketClientDevice.hh"
#include "ui_SAKWebSocketTransponder.h"

SAKWebSocketTransponder::SAKWebSocketTransponder(QWidget *parent)
    :SAKDebuggerPluginTransponder(parent)
    ,mUi(new Ui::SAKWebSocketTransponder)
    ,mDevice(new SAKWebSocketClientDevice(Q_NULLPTR, QString(), Q_NULLPTR, Q_NULLPTR))
{
    mUi->setupUi(this);
    SAKCommonDataStructure::setComboBoxTextWebSocketSendingType(mUi->sendingTypeComboBox);
    setupDevice();
    initSignals();
}

SAKWebSocketTransponder::SAKWebSocketTransponder(quint64 id,
                                                 SAKWSClientParametersContext parasCtx,
                                                 QWidget *parent)
    :SAKDebuggerPluginTransponder(id, parent)
    ,mUi(new Ui::SAKWebSocketTransponder)
    ,mDevice(new SAKWebSocketClientDevice(Q_NULLPTR, QString(), Q_NULLPTR, Q_NULLPTR))
{
    mUi->setupUi(this);
    SAKCommonDataStructure::setComboBoxTextWebSocketSendingType(mUi->sendingTypeComboBox);
    mUi->serverAddressLineEdit->setText(parasCtx.serverAddress);
    mUi->sendingTypeComboBox->setCurrentIndex(parasCtx.sendingType);
    setupDevice();
    initSignals();
}

QVariant SAKWebSocketTransponder::parametersContext()
{
    SAKWSClientParametersContext ctx;
    ctx.sendingType = mUi->sendingTypeComboBox->currentData().toInt();
    ctx.serverAddress = mUi->serverAddressLineEdit->text().trimmed();
    return QVariant::fromValue(ctx);
}

SAKDebuggerDevice *SAKWebSocketTransponder::device()
{
    return mDevice;
}

void SAKWebSocketTransponder::onDeviceStateChanged(bool opened)
{
    mUi->serverAddressLineEdit->setEnabled(!opened);
    mUi->sendingTypeComboBox->setEnabled(!opened);
}

void SAKWebSocketTransponder::initSignals()
{
    connect(mUi->enableCheckBox, &QCheckBox::clicked, this, [=](){
        if (mUi->enableCheckBox->isChecked()) {
            mDevice->start();
        } else {
            mDevice->exit();
            mDevice->wait();
        }
    });

    connect(mDevice, &SAKWebSocketClientDevice::errorOccurred, this, [=](){
        mUi->enableCheckBox->setChecked(false);
    });

    connect(mUi->serverAddressLineEdit, &QLineEdit::textChanged, this, [=](){
        emit parametersContextChanged();
    });

    connect(mUi->sendingTypeComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            [=](){
        emit parametersContextChanged();
    });
}
