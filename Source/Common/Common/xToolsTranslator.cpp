/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsTranslator.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QJsonDocument>

xToolsTranslator::xToolsTranslator(QObject* parent)
    : QObject{parent}
{
    m_flagNameMap.insert("zh_CN", "简体中文");
    m_flagNameMap.insert("en", "English");
#if 0
  mFlagNameMap.insert("zh_TW", "繁體中文");
  mFlagNameMap.insert("ar", "العربية");
  mFlagNameMap.insert("cs", "Čeština");
  mFlagNameMap.insert("da", "Dansk");
  mFlagNameMap.insert("de", "Deutsch");
  mFlagNameMap.insert("es", "Español");
  mFlagNameMap.insert("fa", "فارسی");
  mFlagNameMap.insert("fi", "Suomi");
  mFlagNameMap.insert("fr", "Français");
  mFlagNameMap.insert("he", "עִבְרִית");
  mFlagNameMap.insert("uk", "українська мова");
  mFlagNameMap.insert("it", "Italiano");
  mFlagNameMap.insert("ja", "日本语");
  mFlagNameMap.insert("ko", "한글");
  mFlagNameMap.insert("lt", "Lietuvių kalba");
  mFlagNameMap.insert("pl", "Polski");
  mFlagNameMap.insert("pt", "Português");
  mFlagNameMap.insert("ru", "русский язык");
  mFlagNameMap.insert("sk", "Slovenčina");
  mFlagNameMap.insert("sl", "Slovenščina");
  mFlagNameMap.insert("sv", "Svenska");
#endif
}

xToolsTranslator* xToolsTranslator::instance()
{
    static xToolsTranslator* translator = Q_NULLPTR;
    if (!translator) {
        translator = new xToolsTranslator(qApp);
    }

    return translator;
}

QStringList xToolsTranslator::languanges()
{
    return m_flagNameMap.values();
}

void xToolsTranslator::setupLanguage(const QString& language)
{
    QCoreApplication::removeTranslator(&m_translator);

    QString key = m_flagNameMap.key(language);
    if (language.isEmpty()) {
        qWarning() << "The language is not specified, system language will be used";
        key = QLocale::system().name();
    }

    if (key.isEmpty() || !m_flagNameMap.contains(key)) {
        qWarning() << "Unsupported language, english will be used";
        key = "en";
    }

    QString fileName = ":/Resources/Translations/app_" + key + ".qm";
    if (m_translator.load(fileName)) {
        QCoreApplication::installTranslator(&m_translator);
        qInfo() << m_flagNameMap.value(key) << " has been setup!";
        emit languageChanged();
    } else {
        qWarning() << "Load file failed: " << fileName;
    }
}
