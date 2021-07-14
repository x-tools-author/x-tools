/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QList>
#include <QDebug>
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKDebugger.hh"
#include "SAKCommonInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKWebSocketServerController.hh"
#include "ui_SAKWebSocketServerController.h"

SAKWebSocketServerController::SAKWebSocketServerController(SAKDebugger *debugPage, QWidget *parent)
    :SAKDebugPageController(debugPage, parent)
    ,mUi(new Ui::SAKWebSocketServerController)
{
    mUi->setupUi(this);
    mServerHostComboBox = mUi->serverhostComboBox;
    mServerPortLineEdit = mUi->serverPortLineEdit;
    mClientHostComboBox = mUi->clientHostComboBox;
    mSendingTypeComboBox = mUi->sendingTypeComboBox;

    SAKCommonDataStructure::setComboBoxTextWebSocketSendingType(mSendingTypeComboBox);
    qRegisterMetaType<SAKWebSocketServerController::WebSocketServerParameters>("SAKWebSocketServerController::WebSocketServerParameters");
    mParameters.serverHost = mServerHostComboBox->currentText();
    mParameters.serverPort = mServerPortLineEdit->text().toInt();
    refreshDevice();
}

SAKWebSocketServerController::~SAKWebSocketServerController()
{
    delete mUi;
}

QVariant SAKWebSocketServerController::parameters()
{
    mParametersMutex.lock();
    auto parameters = mParameters;
    mParametersMutex.unlock();

    return QVariant::fromValue(parameters);
}

void SAKWebSocketServerController::setUiEnable(bool opened)
{
    mServerHostComboBox->setEnabled(!opened);
    mServerPortLineEdit->setEnabled(!opened);
}

void SAKWebSocketServerController::refreshDevice()
{
    SAKCommonInterface::addIpItemsToComboBox(mServerHostComboBox);
}

void SAKWebSocketServerController::addClient(QString host, quint16 port, QWebSocket *socket)
{
    QString item = host.append(":");
    item.append(QString::number(port));

    for(int i = 0; i < mClientHostComboBox->count(); i++){
        if (mClientHostComboBox->itemText(i).compare(item) == 0){
            return;
        }
    }

    mClientHostComboBox->addItem(item, QVariant::fromValue(socket));
}

void SAKWebSocketServerController::removeClient(QWebSocket *socket)
{
    for(int i = 0; i < mClientHostComboBox->count(); i++){
        if (mClientHostComboBox->itemData(i).value<QWebSocket*>() == socket){
            mClientHostComboBox->removeItem(i);
            break;
        }
    }
}

void SAKWebSocketServerController::clearClient()
{
    mClientHostComboBox->clear();
}

void SAKWebSocketServerController::on_serverhostComboBox_currentTextChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverHost = arg1;
    mParametersMutex.unlock();
}

void SAKWebSocketServerController::on_serverPortLineEdit_textChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.serverPort = arg1.toInt();
    mParametersMutex.unlock();
}

void SAKWebSocketServerController::on_clientHostComboBox_currentTextChanged(const QString &arg1)
{
    mParametersMutex.lock();
    mParameters.currentClientHost = arg1.split(':').first();
    mParameters.currentClientPort = arg1.split(':').last().toInt();
    mParametersMutex.unlock();
}

void SAKWebSocketServerController::on_sendingTypeComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    mParametersMutex.lock();
    mParameters.sendingType = mSendingTypeComboBox->currentData().toInt();
    mParametersMutex.unlock();
}
