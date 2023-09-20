/*******************************************************************************
 * Copyright 2020-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 ******************************************************************************/
#include "saknumberassistant.h"

#include "SAKCommonInterface.h"
#include "SAKInterface.h"
#include "ui_saknumberassistant.h"

SAKNumberAssistant::SAKNumberAssistant(QWidget* parent)
    : QWidget(parent), ui_(new Ui::SAKNumberAssistant) {
  ui_->setupUi(this);
  common_interface_ = new SAKCommonInterface(this);
  common_interface_->setLineEditValidator(ui_->rawDataLineEdit,
                                          SAKCommonInterface::ValidatorFloat);

  connect(ui_->hexRawDataCheckBox, &QCheckBox::clicked, this,
          &SAKNumberAssistant::OnHexRawDataCheckBoxClicked);
  connect(ui_->createPushButton, &QPushButton::clicked, this,
          &SAKNumberAssistant::OnCreatePushButtonClicked);
  connect(ui_->rawDataLineEdit, &QLineEdit::textChanged, this,
          &SAKNumberAssistant::OnRawDataLineEditTextChanged);
  connect(ui_->bigEndianCheckBox, &QCheckBox::clicked, this,
          &SAKNumberAssistant::OnBigEndianCheckBoxClicked);
  connect(ui_->floatRadioButton, &QRadioButton::clicked, this,
          &SAKNumberAssistant::OnFloatRadioButtonClicked);
  connect(ui_->doubleRadioButton, &QRadioButton::clicked, this,
          &SAKNumberAssistant::OnDoubleRadioButtonClicked);

  OnCreatePushButtonClicked();
}

SAKNumberAssistant::~SAKNumberAssistant() { delete ui_; }

void SAKNumberAssistant::FixedLength(QStringList& stringList) {
  if (ui_->bigEndianCheckBox->isChecked()) {
    if (ui_->floatRadioButton->isChecked()) {
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
    if (ui_->floatRadioButton->isChecked()) {
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

void SAKNumberAssistant::OnHexRawDataCheckBoxClicked() {
  ui_->rawDataLineEdit->clear();
  if (ui_->hexRawDataCheckBox->isChecked()) {
    common_interface_->setLineEditValidator(ui_->rawDataLineEdit,
                                            SAKCommonInterface::ValidatorHex);
  } else {
    common_interface_->setLineEditValidator(ui_->rawDataLineEdit,
                                            SAKCommonInterface::ValidatorFloat);
  }
}

void SAKNumberAssistant::OnCreatePushButtonClicked() {
  if (ui_->hexRawDataCheckBox->isChecked()) {
    ui_->rawDataLineEdit->setMaxLength(ui_->floatRadioButton->isChecked() ? 11
                                                                          : 23);
    QString rawDataString = ui_->rawDataLineEdit->text().trimmed();
    QStringList rawDataStringList = rawDataString.split(' ');
    FixedLength(rawDataStringList);

    QByteArray data;
    for (int i = 0; i < rawDataStringList.length(); i++) {
      bool isBigEndian = ui_->bigEndianCheckBox->isChecked();
      quint8 value =
          quint8(rawDataStringList
                     .at(isBigEndian ? i : rawDataStringList.length() - 1 - i)
                     .toInt());
      data.append(reinterpret_cast<char*>(&value), 1);
    }

    if (ui_->floatRadioButton->isChecked()) {
      float* f = reinterpret_cast<float*>(data.data());
      ui_->friendlyCookedDataLineEdit->setText(QString("%1").arg(*f));
      data = SAKInterface::arrayToHex(data, ' ');
      ui_->hexCookedDataLineEdit->setText(QString(data));
    } else {
      double* d = reinterpret_cast<double*>(data.data());
      ui_->friendlyCookedDataLineEdit->setText(QString("%1").arg(*d));
      data = SAKInterface::arrayToHex(data, ' ');
      ui_->hexCookedDataLineEdit->setText(QString(data));
    }
  } else {
    QByteArray data;
    ui_->rawDataLineEdit->setMaxLength(INT_MAX);
    if (ui_->floatRadioButton->isChecked()) {
      float value = ui_->rawDataLineEdit->text().trimmed().toFloat();
      if (ui_->bigEndianCheckBox->isChecked()) {
        // To big endian
        float temp = value;
        quint8* ptr = reinterpret_cast<quint8*>(&value);
        for (int i = 0; i < int(sizeof(value)); i++) {
          ptr[i] = reinterpret_cast<quint8*>(&temp)[int(sizeof(value)) - 1 - i];
        }
      }
      data.append(reinterpret_cast<char*>(&value), sizeof(value));
    } else {
      double value = ui_->rawDataLineEdit->text().trimmed().toFloat();
      if (ui_->bigEndianCheckBox->isChecked()) {
        // To big endian
        double temp = value;
        quint8* ptr = reinterpret_cast<quint8*>(&value);
        for (int i = 0; i < int(sizeof(value)); i++) {
          ptr[i] = reinterpret_cast<quint8*>(&temp)[int(sizeof(value)) - 1 - i];
        }
      }
      data.append(reinterpret_cast<char*>(&value), sizeof(value));
    }
    ui_->friendlyCookedDataLineEdit->setText(ui_->rawDataLineEdit->text());
    data = SAKInterface::arrayToHex(data, ' ');
    ui_->hexCookedDataLineEdit->setText(QString(data));
  }
}

void SAKNumberAssistant::OnRawDataLineEditTextChanged(const QString& text) {
  Q_UNUSED(text);
  OnCreatePushButtonClicked();
}

void SAKNumberAssistant::OnBigEndianCheckBoxClicked() {
  OnCreatePushButtonClicked();
}

void SAKNumberAssistant::OnFloatRadioButtonClicked() {
  OnCreatePushButtonClicked();
}

void SAKNumberAssistant::OnDoubleRadioButtonClicked() {
  OnCreatePushButtonClicked();
}
