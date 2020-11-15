/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QLabel>
#include <QDebug>
#include <QLineEdit>
#include <QRegularExpressionValidator>

#include "SAKModbusCommonRegister.hh"
#include "ui_SAKModbusCommonRegister.h"

SAKModbusCommonRegister::SAKModbusCommonRegister(QModbusDataUnit::RegisterType type, quint16 address, quint16 value, QWidget *parent)
    :QWidget(parent)
    ,mType(type)
    ,mAddress(address)
    ,mValue(value)
    ,ui(new Ui::SAKModbusCommonRegister)
{
    ui->setupUi(this);

    QLabel *label = ui->label;
    label->setText(QString("%1").arg(QString::number(mAddress), 5, '0'));

    QLineEdit *lineEdit = ui->lineEdit;
#if 0
    lineEdit->setText(QString::number(mAddress));
#endif

    if (mType == QModbusDataUnit::Coils || mType == QModbusDataUnit::DiscreteInputs){
        QRegularExpression regExp("[01]");
        lineEdit->setValidator(new QRegularExpressionValidator(regExp, this));
    }else{
        QRegularExpression regExp("[a-fA-F0-9]{4}");
        lineEdit->setValidator(new QRegularExpressionValidator(regExp, this));
    }
}

SAKModbusCommonRegister::~SAKModbusCommonRegister()
{
    delete ui;
}

QModbusDataUnit::RegisterType SAKModbusCommonRegister::type()
{
    return mType;
}

quint16 SAKModbusCommonRegister::address()
{
    return mAddress;
}

quint16 SAKModbusCommonRegister::value()
{
    return mValue;
}

void SAKModbusCommonRegister::setValue(quint16 value)
{
    if (mType == QModbusDataUnit::Coils || mType == QModbusDataUnit::DiscreteInputs){
        ui->lineEdit->setText(value ? "1" : "0");
    }else{
        ui->lineEdit->setText(QString("%1").arg(QString::number(value, 16), 4, '0'));
    }
}

void SAKModbusCommonRegister::on_lineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    mValue = ui->lineEdit->text().toInt(Q_NULLPTR, 16);
    emit registerValueChanged(type(), address(), value());
}
