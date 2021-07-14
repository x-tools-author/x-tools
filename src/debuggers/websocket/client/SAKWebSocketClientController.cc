/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QList>
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKDebugPage.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKWebSocketClientController.hh"
#include "ui_SAKWebSocketClientController.h"
SAKWebSocketClientController::SAKWebSocketClientController(SAKDebugPage *debugPage, QWidget *parent)
    :SAKDebugPageController(debugPage, parent)
    ,mUi(new Ui::SAKWebSocketClientController)
{
    mUi->setupUi(this);
    mServerAddressLineEdit = mUi->serverHostLineEdit;
    mSendingTypeComboBox = mUi->sendingTypeComboBox;
    mClientInfoLineEdit = mUi->clientInfoLineEdit;

    SAKCommonDataStructure::setComboBoxTextWebSocketSendingType(mSendingTypeComboBox);
    on_serverHostLineEdit_textChanged(mServerAddressLineEdit->text());
    on_sendingTypeComboBox_currentIndexChanged(0);
    qRegisterMetaType<SAKWebSocketClientController::WebSocketClientParameters>("SAKWebSocketClientController::WebSocketClientParameters");
}

SAKWebSocketClientController::~SAKWebSocketClientController()
{
    delete mUi;
}

QVariant SAKWebSocketClientController::parameters()
{
    mParametersMutex.lock();
    auto parameters = mParameters;
    mParametersMutex.unlock();

    return QVariant::fromValue(parameters);
}

void SAKWebSocketClientController::setUiEnable(bool opend)
{
    mServerAddressLineEdit->setEnabled(!opend);
}

void SAKWebSocketClientController::setClientInfo(QString info)
{
    mClientInfoLineEdit->setText(info);
}

void SAKWebSocketClientController::on_serverHostLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverAddress = arg1;
    mParametersMutex.unlock();
}

void SAKWebSocketClientController::on_sendingTypeComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    mParametersMutex.lock();
    mParameters.sendingType = mSendingTypeComboBox->currentData().toUInt();
    mParametersMutex.unlock();
}
