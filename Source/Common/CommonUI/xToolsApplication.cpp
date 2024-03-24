/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsApplication.h"

#include <QDataStream>
#include <QFile>
#include <QPainter>

#include "xToolsSettings.h"

xToolsApplication::xToolsApplication(int argc, char *argv[])
    : QApplication(argc, argv)
{
    // Splash screen
    m_splashScreen.setPixmap(splashScreenPixmap());
    m_splashScreen.show();
    processEvents();
    showSplashScreenMessage(tr("Initialize application..."));

    // Palette
    showSplashScreenMessage(tr("Load palette..."));
    int ret = xToolsSettings::instance()->palette();
    if ((ret == DarkPalette) || (ret == LightPalette)) {
        setupPalette(ret);
    } else {
        QString customPalette = xToolsSettings::instance()->customPalette();
        if (!customPalette.isEmpty()) {
            setupPalette(customPalette);
        } else {
            qInfo() << "Current palette is not specified, use default palette.";
        }
    }
}

void xToolsApplication::showSplashScreenMessage(const QString &msg)
{
    m_splashScreen.showMessage(msg, Qt::AlignBottom, QColor(255, 255, 255));
}

void xToolsApplication::setupPalette(int palette)
{
    if (palette == DarkPalette) {
        setupPalette(QString(":/Resources/Palettes/DarkPalette"));
    } else if (palette == LightPalette) {
        setupPalette(QString(":/Resources/Palettes/LightPalette"));
    } else {
        qInfo() << "Current palette is not specified, use default palette.";
    }
}

void xToolsApplication::setupPalette(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QDataStream out(&file);
        QPalette p;
        out >> p;
        file.close();
        setPalette(p);
        qInfo() << "Current palette is:" << fileName;
    } else {
        qWarning() << "Open palette file error:" << file.errorString();
    }
}

void xToolsApplication::setupLanguage(const QString &language) {}

QPixmap xToolsApplication::splashScreenPixmap()
{
    QPixmap pixmap(400, 260);
    pixmap.fill(QColor(0x2d2d30));

    QPainter painter(&pixmap);
    painter.setPen(QColor(Qt::white));
    auto font = painter.font();
    font.setPixelSize(48);
    painter.setFont(font);
    painter.drawText(pixmap.rect(),
                     Qt::AlignHCenter | Qt::AlignVCenter,
                     xToolsApplication::applicationName());

    return pixmap;
}
