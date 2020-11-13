/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include "SAKModbusCommonRegister.hh"
#include "SAKModbusCommonFlowLayout.hh"
#include "SAKModbusCommonRegisterView.hh"

#include "ui_SAKModbusCommonReigsterView.h"

SAKModbusCommonRegisterView::SAKModbusCommonRegisterView(QModbusDataUnit::RegisterType registerType, QWidget *parent)
    :QWidget(parent)
    ,mRegisterType(registerType)
    ,ui(new Ui::SAKModbusCommonReigsterView)
{
    ui->setupUi(this);

    mFlowLayout = new SAKModbusCommonFlowLayout;
    ui->scrollAreaWidgetContents->setLayout(mFlowLayout);
}

SAKModbusCommonRegisterView::~SAKModbusCommonRegisterView()
{
    delete ui;
}

void SAKModbusCommonRegisterView::addWidget(QWidget *registerWisget)
{
    ui->scrollAreaWidgetContents->layout()->addWidget(registerWisget);
}

void SAKModbusCommonRegisterView::updateRegister(int startAddress, int registerNumber)
{
    // Remove the old widgets
    while (true) {
        QLayoutItem *item = mFlowLayout->takeAt(0);
        if (item){
            delete item->widget();
            delete item;
        }else{
            break;
        }
    }

    for (quint16 i = 0; i < registerNumber; i++){
        addWidget(new SAKModbusCommonRegister(mRegisterType, startAddress + i, startAddress + i));
    }
}
