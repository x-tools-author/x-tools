/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDI18N_HPP
#define EDI18N_HPP

#include <QMap>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QTranslator>
#include <QVariantList>
#include <QLoggingCategory>

class EDI18N : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList languanges READ languanges CONSTANT)
private:
    explicit EDI18N(QObject *parent = nullptr);

public:
    static EDI18N* instance();
    Q_INVOKABLE void installTranslator(const QString &name);
    Q_INVOKABLE void setConfigurationFile(const QString &conf);

private:
    QList<QJsonObject> mLanguageContextList;
    QList<QTranslator*> mTranslators;
    QString mCurrentName;
    QString mConf;

private:
    void uninstallTranslator();
    void installTranslator(const QJsonArray &packets);

private:
    QVariantList languanges();
    const QLoggingCategory mCategory{"EDI18N"};

signals:
    void languageChanged();
};

#endif // EDI18N_HPP
