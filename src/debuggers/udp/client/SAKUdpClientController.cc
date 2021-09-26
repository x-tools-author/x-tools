/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
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
#include "SAKUdpClientDevice.hh"
#include "SAKUdpClientController.hh"
#include "SAKUdpClientAdvanceSettingWidget.hh"

#include "ui_SAKUdpClientController.h"

SAKUdpClientController::SAKUdpClientController(QSettings *settings,
                                               const QString &settingsGroup,
                                               QWidget *parent)
    :SAKDebuggerController(settings, settingsGroup, parent)
    ,mUi(new Ui::SAKUdpClientController)
{
    mUi->setupUi(this);
    refreshDevice();
}

SAKUdpClientController::~SAKUdpClientController()
{
    delete mUi;
}

void SAKUdpClientController::updateUiState(bool opened)
{
    mUi->localhostComboBox->setEnabled(!opened);
    mUi->localPortlineEdit->setEnabled(!opened);
    mUi->specifyClientAddressAndPort->setEnabled(!opened);
    mUi->targetHostLineEdit->setEnabled(!opened);
    mUi->targetPortLineEdit->setEnabled(!opened);
}

void SAKUdpClientController::refreshDevice()
{
    SAKCommonInterface::addIpItemsToComboBox(mUi->localhostComboBox, true);
}

QVariant SAKUdpClientController::parametersContext()
{
    SAKCommonDataStructure::SAKStructUdpClientParametersContext parasCtx;
    parasCtx.peerHost = mUi->targetHostLineEdit->text().trimmed();
    parasCtx.peerPort = mUi->targetPortLineEdit->text().trimmed().toInt();
    parasCtx.localHost = mUi->localhostComboBox->currentText();
    parasCtx.localPort = mUi->localPortlineEdit->text().trimmed().toInt();
    parasCtx.specifyLocalInfo = mUi->specifyClientAddressAndPort->isChecked();

    return QVariant::fromValue(parasCtx);
}

void SAKUdpClientController::onClientInfoChanged(QString info)
{
    mUi->boundInfoLineEdit->setText(info);
}
