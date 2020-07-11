/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
