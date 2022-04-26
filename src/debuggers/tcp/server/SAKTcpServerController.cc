/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
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
#include "SAKTcpServerController.hh"
#include "SAKCommonDataStructure.hh"
#include "ui_SAKTcpServerController.h"

SAKTcpServerController::SAKTcpServerController(QSettings *settings,
                                               const QString &settingsGroup,
                                               QWidget *parent)
    :SAKDebuggerController(settings, settingsGroup, parent)
    ,mUi(new Ui::SAKTcpServerController)
    ,mCurClientIndex(0)
{
    mUi->setupUi(this);
    mServerHostComboBox = mUi->serverhostComboBox;
    mServerPortLineEdit = mUi->serverPortLineEdit;
    mClientHostComboBox = mUi->clientHostComboBox;
    mClientHostComboBox->addItem(tr("All Connections(%1)").arg(0));
    refreshDevice();

    // Read in settings data.
    SAKTcpServerParametersContext ctx;
    microIni2CoB(settings, settingsGroup, ctx.serverHost, mServerHostComboBox);
    microIni2LE(settings, settingsGroup, ctx.serverPort, mServerPortLineEdit);
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
#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mServerHostComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
#else
    connect(mServerHostComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
#endif

            this, [=](int index){
        Q_UNUSED(index);
        emit parametersContextChanged();
        microCoB2Ini(settings, settingsGroup, ctx.serverHost, mServerHostComboBox);
    });
    connect(mServerPortLineEdit, &QLineEdit::textChanged, this, [=](const QString &text){
        Q_UNUSED(text);
        emit parametersContextChanged();
        microLE2Ini(settings, settingsGroup, ctx.serverPort, mServerPortLineEdit);
    });
}

SAKTcpServerController::~SAKTcpServerController()
{
    delete mUi;
}

void SAKTcpServerController::updateUiState(bool opened)
{
    mServerHostComboBox->setEnabled(!opened);
    mServerPortLineEdit->setEnabled(!opened);
    if (!opened) {
        mClientHostComboBox->clear();
        mClientHostComboBox->addItem(tr("All Connections(%1)").arg(0));
    }
}

void SAKTcpServerController::refreshDevice()
{
    SAKCommonInterface::addIpItemsToComboBox(mServerHostComboBox);
}

QVariant SAKTcpServerController::parametersContext()
{
    SAKCommonDataStructure::SAKStructTcpServerParametersContext parameters;
    parameters.serverHost = mServerHostComboBox->currentText().trimmed();
    parameters.serverPort = mServerPortLineEdit->text().trimmed().toInt();

    const QString currentText = mClientHostComboBox->currentText().trimmed();
    const QStringList infos = currentText.split(":");
    if (infos.length() == 2) {
        parameters.currentClientHost = infos.first();
        parameters.currentClientPort = infos.last().toInt();
    } else {
        parameters.currentClientHost = "All Connections";
        parameters.currentClientPort = 0;
    }

    return QVariant::fromValue(parameters);
}

void SAKTcpServerController::onAddClient(QString host, quint16 port, QTcpSocket *socket)
{
    auto index = mClientHostComboBox->findData(QVariant::fromValue(socket));
    if (index != -1)
        return;

    mClientHostComboBox->blockSignals(true);
    QString itemString = host.append(":");
    itemString.append(QString::number(port));
    mClientHostComboBox->addItem(itemString, QVariant::fromValue(socket));
    emit parametersContextChanged();
    updateClientHostCombox();
    mClientHostComboBox->blockSignals(false);
}

void SAKTcpServerController::onRemoveClient(QTcpSocket *socket)
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

void SAKTcpServerController::updateClientHostCombox()
{
    mClientHostComboBox->removeItem(0);
    mClientHostComboBox->insertItem(0, tr("All Connections(%1)").arg(mClientHostComboBox->count()));
    mClientHostComboBox->setCurrentIndex(mCurClientIndex);
}
