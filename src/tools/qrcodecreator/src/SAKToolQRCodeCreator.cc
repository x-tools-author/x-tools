/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QFile>
#include <QDateTime>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QStandardPaths>

#include "SAKQRCode.hh"
#include "SAKToolQRCodeCreator.hh"

extern "C" {
    #include "qrencode.h"
}

#include "ui_SAKToolQRCodeCreator.h"

SAKToolQRCodeCreator::SAKToolQRCodeCreator(QWidget *parent)
    :SAKToolBase(parent)
    ,ui(new Ui::SAKToolQRCodeCreator)
{
    ui->setupUi(this);
    plainTextEdit = ui->plainTextEdit;
    savePushButton = ui->savePushButton;
    createPushButton = ui->createPushButton;

    qrCodeWidget = new SAKQRCode;
    qrCodeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QWidget *w = ui->qrCodeWidget;
    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(qrCodeWidget);
    w->setLayout(hBoxLayout);
}

SAKToolQRCodeCreator::~SAKToolQRCodeCreator()
{
    delete ui;
    delete qrCodeWidget;
}

QPixmap SAKToolQRCodeCreator::encodeString(const QString &text, int width)
{
    if (text.length() == 0){
        return QPixmap();
    }

    QRcode *qrcode = QRcode_encodeString(text.toLatin1().constData(), 0, QR_ECLEVEL_L, QR_MODE_8, 0);
    if (qrcode == nullptr) {
        return QPixmap();
    }

    unsigned char *p = nullptr;
    unsigned char *q = nullptr;
    int realwidth = qrcode->width;
    int x, y, bit;

    QImage image = QImage(realwidth, realwidth, QImage::Format_Indexed8);
    image.fill(qRgb(255, 255, 255));

    QRgb black = qRgb(255, 255, 255);
    image.setColor(0, black);
    QRgb white = qRgb(0, 0, 0);
    image.setColor(1, white);

    p = qrcode->data;
    for (y = 0; y < qrcode->width; y++) {
        bit = 7;
        for (x = 0; x< qrcode->width; x++) {
            if ((*p & 1) << bit){
                image.setPixel(x, y, 1);
            } else {
                image.setPixel(x, y, 0);
            }
            bit--;
            if (bit < 0){
                q++;
                bit = 7;
            }
            p++;
        }
    }

    QRcode_free(qrcode);
    return QPixmap::fromImage(image.scaledToWidth(width));
}

void SAKToolQRCodeCreator::on_savePushButton_clicked()
{
    QPixmap pix = qrCodeWidget->qrCode();
    if (pix.isNull()){
        return;
    }

    QString defaultFileName = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    defaultFileName.append(QString("/"));
    defaultFileName.append(QString::number(QDateTime::currentMSecsSinceEpoch()));
    QString fileName = QFileDialog::getSaveFileName(this,
                                                   tr("保存二维码"),
                                                   defaultFileName,
                                                   QString("JPG(*.jpg);;PNG(*.png)"));
    if (fileName.length()){
        QString format = fileName.split('.').last();
        pix.save(fileName, format.toLatin1().data());
    }
}

void SAKToolQRCodeCreator::on_createPushButton_clicked()
{
    QPixmap pix = encodeString(plainTextEdit->toPlainText(), 100);
    qrCodeWidget->updateQRCode(pix);
}
