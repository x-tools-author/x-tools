/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QApplication>
#include <QDateTime>
#include <QMainWindow>
#include <QPixmap>
#include <QSplashScreen>

class QLineEdit;
class xToolsApplication : public QApplication
{
    Q_OBJECT
    Q_PROPERTY(QStringList supportedLanguages READ supportedLanguages CONSTANT FINAL)
public:
    enum class AppPalette { Default, Dark, Light, Custom = -1 };
    Q_ENUM(AppPalette)
    enum class ValidatorType { Bin, Otc, Dec, Hex, Ascii, Float, None };
    Q_ENUM(ValidatorType)

public:
    explicit xToolsApplication(int argc, char *argv[]);

    static bool enableSplashScreen();
    static void setEnableSplashScreen(bool enable);

    static QString friendlyAppName();
    static void setFriendlyAppName(const QString &name);

    void showSplashScreenMessage(const QString &msg);
    void setupPalette(const QString &fileName);
    QSplashScreen &splashScreen();
    QStringList supportedLanguages();

    QString language();
    virtual void setupLanguage(const QString &language);

public:
    static void setValidator(QLineEdit *target, int validatorType, int maxLength = INT_MAX);
    static QIcon cookedIcon(const QIcon &icon);
    static QMainWindow *mainWindow();
    static void moveToScreenCenter(QWidget *widget);

    Q_INVOKABLE static QString clipboardText();
    Q_INVOKABLE static void setClipboardText(const QString &text);

    Q_INVOKABLE static QString dateTimeString(const QString &format);
    Q_INVOKABLE static QString stringToHexString(const QString &str);
    Q_INVOKABLE static QString hexStringToString(const QString &str);
    Q_INVOKABLE static QDateTime buildDateTime();
    Q_INVOKABLE static QString buildDateTimeString(const QString &format);
    Q_INVOKABLE static QString systemDateFormat();
    Q_INVOKABLE static QString systemTimeFormat();
    Q_INVOKABLE static QString desktopPath();

signals:
    void languageChanged();

protected:
    QSplashScreen m_splashScreen;

protected:
    void setupLanguageWithPrefix(const QString &language, const QString &prefix);

private:
    const QString m_translatorPrefix{"xToolsCommon"};
    QMap<QString, QString> m_languageFlagNameMap;
    static bool m_enableSplashScreen;
    static QString m_friendlyAppName;

private:
    static QPixmap splashScreenPixMap();
};
