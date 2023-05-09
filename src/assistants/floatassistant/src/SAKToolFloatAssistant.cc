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
#include "SAKCommonInterface.hh"
#include "SAKToolFloatAssistant.hh"
#include "ui_SAKToolFloatAssistant.h"

SAKToolFloatAssistant::SAKToolFloatAssistant(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::SAKToolFloatAssistant)
{
    ui->setupUi(this);
    mCommonInterface = new SAKCommonInterface(this);
    mCommonInterface->setLineEditValidator(ui->rawDataLineEdit, SAKCommonInterface::ValidatorFloat);
    on_createPushButton_clicked();
}

SAKToolFloatAssistant::~SAKToolFloatAssistant()
{
    delete ui;
}

void SAKToolFloatAssistant::fixedLength(QStringList &stringList)
{
    if (ui->bigEndianCheckBox->isChecked()){
        if (ui->floatRadioButton->isChecked()){
            if (stringList.length() < int(sizeof(float))){
                int len = int(sizeof(float)) - stringList.length();
                for (int i = 0; i < len; i++){
                    stringList.prepend(QString(" 00"));
                }
            }
        }else{
            if (stringList.length() < int(sizeof(double))){
                int len = int(sizeof(double)) - stringList.length();
                for (int i = 0; i < len; i++){
                    stringList.prepend(QString(" 00"));
                }
            }
        }
    }else{
        if (ui->floatRadioButton->isChecked()){
            if (stringList.length() < int(sizeof(float))){
                int len = int(sizeof(float)) - stringList.length();
                for (int i = 0; i < len; i++){
                    stringList.append(QString(" 00"));
                }
            }
        }else{
            if (stringList.length() < int(sizeof(double))){
                int len = int(sizeof(double)) - stringList.length();
                for (int i = 0; i < len; i++){
                    stringList.append(QString(" 00"));
                }
            }
        }
    }
}

void SAKToolFloatAssistant::on_hexRawDataCheckBox_clicked()
{
    ui->rawDataLineEdit->clear();
    if (ui->hexRawDataCheckBox->isChecked()){
        mCommonInterface->setLineEditValidator(ui->rawDataLineEdit, SAKCommonInterface::ValidatorHex);
    }else{
        mCommonInterface->setLineEditValidator(ui->rawDataLineEdit, SAKCommonInterface::ValidatorFloat);
    }
}

void SAKToolFloatAssistant::on_createPushButton_clicked()
{
    if (ui->hexRawDataCheckBox->isChecked()){
        ui->rawDataLineEdit->setMaxLength(ui->floatRadioButton->isChecked() ? 11 : 23);
        QString rawDataString = ui->rawDataLineEdit->text().trimmed();
        QStringList rawDataStringList = rawDataString.split(' ');
        fixedLength(rawDataStringList);

        QByteArray data;
        for (int i = 0; i < rawDataStringList.length(); i++){
            bool isBigEndian = ui->bigEndianCheckBox->isChecked();
            quint8 value = quint8(rawDataStringList.at(isBigEndian ? i : rawDataStringList.length()-1-i).toInt());
            data.append(reinterpret_cast<char*>(&value), 1);
        }

        if (ui->floatRadioButton->isChecked()){
            float *f = reinterpret_cast<float*>(data.data());
            ui->friendlyCookedDataLineEdit->setText(QString("%1").arg(*f));
            data = mCommonInterface->byteArrayToHex(data, ' ');
            ui->hexCookedDataLineEdit->setText(QString(data));
        }else{
            double *d = reinterpret_cast<double*>(data.data());
            ui->friendlyCookedDataLineEdit->setText(QString("%1").arg(*d));
            data = mCommonInterface->byteArrayToHex(data, ' ');
            ui->hexCookedDataLineEdit->setText(QString(data));
        }
    }else{
        QByteArray data;
        ui->rawDataLineEdit->setMaxLength(INT_MAX);
        if (ui->floatRadioButton->isChecked()){
            float value = ui->rawDataLineEdit->text().trimmed().toFloat();
            if (ui->bigEndianCheckBox->isChecked()){
                // To big endian
                float temp = value;
                quint8 *ptr = reinterpret_cast<quint8*>(&value);
                for (int i = 0;i < int(sizeof(value)); i++){
                    ptr[i] = reinterpret_cast<quint8*>(&temp)[int(sizeof(value)) - 1 - i];
                }
            }
            data.append(reinterpret_cast<char*>(&value), sizeof(value));
        }else{
            double value = ui->rawDataLineEdit->text().trimmed().toFloat();
            if (ui->bigEndianCheckBox->isChecked()){
                // To big endian
                double temp = value;
                quint8 *ptr = reinterpret_cast<quint8*>(&value);
                for (int i = 0;i < int(sizeof(value)); i++){
                    ptr[i] = reinterpret_cast<quint8*>(&temp)[int(sizeof(value)) - 1 - i];
                }
            }
            data.append(reinterpret_cast<char*>(&value), sizeof(value));
        }
        ui->friendlyCookedDataLineEdit->setText(ui->rawDataLineEdit->text());
        data = mCommonInterface->byteArrayToHex(data, ' ');
        ui->hexCookedDataLineEdit->setText(QString(data));
    }
}

void SAKToolFloatAssistant::on_rawDataLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    on_createPushButton_clicked();
}

void SAKToolFloatAssistant::on_bigEndianCheckBox_clicked()
{
    on_createPushButton_clicked();
}

void SAKToolFloatAssistant::on_floatRadioButton_clicked()
{
    on_createPushButton_clicked();
}

void SAKToolFloatAssistant::on_doubleRadioButton_clicked()
{
    on_createPushButton_clicked();
}
