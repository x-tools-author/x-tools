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
    ,mCurClientIndex(0)
{
    mUi->setupUi(this);
    mServerHostComboBox = mUi->serverhostComboBox;
    mServerPortLineEdit = mUi->serverPortLineEdit;
    mClientHostComboBox = mUi->clientHostComboBox;
    mSendingTypeComboBox = mUi->sendingTypeComboBox;
    mClientHostComboBox->addItem(tr("All Connections(%1)").arg(0));
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
           this, [=](int index){
        mCurClientIndex = index;
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
        ctx.currentClientHost = "All Connections";
        ctx.currentClientPort = 0;
    }
    ctx.sendingType = mSendingTypeComboBox->currentData().toInt();

    return QVariant::fromValue(ctx);
}

void SAKWebSocketServerController::addClient(QString host, quint16 port, QWebSocket *socket)
{
    auto index = mClientHostComboBox->findData(QVariant::fromValue(socket));
    if (index != -1)
        return;

    mClientHostComboBox->blockSignals(true);
    QString item = host.append(":");
    item.append(QString::number(port));
    mClientHostComboBox->addItem(item, QVariant::fromValue(socket));
    updateClientHostCombox();
    mClientHostComboBox->blockSignals(false);
}

void SAKWebSocketServerController::removeClient(QWebSocket *socket)
{
    auto index = mClientHostComboBox->findData(QVariant::fromValue(socket));
    if (index == -1)
        return;

    mClientHostComboBox->blockSignals(true);
    mClientHostComboBox->removeItem(index);
    mCurClientIndex = index == mCurClientIndex ? 0 : mCurClientIndex;
    updateClientHostCombox();
    mClientHostComboBox->blockSignals(false);
}

void SAKWebSocketServerController::clearClient()
{
    mClientHostComboBox->clear();
    mClientHostComboBox->addItem(tr("All Connections(%1)").arg(0));
}

void SAKWebSocketServerController::updateClientHostCombox()
{
    mClientHostComboBox->removeItem(0);
    mClientHostComboBox->insertItem(0, tr("All Connections(%1)").arg(mClientHostComboBox->count()));
    mClientHostComboBox->setCurrentIndex(mCurClientIndex);
}
