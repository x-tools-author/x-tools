/***************************************************************************************************
 * Copyright 2020-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saknumberassistant.h"
#include "ui_saknumberassistant.h"

#include "sakinterface.h"

SAKNumberAssistant::SAKNumberAssistant(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::SAKNumberAssistant)
{
    ui->setupUi(this);
    m_interface = new SAKInterface(this);
    m_interface->setLineEditValidator(ui->rawDataLineEdit, SAKInterface::ValidatorFloat);

    connect(ui->hexRawDataCheckBox,
            &QCheckBox::clicked,
            this,
            &SAKNumberAssistant::onHexRawDataCheckBoxClicked);
    connect(ui->createPushButton,
            &QPushButton::clicked,
            this,
            &SAKNumberAssistant::onCreatePushButtonClicked);
    connect(ui->rawDataLineEdit,
            &QLineEdit::textChanged,
            this,
            &SAKNumberAssistant::onRawDataLineEditTextChanged);
    connect(ui->bigEndianCheckBox,
            &QCheckBox::clicked,
            this,
            &SAKNumberAssistant::onBigEndianCheckBoxClicked);
    connect(ui->floatRadioButton,
            &QRadioButton::clicked,
            this,
            &SAKNumberAssistant::onFloatRadioButtonClicked);
    connect(ui->doubleRadioButton,
            &QRadioButton::clicked,
            this,
            &SAKNumberAssistant::onDoubleRadioButtonClicked);

    onCreatePushButtonClicked();
}

SAKNumberAssistant::~SAKNumberAssistant()
{
    delete ui;
}

void SAKNumberAssistant::fixedLength(QStringList& stringList)
{
    if (ui->bigEndianCheckBox->isChecked()) {
        if (ui->floatRadioButton->isChecked()) {
            if (stringList.length() < int(sizeof(float))) {
                int len = int(sizeof(float)) - stringList.length();
                for (int i = 0; i < len; i++) {
                    stringList.prepend(QString(" 00"));
                }
            }
        } else {
            if (stringList.length() < int(sizeof(double))) {
                int len = int(sizeof(double)) - stringList.length();
                for (int i = 0; i < len; i++) {
                    stringList.prepend(QString(" 00"));
                }
            }
        }
    } else {
        if (ui->floatRadioButton->isChecked()) {
            if (stringList.length() < int(sizeof(float))) {
                int len = int(sizeof(float)) - stringList.length();
                for (int i = 0; i < len; i++) {
                    stringList.append(QString(" 00"));
                }
            }
        } else {
            if (stringList.length() < int(sizeof(double))) {
                int len = int(sizeof(double)) - stringList.length();
                for (int i = 0; i < len; i++) {
                    stringList.append(QString(" 00"));
                }
            }
        }
    }
}

void SAKNumberAssistant::onHexRawDataCheckBoxClicked()
{
    ui->rawDataLineEdit->clear();
    if (ui->hexRawDataCheckBox->isChecked()) {
        m_interface->setLineEditValidator(ui->rawDataLineEdit, SAKInterface::ValidatorHex);
    } else {
        m_interface->setLineEditValidator(ui->rawDataLineEdit, SAKInterface::ValidatorFloat);
    }
}

void SAKNumberAssistant::onCreatePushButtonClicked()
{
    if (ui->hexRawDataCheckBox->isChecked()) {
        ui->rawDataLineEdit->setMaxLength(ui->floatRadioButton->isChecked() ? 11 : 23);
        QString rawDataString = ui->rawDataLineEdit->text().trimmed();
        QStringList rawDataStringList = rawDataString.split(' ');
        fixedLength(rawDataStringList);

        QByteArray data;
        for (int i = 0; i < rawDataStringList.length(); i++) {
            bool isBigEndian = ui->bigEndianCheckBox->isChecked();
            quint8 value = quint8(
                rawDataStringList.at(isBigEndian ? i : rawDataStringList.length() - 1 - i).toInt());
            data.append(reinterpret_cast<char*>(&value), 1);
        }

        if (ui->floatRadioButton->isChecked()) {
            float* f = reinterpret_cast<float*>(data.data());
            ui->friendlyCookedDataLineEdit->setText(QString("%1").arg(*f));
            data = SAKInterface::arrayToHex(data, ' ');
            ui->hexCookedDataLineEdit->setText(QString(data));
        } else {
            double* d = reinterpret_cast<double*>(data.data());
            ui->friendlyCookedDataLineEdit->setText(QString("%1").arg(*d));
            data = SAKInterface::arrayToHex(data, ' ');
            ui->hexCookedDataLineEdit->setText(QString(data));
        }
    } else {
        QByteArray data;
        ui->rawDataLineEdit->setMaxLength(INT_MAX);
        if (ui->floatRadioButton->isChecked()) {
            float value = ui->rawDataLineEdit->text().trimmed().toFloat();
            if (ui->bigEndianCheckBox->isChecked()) {
                // To big endian
                float temp = value;
                quint8* ptr = reinterpret_cast<quint8*>(&value);
                for (int i = 0; i < int(sizeof(value)); i++) {
                    ptr[i] = reinterpret_cast<quint8*>(&temp)[int(sizeof(value)) - 1 - i];
                }
            }
            data.append(reinterpret_cast<char*>(&value), sizeof(value));
        } else {
            double value = ui->rawDataLineEdit->text().trimmed().toFloat();
            if (ui->bigEndianCheckBox->isChecked()) {
                // To big endian
                double temp = value;
                quint8* ptr = reinterpret_cast<quint8*>(&value);
                for (int i = 0; i < int(sizeof(value)); i++) {
                    ptr[i] = reinterpret_cast<quint8*>(&temp)[int(sizeof(value)) - 1 - i];
                }
            }
            data.append(reinterpret_cast<char*>(&value), sizeof(value));
        }
        ui->friendlyCookedDataLineEdit->setText(ui->rawDataLineEdit->text());
        data = SAKInterface::arrayToHex(data, ' ');
        ui->hexCookedDataLineEdit->setText(QString(data));
    }
}

void SAKNumberAssistant::onRawDataLineEditTextChanged(const QString& text)
{
    Q_UNUSED(text);
    onCreatePushButtonClicked();
}

void SAKNumberAssistant::onBigEndianCheckBoxClicked()
{
    onCreatePushButtonClicked();
}

void SAKNumberAssistant::onFloatRadioButtonClicked()
{
    onCreatePushButtonClicked();
}

void SAKNumberAssistant::onDoubleRadioButtonClicked()
{
    onCreatePushButtonClicked();
}
