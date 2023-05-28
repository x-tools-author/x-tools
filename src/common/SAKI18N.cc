/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <QFile>
#include <QJsonDocument>
#include <QCoreApplication>

#include "EDI18N.hpp"

EDI18N::EDI18N(QObject *parent)
    : QObject{parent}
{

}

EDI18N* EDI18N::instance()
{
    static EDI18N i18n;
    return &i18n;
}

void EDI18N::installTranslator(const QString &name)
{
    if (mCurrentName == name) {
        return;
    }

    mCurrentName = name;
    qCWarning(mCategory) << "Installing translator: " << name;
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

void EDI18N::setConfigurationFile(const QString &conf)
{
    mConf = conf;
    languanges();
}

void EDI18N::uninstallTranslator()
{
    while (!mTranslators.isEmpty()) {
        auto translator = mTranslators.takeFirst();
        if (QCoreApplication::removeTranslator(translator)) {
            qCInfo(mCategory) << "old translator removed: "
                              << translator->filePath();
            translator->deleteLater();
            translator = nullptr;
        } else {
            qCInfo(mCategory) << "remove old translator failed!";
        }
    }
}

void EDI18N::installTranslator(const QJsonArray &packets)
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
                qCInfo(mCategory) << "translator installed: " << qmFile;
            } else {
                qCWarning(mCategory) << "install translator failed: "
                                     << qmFile;
            }
        } else {
            qCWarning(mCategory) << "Can not load: " << qmFile;
        }
    }
}

QVariantList EDI18N::languanges()
{
    QVariantList list;

    QFile file(mConf);
    if (file.open(QFile::ReadOnly)) {
        auto json = file.readAll();
        file.close();

        auto jsonDoc = QJsonDocument::fromJson(json);
        auto array = jsonDoc.array();
        for (int i = 0; i < array.count(); i++) {
            auto object = array.at(i).toObject();
            list.append(object);
            if (mLanguageContextList.count() != array.count()) {
                mLanguageContextList.append(object);
            }
        }
    } else {
        qDebug() << file.errorString();
    }

    return list;
}
