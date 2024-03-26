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
    xToolsApplication::setupLanguage(language);

    // Palette
    showSplashScreenMessage(tr("Load palette..."));
    QString palette = xToolsSettings::instance()->palette();
    setupPalette(palette);

    // Splash screen
    m_splashScreen.setPixmap(splashScreenPixmap());
    m_splashScreen.show();
    processEvents();
    showSplashScreenMessage(tr("Initialize application..."));
}

void xToolsApplication::showSplashScreenMessage(const QString &msg)
{
    m_splashScreen.showMessage(msg, Qt::AlignBottom, QColor(255, 255, 255));
}

void xToolsApplication::setupPalette(const QString &fileName)
{
    if (fileName.isEmpty()) {
        qWarning() << "The palette file name is empty, use default palette.";
        return;
    }

    if (!QFile::exists(fileName)) {
        QString info = QString("The palette file not found: %1, use default palette.").arg(fileName);
        qInfo() << qPrintable(info);
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QDataStream out(&file);
        QPalette palette;
        out >> palette;
        file.close();
        setPalette(palette);
        qInfo() << "The palette of application is:" << fileName;
    } else {
        qWarning() << "Open palette file error:" << file.errorString();
    }
}

QSplashScreen &xToolsApplication::splashScreen()
{
    return m_splashScreen;
}

QStringList xToolsApplication::supportedLanguages()
{
    return m_languageFlagNameMap.values();
}

QString xToolsApplication::language()
{
    QString language = xToolsSettings::instance()->language();
    if (language.isEmpty()) {
        language = QLocale::system().name();
    }

    if (!m_languageFlagNameMap.contains(language)) {
        language = "en";
    }

    return m_languageFlagNameMap.value(language);
}

void xToolsApplication::setupLanguage(const QString &language)
{
    setupLanguageWithPrefix(language, m_translatorPrefix);
}

void xToolsApplication::setupLanguageWithPrefix(const QString &language, const QString &prefix)
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
            QString info = QString("The translator(%1) has been installed,").arg(fileName);
            qInfo() << qPrintable(info)
                    << "current language is:" << m_languageFlagNameMap.value(key);
            emit languageChanged();
        } else {
            QString info = QString("Install translator failed, the file is: \"%1\".").arg(fileName);
            qInfo() << qPrintable(info) << "Maybe the qm file if empty!";
            translator->deleteLater();
            translator = Q_NULLPTR;
        }
    } else {
        qWarning() << "Load file failed: " << fileName;
    }
}


QPixmap xToolsApplication::splashScreenPixmap()
{
    QPixmap pixmap(600, 260);
    pixmap.fill(QColor(0x2d2d30));

    QPainter painter(&pixmap);
    painter.setPen(QColor(Qt::white));
    auto font = painter.font();
    font.setPixelSize(52);
    painter.setFont(font);
    painter.drawText(pixmap.rect(),
                     Qt::AlignHCenter | Qt::AlignVCenter,
                     xToolsApplication::applicationName());
    painter.drawRect(pixmap.rect() - QMargins(1, 1, 1, 1));

    return pixmap;
}
