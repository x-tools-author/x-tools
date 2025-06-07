/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xcode.h"
#include "ui_xcode.h"

#include <backend_qt/qzint.h>

#include <QFileDialog>
#include <QMessageBox>

xCode::xCode(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::xCode)
{
    ui->setupUi(this);
    setWindowTitle(tr("xCode - Barcode/QR code Generator"));
    connect(ui->pushButtonExport, &QPushButton::clicked, this, &xCode::onExportButtonClicked);
    connect(ui->pushButtonRefresh, &QPushButton::clicked, this, &xCode::onRefreshButtonClicked);
}

xCode::~xCode()
{
    delete ui;
}

void xCode::onExportButtonClicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save Image"), "", tr("PNG Image (*.png);;JPEG Image (*.jpg);;BMP Image (*.bmp)"));
    if (fileName.isEmpty()) {
        return;
    }

    QPixmap pixmap = ui->labelImage->pixmap(Qt::ReturnByValue);
    if (!pixmap.save(fileName)) {
        QMessageBox::warning(this, tr("Save Image"), tr("Failed to save image."));
    }
}

void xCode::onRefreshButtonClicked()
{
    Zint::QZint code;
    Zint::QZint::AspectRatioMode mode = Zint::QZint::KeepAspectRatio;
    code.setSymbol(BARCODE_MICROQR);
    code.setSecurityLevel(0);
    code.setText(ui->lineEditDetail->text());
    QImage image(ui->labelImage->size(), QImage::Format_ARGB32);
    QPainter painter(&image);
    code.render(painter, image.rect(), mode);
    ui->labelImage->setPixmap(QPixmap::fromImage(image));
}