/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QModbusDataUnit>
#include <QStandardItemModel>

#include "SAKModbusCommonClientSection.hh"
#include "ui_SAKModbusCommonClientSection.h"

SAKModbusCommonClientSection::SAKModbusCommonClientSection(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKModbusCommonClientSection)
{
    ui->setupUi(this);

    struct Info {
        QString name;
        int registerType;
    };

    QList<Info> infoList;
    infoList << Info{tr("Read Coils"), QModbusDataUnit::Coils}
             << Info{tr("Read Discrete Inputs"), QModbusDataUnit::DiscreteInputs}
             << Info{tr("Read Holding Registers"), QModbusDataUnit::HoldingRegisters}
             << Info{tr("Write Coils"), QModbusDataUnit::Coils}
             << Info{tr("Write Input Registers"), QModbusDataUnit::InputRegisters}
             << Info{tr("Write Holding Registers"), QModbusDataUnit::HoldingRegisters};

    QStandardItemModel *itemModel = new QStandardItemModel(this);
    ui->functionCodeComboBox->clear();
    for (auto var : infoList){
        auto item = new QStandardItem(var.name);
        item->setToolTip(var.name);
        itemModel->appendRow(item);
    }
    ui->functionCodeComboBox->setModel(itemModel);
    for (int i = 0; i < infoList.length(); i++){
        ui->functionCodeComboBox->setItemData(i, infoList.at(i).registerType);
    }
}

SAKModbusCommonClientSection::~SAKModbusCommonClientSection()
{
    delete ui;
}
