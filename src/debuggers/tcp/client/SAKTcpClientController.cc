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
#include <QDebug>
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKDebugger.hh"
#include "SAKCommonInterface.hh"
#include "SAKTcpClientController.hh"
#include "SAKCommonDataStructure.hh"
#include "ui_SAKTcpClientController.h"

SAKTcpClientController::SAKTcpClientController(QSettings *settings,
                                               const QString &settingsGroup,
                                               QWidget *parent)
    :SAKDebuggerController(settings, settingsGroup, parent)
    ,mUi(new Ui::SAKTcpClientController)
{
    mUi->setupUi(this);

    mLocalhostComboBox = mUi->localhostComboBox;
    mLocalPortlineEdit = mUi->localPortlineEdit;
    mSpecifyClientAddressAndPort = mUi->specifyClientAddressAndPort;
    mAutomaticConnectionCheckBox = mUi->automaticConnectionCheckBox;
    mClientInfoLineEdit = mUi->clientInfoLineEdit;
    mServerHostLineEdit = mUi->serverHostLineEdit;
    mServerPortLineEdit = mUi->serverPortLineEdit;
    refreshDevice();

    // Read in settings data.
    SAKUdpClientParametersContext ctx;
    microIni2CoB(settings, settingsGroup, ctx.localHost, mLocalhostComboBox);
    microIni2LE(settings, settingsGroup, ctx.locarPort, mLocalPortlineEdit);
    microIni2LE(settings, settingsGroup, ctx.peerHost, mServerHostLineEdit);
    microIni2LE(settings, settingsGroup, ctx.peerPort, mServerPortLineEdit);
    microIni2ChB(settings, settingsGroup,
                 ctx.specifyLocalInfo, mSpecifyClientAddressAndPort);

#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mLocalhostComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
#else
    connect(mLocalhostComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
#endif
            this, [=](int index){
        Q_UNUSED(index);
        emit parametersContextChanged();
        microCoB2Ini(settings, settingsGroup, ctx.localHost, mLocalhostComboBox);
    });
    connect(mLocalPortlineEdit, &QLineEdit::textChanged,
            this, [=](const QString &text){
        Q_UNUSED(text);
        emit parametersContextChanged();
        microLE2Ini(settings, settingsGroup, ctx.locarPort, mLocalPortlineEdit);
    });
    connect(mSpecifyClientAddressAndPort, &QCheckBox::clicked,
            this, [=](){
        emit parametersContextChanged();
        microChB2Ini(settings, settingsGroup,
                     ctx.specifyLocalInfo, mSpecifyClientAddressAndPort);
    });
    connect(mAutomaticConnectionCheckBox, &QCheckBox::clicked,
            this, [=](){
        emit parametersContextChanged();
    });
    connect(mServerHostLineEdit, &QLineEdit::textChanged,
            this, [=](const QString &text){
        Q_UNUSED(text);
        emit parametersContextChanged();
        microLE2Ini(settings, settingsGroup, ctx.peerHost, mServerHostLineEdit);
    });
    connect(mServerPortLineEdit, &QLineEdit::textChanged,
            this, [=](const QString &text){
        Q_UNUSED(text);
        emit parametersContextChanged();
        microLE2Ini(settings, settingsGroup, ctx.peerPort, mServerPortLineEdit);
    });
}

SAKTcpClientController::~SAKTcpClientController()
{
    delete mUi;
}

void SAKTcpClientController::updateUiState(bool opened)
{
    mLocalhostComboBox->setEnabled(!opened);
    mLocalPortlineEdit->setEnabled(!opened);
    mSpecifyClientAddressAndPort->setEnabled(!opened);
    mServerHostLineEdit->setEnabled(!opened);
    mServerPortLineEdit->setEnabled(!opened);
}

void SAKTcpClientController::refreshDevice()
{
    SAKCommonInterface::addIpItemsToComboBox(mLocalhostComboBox);
}

QVariant SAKTcpClientController::parametersContext()
{
    SAKCommonDataStructure::SAKStructTcpClientParametersContext parameters;
    mParametersMutex.lock();
    parameters.localHost = mLocalhostComboBox->currentText();
    parameters.localPort = mLocalPortlineEdit->text().toInt();
    parameters.specifyClientAddressAndPort = mSpecifyClientAddressAndPort->isChecked();
    parameters.serverHost = mServerHostLineEdit->text();
    parameters.serverPort = mServerPortLineEdit->text().toInt();
    mParametersMutex.unlock();
    return QVariant::fromValue(parameters);
}

void SAKTcpClientController::onServerInfoChanged(QString info)
{
    mClientInfoLineEdit->setText(info);
}
