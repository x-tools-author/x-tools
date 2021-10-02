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

#include "ui_SAKUdpClientController.h"

SAKUdpClientController::SAKUdpClientController(QSettings *settings,
                                               const QString &settingsGroup,
                                               QWidget *parent)
    :SAKDebuggerController(settings, settingsGroup, parent)
    ,mUi(new Ui::SAKUdpClientController)
{
    mUi->setupUi(this);
    refreshDevice();

    // Read in settings date.
    SAKUdpClientParametersContext ctx;
    microIni2CoB(settings, settingsGroup, ctx.localHost, mUi->localhostComboBox);
    microIni2LE(settings, settingsGroup, ctx.localPort, mUi->localPortlineEdit);
    microIni2LE(settings, settingsGroup, ctx.peerHost, mUi->targetHostLineEdit);
    microIni2LE(settings, settingsGroup, ctx.peerPort, mUi->targetPortLineEdit);
    microIni2ChB(settings, settingsGroup,
                 ctx.specifyLocalInfo, mUi->specifyClientAddressAndPort);

#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mUi->localhostComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
#else
    connect(mUi->localhostComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
#endif
            this, [=](int index){
        Q_UNUSED(index);
        emit parametersContextChanged();
        microCoB2Ini(settings, settingsGroup, ctx.localHost, mUi->localhostComboBox);
    });
    connect(mUi->localPortlineEdit, &QLineEdit::textChanged,
            this, [=](const QString &text){
        Q_UNUSED(text);
        emit parametersContextChanged();
        microLE2Ini(settings, settingsGroup, ctx.localPort, mUi->localPortlineEdit);
    });
    connect(mUi->specifyClientAddressAndPort, &QCheckBox::clicked,
            this, [=](){
        emit parametersContextChanged();
        microChB2Ini(settings, settingsGroup,
                     ctx.specifyLocalInfo, mUi->specifyClientAddressAndPort);
    });
    connect(mUi->targetHostLineEdit, &QLineEdit::textChanged,
            this, [=](const QString &text){
        Q_UNUSED(text);
        emit parametersContextChanged();
        microLE2Ini(settings, settingsGroup, ctx.peerHost, mUi->targetHostLineEdit);
    });
    connect(mUi->targetPortLineEdit, &QLineEdit::textChanged,
            this, [=](const QString &text){
        Q_UNUSED(text);
        emit parametersContextChanged();
        microLE2Ini(settings, settingsGroup, ctx.peerPort, mUi->targetPortLineEdit);
    });
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
#if 0
    mUi->targetHostLineEdit->setEnabled(!opened);
    mUi->targetPortLineEdit->setEnabled(!opened);
#endif
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
