/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QJsonDocument>
#include <QCoreApplication>

#include "SAKTranslator.hh"

SAKTranslator::SAKTranslator(QObject *parent)
    : QObject{parent}
{
    mFlagNameMap.insert("zh_CN", "简体中文");
    mFlagNameMap.insert("zh_TW", "繁體中文");

    mFlagNameMap.insert("ar", "العربية");
    mFlagNameMap.insert("cs", "Čeština");
    mFlagNameMap.insert("da", "Dansk");
    mFlagNameMap.insert("de", "Deutsch");
    mFlagNameMap.insert("en", "English");
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
}

SAKTranslator* SAKTranslator::instance()
{
    static SAKTranslator translator;
    return &translator;
}

void SAKTranslator::installTranslator(const QString &name)
{
    if (mCurrentName == name) {
        return;
    }

    mCurrentName = name;
    qCWarning(mLoggingCategory) << "Installing translator: " << name;
    for (auto &ctx : mLanguageContextList) {
        QString languageName = ctx.value("name").toString();
        if (name == languageName) {
            uninstallTranslator();
            QJsonArray packets = ctx.value("packets").toArray();
            installTranslator(packets);

            emit languageChanged();
        }
    }
}

void SAKTranslator::setConfigurationFile(const QString &conf)
{
    mConf = conf;
    languanges();
}

void SAKTranslator::uninstallTranslator()
{
    while (!mTranslators.isEmpty()) {
        auto translator = mTranslators.takeFirst();
        if (QCoreApplication::removeTranslator(translator)) {
            qCInfo(mLoggingCategory) << "old translator removed: "
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
                              << translator->filePath();
#else
                ;
#endif
            translator->deleteLater();
            translator = nullptr;
        } else {
            qCInfo(mLoggingCategory) << "remove old translator failed!";
        }
    }
}

void SAKTranslator::installTranslator(const QJsonArray &packets)
{
    for (int i = 0; i < packets.count(); i++) {
        QString qmFile = ":/res/i18n/";
        QString packet = packets.at(i).toString();
        qmFile.append(packet);
        qmFile.append("_");
        qmFile.append(mCurrentName);
        qmFile.append(".qm");

        QTranslator *translator = new QTranslator();
        mTranslators.append(translator);
        if (translator->load(qmFile)) {
            if (QCoreApplication::installTranslator(translator)) {
                qCInfo(mLoggingCategory) << "translator installed: " << qmFile;
            } else {
                qCWarning(mLoggingCategory) << "install translator failed: "
                                     << qmFile;
            }
        } else {
            qCWarning(mLoggingCategory) << "Can not load: " << qmFile;
        }
    }
}

QStringList SAKTranslator::languanges()
{
    QString path = QCoreApplication::applicationDirPath();
    path += "/translations";
    QDir dir(path);
    QFileInfoList fileInfoList = dir.entryInfoList("QM (*.qm)");

    QStringList list;
    list << "简体中文";
    return list;
}
