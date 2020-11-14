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
#if 0
    QLineEdit *lineEdit = ui->lineEdit;
    lineEdit->setText(QString::number(mAddress));
#endif
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
    ui->lineEdit->setText(QString::number(value));
}

void SAKModbusCommonRegister::on_lineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    mValue = ui->lineEdit->text().toInt();
    emit registerValueChanged(type(), address(), value());
}
