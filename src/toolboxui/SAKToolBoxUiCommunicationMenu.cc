/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QWidgetAction>
#include "SAKToolBox.hh"
#include "SAKToolBoxUiCommunicationMenu.hh"
#include "ui_SAKToolBoxUiCommunicationMenu.h"

SAKToolBoxUiCommunicationMenu::SAKToolBoxUiCommunicationMenu(QWidget *parent)
    : QMenu(parent)
    , ui(new Ui::SAKToolBoxUiCommunicationMenu)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    QWidgetAction *action = new QWidgetAction(this);
    action->setDefaultWidget(w);
    addAction(action);

    ui->widgetTxMasker->setToolName("Tx masker");
    ui->widgetRxMasker->setToolName("Rx masker");
    ui->widgetTxAnanyzer->setToolName(tr("Tx analyzer"));
    ui->widgetRxAnanlyzer->setToolName(tr("Rx analyzer"));
}

SAKToolBoxUiCommunicationMenu::~SAKToolBoxUiCommunicationMenu()
{
    delete ui;
}

void SAKToolBoxUiCommunicationMenu::initialize(SAKToolBox *toolBox,
                                        const QString &settingsGroup)
{
    auto txM = toolBox->getTxMaskerTool();
    auto rxM = toolBox->getRxMaskerTool();
    auto txA = toolBox->getTxAnalyzerTool();
    auto rxA = toolBox->getRxAnalyzerTool();
    auto storer = toolBox->getStorerTool();

    auto txMGroup = settingsGroup + "/txMasker";
    auto rxMGroup = settingsGroup + "/rxMasker";
    auto txAGroup = settingsGroup + "/txAnalyzer";
    auto rxAGroup = settingsGroup + "/rxAnalyzer";
    auto storerGroup = settingsGroup + "/storer";

    ui->widgetRxMasker->initialize(rxM, rxMGroup);
    ui->widgetTxMasker->initialize(txM, txMGroup);
    ui->widgetTxAnanyzer->initialize(txA, txAGroup);
    ui->widgetRxAnanlyzer->initialize(rxA, rxAGroup);
    ui->widgetStorer->initialize(storer, storerGroup);
}
