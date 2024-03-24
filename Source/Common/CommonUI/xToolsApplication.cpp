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
#include <QTranslator>

#include "xToolsSettings.h"

xToolsApplication::xToolsApplication(int argc, char *argv[])
    : QApplication(argc, argv)
{
    m_languageFlagNameMap.insert("zh_CN", "简体中文");
    m_languageFlagNameMap.insert("en", "English");
#if 0
  m_languageFlagNameMap.insert("zh_TW", "繁體中文");
  m_languageFlagNameMap.insert("ar", "العربية");
  m_languageFlagNameMap.insert("cs", "Čeština");
  m_languageFlagNameMap.insert("da", "Dansk");
  m_languageFlagNameMap.insert("de", "Deutsch");
  m_languageFlagNameMap.insert("es", "Español");
  m_languageFlagNameMap.insert("fa", "فارسی");
  m_languageFlagNameMap.insert("fi", "Suomi");
  m_languageFlagNameMap.insert("fr", "Français");
  m_languageFlagNameMap.insert("he", "עִבְרִית");
  m_languageFlagNameMap.insert("uk", "українська мова");
  m_languageFlagNameMap.insert("it", "Italiano");
  m_languageFlagNameMap.insert("ja", "日本语");
  m_languageFlagNameMap.insert("ko", "한글");
  m_languageFlagNameMap.insert("lt", "Lietuvių kalba");
  m_languageFlagNameMap.insert("pl", "Polski");
  m_languageFlagNameMap.insert("pt", "Português");
  m_languageFlagNameMap.insert("ru", "русский язык");
  m_languageFlagNameMap.insert("sk", "Slovenčina");
  m_languageFlagNameMap.insert("sl", "Slovenščina");
  m_languageFlagNameMap.insert("sv", "Svenska");
#endif
    QString language = xToolsSettings::instance()->language();
    setupLanguage(language, "xToolsCommon");

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
            qInfo() << "The palette is not specified, use default palette.";
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
        qInfo() << "The palette is not specified, use default palette.";
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

void xToolsApplication::setupLanguage(const QString &language, const QString &prefix)
{
    QString key = m_languageFlagNameMap.key(language);
    if (language.isEmpty()) {
        key = QLocale::system().name();
        qWarning() << "The language is not specified, system language will be used:" << key;
    }

    if (!m_languageFlagNameMap.contains(key)) {
        qWarning() << "Unsupported language, english will be used";
        key = "en";
    }

    static QMap<QString, QTranslator *> fileTranslatorMap;
    if (!fileTranslatorMap.contains(prefix)) {
        QTranslator *translator = new QTranslator();
        fileTranslatorMap.insert(prefix, translator);
    } else {
        if (!QCoreApplication::removeTranslator(fileTranslatorMap.value(prefix))) {
            qWarning() << "Remove translator failed:" << prefix;
        }
    }

    QString fileName = QString(":/Resources/Translations/%1_%2.qm").arg(prefix, key);
    QTranslator *translator = fileTranslatorMap.value(prefix);
    if (translator->load(fileName)) {
        if (QCoreApplication::installTranslator(translator)) {
            qInfo() << m_languageFlagNameMap.value(key) << " has been setup!";
            emit languageChanged();
        } else {
            qInfo() << "Install translator failed, the file is:" << fileName;
            translator->deleteLater();
            translator = Q_NULLPTR;
        }
    } else {
        qWarning() << "Load file failed: " << fileName;
    }
}

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
