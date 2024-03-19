/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakfontawesome.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QFontDatabase>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

SAKFontAwesome::SAKFontAwesome(QObject *parent)
    : QObject{parent}
{
    QFile file(fontAwesomePath() + "/metadata/icon-families.json");
    if (file.open(QFile::ReadOnly)) {
        QByteArray all = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(all);
        QJsonObject obj = doc.object();

        QStringList keys = obj.keys();
        SAKFontAwesomeIconContext icon;
        for (auto &key : keys) {
            QJsonObject iconCtx = obj.value(key).toObject();
            icon.terms.clear();
            icon.unicode = iconCtx.value("unicode").toString().toInt(nullptr, 16);
            icon.label = iconCtx.value("label").toString();

            QJsonObject search = iconCtx.value("search").toObject();
            QJsonArray terms = search.value("terms").toArray();
            for (int i = 0; i < terms.count(); i++) {
                icon.terms.append(terms.at(i).toString().toUpper());
            }

            QJsonObject svgs = iconCtx.value("svgs").toObject();
            QJsonObject classic = svgs.value("classic").toObject();
            QJsonObject family = classic.value("solid").toObject();
            if (!family.isEmpty()) {
                icon.family = familyToString(FreeSolid);
                m_supportedIcons.append(icon);
                continue;
            }

            family = classic.value("brands").toObject();
            if (!family.isEmpty()) {
                icon.family = familyToString(BrandRegular);
                m_supportedIcons.append(icon);
                continue;
            }

            family = classic.value("regular").toObject();
            if (!family.isEmpty()) {
                icon.family = familyToString(FreeRegular);
                m_supportedIcons.append(icon);
                continue;
            }
        }

    } else {
        qWarning() << "open file failed:" << file.errorString();
    }
}

SAKFontAwesome *SAKFontAwesome::instance()
{
    static SAKFontAwesome *instance = nullptr;
    if (!instance) {
        instance = new SAKFontAwesome(qApp);
        instance->addApplicationFonts();
    }

    return instance;
}

QList<SAKFontAwesomeIconContext> SAKFontAwesome::supportedIcons() const
{
    return m_supportedIcons;
}

QString SAKFontAwesome::familyToString(int family)
{
    switch (family) {
    case BrandRegular:
        return QString("Font Awesome 6 Brands Regular");
    case FreeRegular:
        return QString("Font Awesome 6 Free Regular");
    case FreeSolid:
        return QString("Font Awesome 6 Free Solid");
    default:
        qWarning() << "unknown font family:" << family;
        return QString();
    }
}

QString SAKFontAwesome::fontAwesomePath() const
{
    QString path = QCoreApplication::applicationDirPath();
    path += "/fonts/" + QString(SAK_FONTAWESOME_DIR);
    return path;
}

void SAKFontAwesome::addApplicationFonts()
{
    QString path = fontAwesomePath() + "/otfs/";
    QString name = path + QString("Font Awesome 6 Brands-Regular-400.otf");
    int fontId = QFontDatabase::addApplicationFont(name);
    qInfo() << "Install font:" << QFontDatabase::applicationFontFamilies(fontId);

    name = path + QString("Font Awesome 6 Free-Regular-400.otf");
    fontId = QFontDatabase::addApplicationFont(name);
    qInfo() << "Install font:" << QFontDatabase::applicationFontFamilies(fontId);

    name = path + QString("Font Awesome 6 Free-Solid-900.otf");
    fontId = QFontDatabase::addApplicationFont(name);
    qInfo() << QFontDatabase::applicationFontFamilies(fontId);
}