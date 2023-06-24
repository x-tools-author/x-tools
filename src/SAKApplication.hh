/******************************************************************************
 * Copyright 2018-2022 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKAPPLICATION_HH
#define SAKAPPLICATION_HH

#include <QSettings>
#include <QTranslator>
#include <QApplication>
#include <QStyleFactory>
#include <QSplashScreen>
#include <QLoggingCategory>

#define sakApp (static_cast<SAKApplication *>(QCoreApplication::instance()))

class SAKApplication:public QApplication
{
    Q_OBJECT
public:
    explicit SAKApplication(int argc, char **argv);
    ~SAKApplication();

    const QDate *buildDate();
    const QTime *buildTime();
    QString dataPath();
    QSettings *settings();
    QSplashScreen *splashScreen();
    void showSplashScreenMessage(QString msg);

private:
    QTranslator mQtTranslator;
    QTranslator mQtBaseTranslator;
    QTranslator mSakTranslator;
    QString mDatabaseName;
    QString mLastDataTime;
    QString mSettingsFileName;
    QSplashScreen *mSplashScreen;
    const QLoggingCategory mLoggingCategory{"SAK.Application"};

signals:
    void activeMainWindow();
};

#endif
