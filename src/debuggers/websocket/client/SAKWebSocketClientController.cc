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
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKDebugger.hh"
#include "SAKCommonInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKWebSocketClientController.hh"
#include "ui_SAKWebSocketClientController.h"
SAKWebSocketClientController::SAKWebSocketClientController(QSettings *settings,
                                                           const QString &settingsGroup,
                                                           QWidget *parent)
    :SAKDebuggerController(settings, settingsGroup, parent)
    ,mUi(new Ui::SAKWebSocketClientController)
{
    mUi->setupUi(this);
    mServerAddressLineEdit = mUi->serverHostLineEdit;
    mSendingTypeComboBox = mUi->sendingTypeComboBox;
    mClientInfoLineEdit = mUi->clientInfoLineEdit;
    SAKCommonDataStructure::setComboBoxTextWebSocketSendingType(mSendingTypeComboBox);

    // Read in settings data.
    SAKWSClientParametersContext ctx;
    microIni2LE(settings, settingsGroup, ctx.serverAddress, mServerAddressLineEdit);
    microIni2CoB(settings, settingsGroup, ctx.sendingType, mSendingTypeComboBox);

    connect(mServerAddressLineEdit, &QLineEdit::textChanged, this, [=](){
        emit parametersContextChanged();
        microLE2Ini(settings, settingsGroup, ctx.serverAddress, mServerAddressLineEdit);
    });
    connect(mSendingTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [=](){
        emit parametersContextChanged();
        microCoB2Ini(settings, settingsGroup, ctx.sendingType, mSendingTypeComboBox);
    });
}

SAKWebSocketClientController::~SAKWebSocketClientController()
{
    delete mUi;
}

void SAKWebSocketClientController::updateUiState(bool opened)
{
    mServerAddressLineEdit->setEnabled(!opened);
}

void SAKWebSocketClientController::refreshDevice()
{
    // Nothing to do.
}

QVariant SAKWebSocketClientController::parametersContext()
{
    SAKWSClientParametersContext ctx;
    ctx.serverAddress = mServerAddressLineEdit->text().trimmed();
    ctx.sendingType = mSendingTypeComboBox->currentData().toInt();
    return QVariant::fromValue(ctx);
}

void SAKWebSocketClientController::onClientInfoChanged(QString info)
{
    mClientInfoLineEdit->setText(info);
}
