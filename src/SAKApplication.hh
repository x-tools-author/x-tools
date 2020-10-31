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

#ifdef SAK_IMPORT_SQL_MODULE
#include <QSqlError>
#include <QSqlDatabase>
#endif

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

    /**
     * @brief installLanguage: Setup the language packet of application.
     * the interface must be called before initalizing any text.
     */
    void installLanguage();

    /**
     * @brief buildDateTime: Get the build date time info
     * @return The build date time info
     */
    QDateTime *buildDateTime();
#ifdef SAK_IMPORT_SQL_MODULE
    /**
     * @brief sqlDatabase: Get the instance of sql database
     * @return The instance of sql database
     */
    QSqlDatabase *sqlDatabase();
#endif

    /**
     * @brief dataPath: Get the settings file.
     * @return
     */
    QString dataPath();

    /**
     * @brief settings: Get the pointer of settings instance
     * @return The pointer of settings instance
     */
    QSettings *settings();

    SettingsKeyContext *settingsKeyContext();
private:
    SettingsKeyContext mSettingsKeyContext;
    QTranslator mQtTranslator;
    QTranslator mQtBaseTranslator;
    QTranslator mSakTranslator;
    SAKMainWindow *mMainWindow;
    QSettings *mSettings;
    QString mDatabaseName;
    QString mLastDataTime;
    QString mSettingsFileName;
#ifdef SAK_IMPORT_SQL_MODULE
    QSqlDatabase mSqlDatabase;
#endif
};

#endif
