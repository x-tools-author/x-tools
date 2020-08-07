/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
