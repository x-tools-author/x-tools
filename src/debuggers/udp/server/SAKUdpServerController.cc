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
#include "SAKUdpServerController.hh"
#include "ui_SAKUdpServerController.h"

SAKUdpServerController::SAKUdpServerController(QSettings *settings,
                                               const QString &settingsGroup,
                                               QWidget *parent)
    :SAKDebuggerController(settings, settingsGroup, parent)
    ,mUi(new Ui::SAKUdpServerController)
{
    mUi->setupUi(this);
    refreshDevice();

    // Read in settings date.
    SAKUdpServerParametersContext ctx;
    microIni2CoB(settings, settingsGroup, ctx.serverHost, mUi->serverhostComboBox);
    microIni2LE(settings, settingsGroup, ctx.serverPort, mUi->serverPortLineEdit);

#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mUi->serverhostComboBox, QOverload<int>::of(&QComboBox::activated),
#else
    connect(mUi->serverhostComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
#endif
            this, [=](int index){
        Q_UNUSED(index);
        emit parametersContextChanged();
        microCoB2Ini(settings, settingsGroup, ctx.serverHost, mUi->serverhostComboBox);
    });
    connect(mUi->serverPortLineEdit, &QLineEdit::textChanged,
            this, [=](const QString &text){
        Q_UNUSED(text);
        emit parametersContextChanged();
        microLE2Ini(settings, settingsGroup, ctx.serverPort, mUi->serverPortLineEdit);
    });
}

SAKUdpServerController::~SAKUdpServerController()
{
    delete mUi;
}

void SAKUdpServerController::updateUiState(bool opened)
{
    mUi->serverhostComboBox->setEnabled(!opened);
    mUi->serverPortLineEdit->setEnabled(!opened);
}

void SAKUdpServerController::refreshDevice()
{
    SAKCommonInterface::addIpItemsToComboBox(mUi->serverhostComboBox);
}

QVariant SAKUdpServerController::parametersContext()
{
    SAKCommonDataStructure::SAKStructUdpServerParametersContext ctx;

    ctx.serverHost = mUi->serverhostComboBox->currentText().trimmed();
    ctx.serverPort = mUi->serverPortLineEdit->text().trimmed().toInt();

    QString currentText = mUi->clientHostComboBox->currentText();
    QStringList infoList = currentText.trimmed().split(':');
    if (infoList.length() == 2) {
        ctx.currentClientHost = infoList.first();
        ctx.currentClientPort = infoList.last().toInt();
    }

    for (int i = 0; i < mUi->clientHostComboBox->count(); i++) {
        QString text = mUi->clientHostComboBox->itemText(i);
        ctx.clients.append(text);
    }

    return QVariant::fromValue(ctx);
}

void SAKUdpServerController::onAddClient(QString host, quint16 port)
{
    QString item = host.append(":");
    item.append(QString::number(port));

    bool isItemExisted = false;
    for(int i = 0; i < mUi->clientHostComboBox->count(); i++){
        if (mUi->clientHostComboBox->itemText(i).compare(item) == 0){
            isItemExisted = true;
            break;
        }
    }

    if (!isItemExisted){
        mUi->clientHostComboBox->addItem(item);
        emit parametersContextChanged();
    }
}
