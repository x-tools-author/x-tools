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
    ,mReigsterCount(0)
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

    mReigsterCount = registerNumber;
    SAKModbusCommonRegister *registerWidget = Q_NULLPTR;
    for (quint16 i = 0; i < registerNumber; i++){
        registerWidget = new SAKModbusCommonRegister(mRegisterType, startAddress + i, startAddress + i, this);
        ui->scrollAreaWidgetContents->layout()->addWidget(registerWidget);
        connect(registerWidget, &SAKModbusCommonRegister::registerValueChanged, this, &SAKModbusCommonRegisterView::registerValueChanged);
    }

    if (registerWidget){
        emit invokeUpdateRegisterValue(registerWidget->type(), startAddress, registerNumber);
    }
}

void SAKModbusCommonRegisterView::updateRegisterValue(quint16 address, quint16 value)
{
    for (int i = 0; i < mReigsterCount; i++ ){
        QLayoutItem *item = mFlowLayout->itemAt(i);
        auto itemWidget = qobject_cast<SAKModbusCommonRegister*>(item->widget());
        if (itemWidget && (address == itemWidget->address())){
            itemWidget->blockSignals(true);
            itemWidget->setValue(value);
            itemWidget->blockSignals(false);
            break;
        }
    }
}

QModbusDataUnit::RegisterType SAKModbusCommonRegisterView::registerType()
{
    return mRegisterType;
}
