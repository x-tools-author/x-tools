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
#include <QPixmap>
#include <QSplashScreen>

class xToolsApplication : public QApplication
{
    Q_OBJECT
    Q_PROPERTY(QStringList supportedLanguages READ supportedLanguages CONSTANT FINAL)
public:
    enum xToolsAppPalette { DefaultPalette, DarkPalette, LightPalette, CustomPalette = -1 };

public:
    explicit xToolsApplication(int argc, char *argv[]);

    void showSplashScreenMessage(const QString &msg);
    void setupPalette(int palette);
    void setupPalette(const QString &fileName);
    QSplashScreen &splashScreen();
    QStringList supportedLanguages();

    virtual void setupLanguage(const QString &language);

signals:
    void languageChanged();

protected:
    QSplashScreen m_splashScreen;

protected:
    void setupLanguage(const QString &language, const QString &prefix);

private:
    const QString m_translatorPrefix{"xToolsCommon"};
    QMap<QString, QString> m_languageFlagNameMap;

private:
    static QPixmap splashScreenPixmap();
};
