/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "ui_xToolsQRCodeAssistant.h"
#include "xToolsQRCodeAssistant.h"

#include <qrencode.h>

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QTimer>

xToolsQRCodeAssistant::xToolsQRCodeAssistant(QWidget *parent)
    : QWidget{parent}
    , ui{new Ui::xToolsQRCodeAssistant}
{
    ui->setupUi(this);
    connect(ui->pushButtonGenerate,
            &QPushButton::clicked,
            this,
            &xToolsQRCodeAssistant::onGeneratePushButtonClicked);
    connect(ui->pushButtonExport,
            &QPushButton::clicked,
            this,
            &xToolsQRCodeAssistant::onExportPushButtonClicked);

    ui->lineEditData->setText(QString("Hello, xTools!"));
    QTimer::singleShot(1000, this, &xToolsQRCodeAssistant::onGeneratePushButtonClicked);
}

xToolsQRCodeAssistant::~xToolsQRCodeAssistant()
{
    delete ui;
}

void xToolsQRCodeAssistant::onGeneratePushButtonClicked()
{
    QString txt = ui->lineEditData->text();
    if (txt.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please input the data to generate QR code."));
        ui->lineEditData->setFocus();
        return;
    }

    QByteArray array = txt.toLocal8Bit();
    array.append('\0'); // Add a null terminator to the end of the string.
    QRcode *qrCode = QRcode_encodeString(array.data(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    if (!qrCode) {
        qWarning() << "The QR code is not generated, please check parameters then try again.";
        return;
    }

    QPixmap tmpPixMap = QPixmap(qrCode->width, qrCode->width);
    QPainter painter(&tmpPixMap);
    QColor background(Qt::white);
    painter.setBrush(background);
    painter.setPen(Qt::NoPen);
    painter.drawRect(tmpPixMap.rect());
    QColor foreground(Qt::black);
    painter.setBrush(foreground);
    for (int y = 0; y < qrCode->width; y++) {
        for (int x = 0; x < qrCode->width; x++) {
            unsigned char b = qrCode->data[y * qrCode->width + x];
            if (b & 0x01) {
                QRectF r(x, y, 1, 1);
                painter.drawRects(&r, 1);
            }
        }
    }
    QRcode_free(qrCode);

    m_pixMap = tmpPixMap.scaled(ui->labelImage->width(),
                                ui->labelImage->height(),
                                Qt::KeepAspectRatio);

    ui->labelImage->setPixmap(m_pixMap);
    ui->labelImage->setAlignment(Qt::AlignCenter);
}

void xToolsQRCodeAssistant::onExportPushButtonClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save QR Code Image"),
                                                    QDir::homePath(),
                                                    tr("PNG Images (*.png);;JPG Images (*.jpg)"));
    if (fileName.isEmpty()) {
        return;
    }

    m_pixMap.save(fileName);
    qInfo() << "The QR code image is saved to" << fileName;
}