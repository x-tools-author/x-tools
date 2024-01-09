/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakbase64assistant.h"
#include "ui_sakbase64assistant.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>

SAKBase64Assisatnt::SAKBase64Assisatnt(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::SAKBase64Assisatnt)
{
    ui->setupUi(this);
    connect(ui->image, &QPushButton::clicked, this, &SAKBase64Assisatnt::onImageClicked);
    connect(ui->decrypt, &QPushButton::clicked, this, &SAKBase64Assisatnt::onDecryptClicked);
    connect(ui->encrypt, &QPushButton::clicked, this, &SAKBase64Assisatnt::onEncryptClicked);
}

SAKBase64Assisatnt::~SAKBase64Assisatnt()
{
    delete ui;
}

void SAKBase64Assisatnt::onImageClicked()
{
    QString cipherText = ui->cipherText->toPlainText();
    QByteArray base64 = cipherText.toUtf8();
    QByteArray bytes = QByteArray::fromBase64(base64);

    QPixmap pix;
    if (!pix.loadFromData(bytes)) {
        QMessageBox::warning(this, tr("Data error"), tr("Data can not convert image."));
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

void SAKBase64Assisatnt::onEncryptClicked()
{
    QString plainText = ui->plainText->toPlainText();
    QByteArray byteArray = plainText.toUtf8();
    QByteArray base64 = byteArray.toBase64();
    QString ciperText = QString::fromLatin1(base64);
    ui->cipherText->setPlainText(ciperText);
}

void SAKBase64Assisatnt::onDecryptClicked()
{
    QString cipherText = ui->cipherText->toPlainText();
    QByteArray base64 = cipherText.toUtf8();
    QByteArray byteArray = QByteArray::fromBase64(base64);
    QString plainText = QString::fromUtf8(byteArray);
    ui->plainText->setPlainText(plainText);
}
