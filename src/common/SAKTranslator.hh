/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTRANSLATOR_HH
#define SAKTRANSLATOR_HH

#include <QMap>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QTranslator>
#include <QVariantList>
#include <QLoggingCategory>

class SAKTranslator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList languanges READ languanges CONSTANT)
private:
    explicit SAKTranslator(QObject *parent = nullptr);

public:
    static SAKTranslator* instance();
    QStringList languanges();
    void setupLanguage(const QString &language);

private:
    const QLoggingCategory mLoggingCategory{"sak.translator"};
    QMap<QString, QString> mFlagNameMap;
    QTranslator mTranslator;

signals:
    void languageChanged();
};

#endif // SAKTRANSLATOR_HH
