/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QIconEngine>

#if 0 // QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
#define xIcon(file) QIcon(file)
#else
#define xIcon(file) QIcon(new xTools::IconEngine(file))
#endif

#define xColorIcon(file, color) QIcon(new xTools::IconEngine(file, color))

namespace xTools {

class IconEngine : public QIconEngine
{
public:
    IconEngine(const QString &iconFile);
    IconEngine(const QString &iconFile, const QString &color);
    ~IconEngine();

    QIconEngine *clone() const override;
    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) override;

private:
    QString m_iconFile;
    QString m_color;
};

} // namespace xTools