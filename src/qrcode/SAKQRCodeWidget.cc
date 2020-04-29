/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QRectF>
#include <QPainter>
#include "SAKQRCodeWidget.hh"

SAKQRCodeWidget::SAKQRCodeWidget(QSize size, QString image, QWidget *parent)
    : QWidget(parent)
    , size(size)
    , image(image)
{
    resize(size.width(), size.height());
}

SAKQRCodeWidget::~SAKQRCodeWidget()
{

}

void SAKQRCodeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPixmap qrCode(image);
    qrCode = qrCode.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    int w = qrCode.width();
    int h = qrCode.height();

    QRectF target((width()-w)/2, (height()-h)/2, w, h);
    QRectF source(0, 0, w, h);

    QPainter painter(this);
    painter.drawPixmap(target, qrCode, source);
}
