/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QRectF>
#include <QPainter>
#include "SAKQRCode.hh"

SAKQRCode::SAKQRCode(QWidget *parent)
    :QWidget(parent)
{

}

SAKQRCode::~SAKQRCode()
{

}

void SAKQRCode::updateQRCode(QPixmap pixmap)
{
    innerPixmap = pixmap;
    update();
}

QPixmap SAKQRCode::qrCode()
{
    return innerPixmap;
}

void SAKQRCode::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (innerPixmap.isNull()){
        return;
    }

   int qrCodeLeftMargin = (width()-innerPixmap.width())/2;
   int qrCodeTopMargin = (height()-innerPixmap.height())/2;

   /// @brief 绘制二维码
   QPainter painter(this);
   QRect qrCodeRect = QRect(qrCodeLeftMargin, qrCodeTopMargin, innerPixmap.height(), innerPixmap.height());
   innerPixmap = innerPixmap.scaledToHeight(qrCodeRect.height());
   painter.drawPixmap(qrCodeRect, innerPixmap);
}
