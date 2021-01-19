/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKAPPLICATION_HH
#define SAKAPPLICATION_HH

#include <QSettings>
#include <QTranslator>
#include <QApplication>
#include <QStyleFactory>
#include <QSplashScreen>

#include <QSqlError>
#include <QSqlDatabase>

#define sakApp (static_cast<SAKApplication *>(QCoreApplication::instance()))

class SAKApplication:public QApplication
{
    Q_OBJECT
public:
    explicit SAKApplication(int argc, char **argv);
    ~SAKApplication();

    struct SettingsKeyContext {
        QString lastDateTime; // The last time of starting
        QString removeSettingsFile;
        QString removeDatabase;
        QString language;
        QString appStyle;
    };

    void installLanguage();
    const QDate *buildDate();
    const QTime *buildTime();
    QString dataPath();
    QSettings *settings();
    SettingsKeyContext *settingsKeyContext();
    QSplashScreen *splashScreen();
    void showSplashScreenMessage(QString msg);
    QSqlDatabase *sqlDatabase();
private:
    SettingsKeyContext mSettingsKeyContext;
    QTranslator mQtTranslator;
    QTranslator mQtBaseTranslator;
    QTranslator mSakTranslator;
    QSettings *mSettings;
    QString mDatabaseName;
    QString mLastDataTime;
    QString mSettingsFileName;
    QSplashScreen *mSplashScreen;
    QSqlDatabase mSqlDatabase;
signals:
    void activeMainWindow();
};

#endif
