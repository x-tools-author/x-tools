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
