/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */

#include "SAKDebugPage.hh"
#include "ReadWriteSettingsWidget.hh"
#include "ui_ReadWriteSettingsWidget.h"

#include <QDebug>
#include <QIntValidator>
#include <QRegExpValidator>

ReadWriteSettingsWidget::ReadWriteSettingsWidget(SAKDebugPage *debugPage, QWidget* parent)
    :QDialog (parent)
    ,ui(new Ui::ReadWriteSettingsWidget)
    ,_debugPage (debugPage)
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

    connect(readDelayEnableCheckBox, &QCheckBox::clicked, this, &ReadWriteSettingsWidget::setEnableReadDelay);
    connect(writeDelayEnableCheckBox, &QCheckBox::clicked, this, &ReadWriteSettingsWidget::setEnableWriteDelay);

    connect(readCustomDelayCheckBox, &QCheckBox::clicked, this, &ReadWriteSettingsWidget::changedReadDelayTime);
    connect(writeCustomDelayCheckBox, &QCheckBox::clicked, this, &ReadWriteSettingsWidget::changedWriteDelayTime);

    connect(readDelayComboBox, &QComboBox::currentTextChanged, this, &ReadWriteSettingsWidget::changedReadDelayTime);
    connect(writeDelayComboBox, &QComboBox::currentTextChanged, this, &ReadWriteSettingsWidget::changedWriteDelayTime);

    connect(readLineEdit, &QLineEdit::textChanged, this, &ReadWriteSettingsWidget::changedReadDelayTime);
    connect(writeLineEdit, &QLineEdit::textChanged, this, &ReadWriteSettingsWidget::changedWriteDelayTime);

    initUi();
}

void ReadWriteSettingsWidget::initUi()
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

void ReadWriteSettingsWidget::setEnableReadDelay()
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

void ReadWriteSettingsWidget::setEnableWriteDelay()
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

void ReadWriteSettingsWidget::changedReadDelayTime()
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

void ReadWriteSettingsWidget::changedWriteDelayTime()
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
