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
{
    mUi->setupUi(this);
    mServerHostComboBox = mUi->serverhostComboBox;
    mServerPortLineEdit = mUi->serverPortLineEdit;
    mClientHostComboBox = mUi->clientHostComboBox;
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
        Q_UNUSED(index);
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
        parameters.currentClientHost.clear();
        parameters.currentClientPort = 0;
    }

    return QVariant::fromValue(parameters);
}

void SAKTcpServerController::onAddClient(QString host, quint16 port, QTcpSocket *socket)
{
    QString itemString = host.append(":");
    itemString.append(QString::number(port));

    for(int i = 0; i < mClientHostComboBox->count(); i++){
        if (mClientHostComboBox->itemText(i).compare(itemString) == 0){
            mClientHostComboBox->removeItem(i);
            break;
        }
    }

    mClientHostComboBox->addItem(itemString, QVariant::fromValue(socket));
    emit parametersContextChanged();
}

void SAKTcpServerController::onRemoveClient(QTcpSocket *socket)
{
    for(int i = 0; i < mClientHostComboBox->count(); i++){
        if (mClientHostComboBox->itemData(i).value<QTcpSocket*>() == socket){
            mClientHostComboBox->removeItem(i);
            if (i == 0) {
                emit parametersContextChanged();
            }
            break;
        }
    }
}
