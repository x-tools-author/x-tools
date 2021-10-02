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

SAKWebSocketServerController::SAKWebSocketServerController(QSettings *settings,
                                                           const QString &settingsGroup,
                                                           QWidget *parent)
    :SAKDebuggerController(settings, settingsGroup, parent)
    ,mUi(new Ui::SAKWebSocketServerController)
{
    mUi->setupUi(this);
    mServerHostComboBox = mUi->serverhostComboBox;
    mServerPortLineEdit = mUi->serverPortLineEdit;
    mClientHostComboBox = mUi->clientHostComboBox;
    mSendingTypeComboBox = mUi->sendingTypeComboBox;
    refreshDevice();
    SAKCommonDataStructure::setComboBoxTextWebSocketSendingType(mSendingTypeComboBox);

    SAKWSServerParametersContext ctx;
    microIni2CoB(settings, settingsGroup, ctx.serverHost, mServerHostComboBox);
    microIni2LE(settings, settingsGroup, ctx.serverPort, mServerPortLineEdit);
    microIni2CoB(settings, settingsGroup, ctx.sendingType, mSendingTypeComboBox);

#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mServerHostComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
#else
    connect(mServerHostComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
#endif

            this, [=](){
        emit parametersContextChanged();
        microCoB2Ini(settings, settingsGroup, ctx.serverHost, mServerHostComboBox);
    });
    connect(mServerPortLineEdit, &QLineEdit::textChanged, this, [=](){
        emit parametersContextChanged();
        microLE2Ini(settings, settingsGroup, ctx.serverPort, mServerPortLineEdit);
    });


#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mSendingTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
#else
    connect(mSendingTypeComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
#endif
            this, [=](){
        emit parametersContextChanged();
        microCoB2Ini(settings, settingsGroup, ctx.sendingType, mSendingTypeComboBox);
    });


#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mClientHostComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
#else
    connect(mClientHostComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
#endif
           this, [=](){
        emit parametersContextChanged();
    });
}

SAKWebSocketServerController::~SAKWebSocketServerController()
{
    delete mUi;
}

void SAKWebSocketServerController::updateUiState(bool opened)
{
    mServerHostComboBox->setEnabled(!opened);
    mServerPortLineEdit->setEnabled(!opened);
}

void SAKWebSocketServerController::refreshDevice()
{
    SAKCommonInterface::addIpItemsToComboBox(mServerHostComboBox);
}

QVariant SAKWebSocketServerController::parametersContext()
{
    SAKWSServerParametersContext ctx;
    ctx.serverHost = mServerHostComboBox->currentText();
    ctx.serverPort = mServerPortLineEdit->text().toInt();
    QString currentText = mClientHostComboBox->currentText();
    QStringList info = currentText.split(':');
    if (info.length() == 2) {
        ctx.currentClientHost = info.first();
        ctx.currentClientPort = info.last().toInt();
    } else {
        ctx.currentClientHost.clear();
        ctx.currentClientPort = 0;
    }
    ctx.sendingType = mSendingTypeComboBox->currentData().toInt();

    return QVariant::fromValue(ctx);
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
