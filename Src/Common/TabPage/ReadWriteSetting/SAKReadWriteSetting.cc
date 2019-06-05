/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2019 wuhai persionnal. All rights reserved.
*******************************************************************************/
#include <QDebug>
#include <QIntValidator>
#include <QRegExpValidator>

#include "SAKReadWriteSetting.hh"
#include "ui_SAKReadWriteSetting.h"

SAKReadWriteSetting::SAKReadWriteSetting(QWidget* parent)
    :QDialog (parent)
    ,ui(new Ui::SAKReadWriteSetting)
{
    ui->setupUi(this);

    readDelayEnableCheckBox = ui->checkBoxRead;
    writeDelayEnableCheckBox = ui->checkBoxWrite;
    readDelayComboBox = ui->comboBoxRead;
    writeDelayComboBox = ui->comboBoxWrite;
    readCustomDelayCheckBox = ui->checkBoxCustomRead;
    writeCustomDelayCheckBox = ui->checkBoxCustomWrite;
    readLineEdit = ui->lineEditRead;
    writeLineEdit = ui->lineEditWrite;
    setModal(true);

    connect(readDelayEnableCheckBox, &QCheckBox::clicked, this, &SAKReadWriteSetting::setEnableReadDelay);
    connect(writeDelayEnableCheckBox, &QCheckBox::clicked, this, &SAKReadWriteSetting::setEnableWriteDelay);

    connect(readCustomDelayCheckBox, &QCheckBox::clicked, this, &SAKReadWriteSetting::changedReadDelayTime);
    connect(writeCustomDelayCheckBox, &QCheckBox::clicked, this, &SAKReadWriteSetting::changedWriteDelayTime);

    connect(readDelayComboBox, &QComboBox::currentTextChanged, this, &SAKReadWriteSetting::changedReadDelayTime);
    connect(writeDelayComboBox, &QComboBox::currentTextChanged, this, &SAKReadWriteSetting::changedWriteDelayTime);

    connect(readLineEdit, &QLineEdit::textChanged, this, &SAKReadWriteSetting::changedReadDelayTime);
    connect(writeLineEdit, &QLineEdit::textChanged, this, &SAKReadWriteSetting::changedWriteDelayTime);

    initUi();
}

void SAKReadWriteSetting::initUi()
{
    readLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9][0-9]"), readLineEdit));
    writeLineEdit->setValidator(new QIntValidator(1, 99, writeLineEdit));


    readDelayEnableCheckBox->setChecked(true);
    readDelayComboBox->setEnabled(true);
    readCustomDelayCheckBox->setChecked(false);
    readLineEdit->setEnabled(true);

    writeDelayEnableCheckBox->setChecked(true);
    writeDelayComboBox->setEnabled(true);
    writeCustomDelayCheckBox->setChecked(false);
    writeLineEdit->setEnabled(true);

    changedReadDelayTime();
    changedWriteDelayTime();
}

void SAKReadWriteSetting::setEnableReadDelay()
{
    if (readDelayEnableCheckBox->isChecked()){
        readDelayEnableCheckBox->setChecked(true);
        readDelayComboBox->setEnabled(true);
        readCustomDelayCheckBox->setEnabled(true);
        readLineEdit->setEnabled(true);
    }else {
        readDelayEnableCheckBox->setChecked(false);
        readDelayComboBox->setEnabled(false);
        readCustomDelayCheckBox->setEnabled(false);
        readLineEdit->setEnabled(false);
    }

    changedReadDelayTime();
}

void SAKReadWriteSetting::setEnableWriteDelay()
{
    if (writeDelayEnableCheckBox->isChecked()){
        writeDelayEnableCheckBox->setChecked(true);
        writeDelayComboBox->setEnabled(true);
        writeCustomDelayCheckBox->setEnabled(true);
        writeLineEdit->setEnabled(true);
    }else {
        writeDelayEnableCheckBox->setChecked(false);
        writeDelayComboBox->setEnabled(false);
        writeCustomDelayCheckBox->setEnabled(false);
        writeLineEdit->setEnabled(false);
    }

    changedWriteDelayTime();
}

void SAKReadWriteSetting::changedReadDelayTime()
{
    int time = 0;
    if (readDelayEnableCheckBox->isChecked()){
        if (readCustomDelayCheckBox->isChecked()){
            time = readLineEdit->text().toInt();
        }else {
            time = readDelayComboBox->currentText().toInt();

        }
    }

    emit readDelayTimeChanged(time);
}

void SAKReadWriteSetting::changedWriteDelayTime()
{
    int time = 0;
    if (writeDelayEnableCheckBox->isChecked()){
        if (writeCustomDelayCheckBox->isChecked()){
            time = writeLineEdit->text().toInt();
        }else {
            time = writeDelayComboBox->currentText().toInt();
        }
    }

    emit writeDelayTimeChanged(time);
}
