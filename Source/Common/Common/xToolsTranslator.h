/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QMap>
#include <QObject>
#include <QTranslator>
#include <QVariantList>

class xToolsTranslator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList languanges READ languanges CONSTANT)
private:
    explicit xToolsTranslator(QObject *parent = nullptr);

public:
    static xToolsTranslator *instance();
    QStringList languanges();
    Q_INVOKABLE void setupLanguage(const QString &language);

private:
    QMap<QString, QString> m_flagNameMap;
    QTranslator m_translator;

signals:
    void languageChanged();
};
