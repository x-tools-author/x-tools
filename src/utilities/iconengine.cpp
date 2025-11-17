/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "iconengine.h"

#include <QDebug>
#include <QGuiApplication>
#include <QPainter>
#include <QPalette>

namespace xTools {

IconEngine::IconEngine(const QString &iconFile)
    : QIconEngine()
    , m_iconFile(iconFile)
{}

IconEngine::IconEngine(const QString &iconFile, const QString &color)
    : QIconEngine()
    , m_iconFile(iconFile)
    , m_color(color)
{}

IconEngine::~IconEngine() {}

QIconEngine *IconEngine::clone() const
{
    return new IconEngine(*this);
}

void IconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state)
{
    if (!painter) {
        return;
    }

    QPalette palette = qApp->palette();
    QColor iconColor = palette.color(QPalette::ButtonText);
    if (mode == QIcon::Disabled) {
        iconColor.setAlpha(100);
    } else if (mode == QIcon::Selected) {
        iconColor = palette.color(QPalette::Highlight);
    }

    if (!m_color.isEmpty()) {
        iconColor = QColor(m_color);
    }

    static QString lastIconFile;
    static QColor lastColor;
    static QPixmap cachedPixmap;

    if (m_iconFile != lastIconFile || iconColor != lastColor || cachedPixmap.isNull()) {
        QPixmap pixmap(m_iconFile);
        if (pixmap.isNull())
            return;

        QPixmap coloredPixmap(pixmap.size());
        coloredPixmap.fill(Qt::transparent);

        QPainter pixmapPainter(&coloredPixmap);
        pixmapPainter.drawPixmap(0, 0, pixmap);
        pixmapPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        pixmapPainter.fillRect(coloredPixmap.rect(), iconColor);
        pixmapPainter.end();

        cachedPixmap = coloredPixmap;
        lastIconFile = m_iconFile;
        lastColor = iconColor;
    }

    auto tmp = cachedPixmap.scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter->drawPixmap(rect, tmp);
}

QPixmap IconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)
{
    QImage img(size, QImage::Format_ARGB32);
    img.fill(qRgba(0, 0, 0, 0));
    QPixmap pix = QPixmap::fromImage(img, Qt::NoFormatConversion);
    {
        QPainter painter(&pix);
        QRect r(QPoint(0.0, 0.0), size);
        this->paint(&painter, r, mode, state);
    }
    return pix;
}

} // namespace xTools