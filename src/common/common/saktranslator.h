/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKTRANSLATOR_H
#define SAKTRANSLATOR_H

#include <QJsonArray>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QMap>
#include <QObject>
#include <QTranslator>
#include <QVariantList>

class SAKTranslator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList languanges READ languanges CONSTANT)
private:
    explicit SAKTranslator(QObject *parent = nullptr);

public:
    static SAKTranslator *instance();
    QStringList languanges();
    void setupLanguage(const QString &language);

private:
    QMap<QString, QString> m_flagNameMap;
    QTranslator m_translator;

signals:
    void languageChanged();
};

#endif // SAKTRANSLATOR_H
