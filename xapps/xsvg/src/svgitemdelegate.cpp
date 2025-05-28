/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SvgItemDelegate.h"

#include <QFile>
#include <QPainter>
#include <QPushButton>
#include <QSvgRenderer>

QString SvgItemDelegate::s_color;
int SvgItemDelegate::s_size{16};
SvgItemDelegate::SvgItemDelegate(QObject *parent)
    : QItemDelegate{parent}
{}

void SvgItemDelegate::paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
#if 0
    QItemDelegate::paint(painter, option, index);
#endif

    const QString svgFile = index.data().toString();
    if (!QFile::exists(svgFile)) {
        return;
    }

    if (!s_color.isEmpty()) {
#if 1
        QSvgRenderer renderer(svgFile);
        QImage image(QSize(s_size, s_size), QImage::Format_ARGB32);
        image.fill(Qt::transparent); // Transparent background

        QPainter tmpPainter(&image);
        tmpPainter.setRenderHint(QPainter::Antialiasing);
        renderer.render(&tmpPainter);

        // Change color
        for (int y = 0; y < image.height(); y++) {
            for (int x = 0; x < image.width(); x++) {
                QColor ic = image.pixelColor(x, y);
                if (ic.alpha() > 0) { // If not transparent
                    image.setPixelColor(x, y, s_color);
                }
            }
        }

        painter->drawPixmap(option.rect, QPixmap::fromImage(image));
#else // TODO: This code does not work
        QSvgRenderer renderer(svgFile);
        QPixmap pixmap(s_size, s_size);
        pixmap.fill(Qt::transparent);
        QPainter tmpPainter(&pixmap);
        tmpPainter.setRenderHint(QPainter::Antialiasing);
        tmpPainter.setPen(QPen(QColor(s_color), 1));
        renderer.render(&tmpPainter);
        tmpPainter.end();
        painter->drawPixmap(option.rect, pixmap);
#endif
    } else {
        const QPixmap pixmap{svgFile};
        if (pixmap.isNull()) {
            return;
        }

        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter->drawPixmap(option.rect, pixmap);
    }
}
