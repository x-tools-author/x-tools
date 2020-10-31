/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QRect>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QScreen>
#include <QAction>
#include <QDateTime>
#include <QSettings>
#include <QTextCodec>
#include <QTextCursor>
#include <QTranslator>
#include <QStandardPaths>
#include <QDesktopWidget>

#include "SAKMainWindow.hh"
#include "SAKApplication.hh"
#include "SAKSplashScreen.hh"

static const QDate buildDate = QLocale( QLocale::English ).toDate( QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
SAKApplication::SAKApplication(int argc, char **argv)
    :QApplication (argc, argv)
    ,mMainWindow(Q_NULLPTR)
{
    // Initialize some information about application.
    setOrganizationName(QString("Qter"));
    setOrganizationDomain(QString("IT"));
    setApplicationName(QString("QtSwissArmyKnife"));

    // Initialize the setting key
    mSettingsKeyContext.lastDateTime = QString("%1/lastDateTime").arg(applicationName());
    mSettingsKeyContext.removeSettingsFile = QString("%1/removeSettingsFile").arg(applicationName());
    mSettingsKeyContext.removeDatabase = QString("%1/removeDatabase").arg(applicationName());
    mSettingsKeyContext.language = QString("%1/language").arg(applicationName());
    mSettingsKeyContext.appStyle = QString("%1/appStyle").arg(applicationName());

    // Initialize the settings file
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    mSettingsFileName = QString("%1/%2.ini").arg(path).arg(qApp->applicationName());
    mSettings = new QSettings(mSettingsFileName, QSettings::IniFormat);
    mSettings->setIniCodec(QTextCodec::codecForName("UTF-8"));
    mLastDataTime = mSettings->value(mSettingsKeyContext.lastDateTime).toString();
    mSettings->setValue(mSettingsKeyContext.lastDateTime, QDateTime::currentDateTime().toString(QLocale::system().dateFormat()));

#ifdef SAK_IMPORT_SQL_MODULE
    // Initialize the data base
    mDatabaseName = QString("%1/%2.sqlite3").arg(path).arg(qApp->applicationName());
    mSqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    mSqlDatabase.setDatabaseName(mDatabaseName);
    // Do something useless
    mSqlDatabase.setHostName("localhost");
    mSqlDatabase.setUserName("Qter");
    mSqlDatabase.setPassword("QterPassword");

    if (!mSqlDatabase.open()){
        qWarning() << __FUNCTION__ << "QSAKDatabase.sqlite3 open failed: " << mSqlDatabase.lastError().text();
        Q_ASSERT_X(false, __FUNCTION__, "Open database failed!");
    }
#endif

    // Setup language packet at first.
    installLanguage();

    // Show a splash screen.
    SAKSplashScreen *splashScreen = SAKSplashScreen::instance();
    splashScreen->show();
    processEvents();

    // Remove settings file
    if (mSettings->value(mSettingsKeyContext.removeSettingsFile).toBool()){
        mSettings->setValue(mSettingsKeyContext.removeSettingsFile, QVariant::fromValue(false));
        if (QFile::remove(mSettingsFileName)){
            qInfo() << "Remove settings file successfully!";
        }
    }

    // Remove database
    if (mSettings->value(mSettingsKeyContext.removeDatabase).toBool()){
        mSettings->setValue(mSettingsKeyContext.removeDatabase, QVariant::fromValue(false));

        QFile databaseFile(mDatabaseName);
        if (databaseFile.remove()){
            qInfo() << "Remove database successfully!";
        }else{
            qWarning() << "Remove database failed: " << databaseFile.errorString();
        }
    }

    // Set application version, if micro SAK_VERSION is not defined, the application version is "0.0.0"
#ifndef SAK_VERSION
    setApplicationVersion(QString("0.0.0"));
#else
    setApplicationVersion(SAK_VERSION);
#endif

    // There is bug: the application will crash if create and show a main window in the main().
    // the bug is appear on linux platform only.
    splashScreen->setMessage(tr("Initializing main window..."));
    mMainWindow = new SAKMainWindow;
    mMainWindow->show();

    // Move the main window to the central of desktop.
    QDesktopWidget *desktop = QApplication::desktop();
    int currentScreen = desktop->screenNumber(mMainWindow);
    QList<QScreen*> screenList = QGuiApplication::screens();
    QScreen *screen = screenList.at(currentScreen);
    mMainWindow->move((screen->geometry().width() - mMainWindow->width())/2, (screen->geometry().height() - mMainWindow->height())/2);
    splashScreen->setMessage(tr("Finished..."));
    splashScreen->finish(mMainWindow);
}

SAKApplication::~SAKApplication()
{
    delete mMainWindow;
    if (mSqlDatabase.isOpen()){
        mSqlDatabase.close();
    }
}

void SAKApplication::installLanguage()
{
    QString language = mSettings->value(mSettingsKeyContext.language).toString();
    QString qmName;
    if (language.isEmpty()){
        if (QLocale().country() == QLocale::China){
            qmName = QString("zh_CN");
        }else{
            qmName = QString("en");
        }
    }else{
        qmName = language.split('-').first();
    }

    mQtBaseTranslator.load(QString(":/translations/qt/qtbase_%1.qm").arg(qmName));
    qApp->installTranslator(&mQtBaseTranslator);

    mQtTranslator.load(QString(":/translations/qt/qt_%1.qm").arg(qmName));
    qApp->installTranslator(&mQtTranslator);

    mSakTranslator.load(QString(":/translations/sak/SAK_%1.qm").arg(qmName));
    qApp->installTranslator(&mSakTranslator);
}

SAKMainWindow *SAKApplication::mainWindow()
{
    return mMainWindow;
}

QDateTime *SAKApplication::buildDateTime()
{
    QDateTime *dateaTime = new QDateTime(QLocale(QLocale::English).toDateTime(QString(__DATE__) + QString(__TIME__), "MMM dd yyyyhh:mm:ss"));
    return dateaTime;
}

#ifdef SAK_IMPORT_SQL_MODULE
QSqlDatabase *SAKApplication::sqlDatabase()
{
    return &mSqlDatabase;
}
#endif

QString SAKApplication::dataPath()
{
    QString path = mSettings->fileName();
    QStringList temp = path.split('/');
    path = path.remove(temp.last());

    return path;
}

QSettings *SAKApplication::settings()
{
    return mSettings;
}

SAKApplication::SettingsKeyContext *SAKApplication::settingsKeyContext()
{
    return &mSettingsKeyContext;
}
