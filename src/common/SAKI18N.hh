/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef SAKI18N_HH
#define SAKI18N_HH

#include <QMap>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QTranslator>
#include <QVariantList>
#include <QLoggingCategory>

class SAKI18N : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList languanges READ languanges CONSTANT)
private:
    explicit SAKI18N(QObject *parent = nullptr);

public:
    static SAKI18N* instance();
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

#endif // SAKI18N_HH
