/*******************************************************************************
 * Copyright 2023 Qter(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/

#include "sakbase64assistant.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>

#include "ui_sakbase64assistant.h"

SAKBase64Assisatnt::SAKBase64Assisatnt(QWidget* parent)
    : QDialog(parent), ui_(new Ui::SAKBase64Assisatnt) {
  ui_->setupUi(this);
  connect(ui_->image_, &QPushButton::clicked, this,
          &SAKBase64Assisatnt::OnImageClicked);
  connect(ui_->decrypt_, &QPushButton::clicked, this,
          &SAKBase64Assisatnt::OnDecryptClicked);
  connect(ui_->encrypt_, &QPushButton::clicked, this,
          &SAKBase64Assisatnt::OnEncryptClicked);
}

SAKBase64Assisatnt::~SAKBase64Assisatnt() {
  delete ui_;
}

void SAKBase64Assisatnt::OnImageClicked() {
  QString cipher_text = ui_->cipher_text_->toPlainText();
  QByteArray base64 = cipher_text.toUtf8();
  QByteArray bytes = QByteArray::fromBase64(base64);

  QPixmap pix;
  if (!pix.loadFromData(bytes)) {
    QMessageBox::warning(this, tr("Data error"),
                         tr("Data can not convert image."));
    return;
  }

  QLabel* label = new QLabel(this);
  label->resize(pix.size());
  label->setPixmap(pix);

  QDialog dialog(this);
  dialog.setLayout(new QHBoxLayout());
  dialog.layout()->addWidget(label);
  dialog.setModal(true);
  dialog.exec();
}

void SAKBase64Assisatnt::OnEncryptClicked() {
  QString plain_text = ui_->plain_text_->toPlainText();
  QByteArray byte_array = plain_text.toUtf8();
  QByteArray base64 = byte_array.toBase64();
  QString ciper_text = QString::fromLatin1(base64);
  ui_->cipher_text_->setPlainText(ciper_text);
}

void SAKBase64Assisatnt::OnDecryptClicked() {
  QString cipher_text = ui_->cipher_text_->toPlainText();
  QByteArray base64 = cipher_text.toUtf8();
  QByteArray byte_array = QByteArray::fromBase64(base64);
  QString plain_text = QString::fromUtf8(byte_array);
  ui_->plain_text_->setPlainText(plain_text);
}
