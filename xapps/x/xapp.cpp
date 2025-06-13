/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xapp.h"

#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QLocale>
#include <QMessageBox>
#include <QPainter>
#include <QProcess>
#include <QScreen>
#include <QStandardPaths>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleHints>
#include <QTimer>
#include <QTranslator>

#include "common/xtools.h"

xApp::xApp(int &argc, char **argv)
    : QApplication(argc, argv)
{}

xApp::~xApp() {}

QSettings *xApp::settings()
{
    QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
    QString path = QStandardPaths::writableLocation(type);
    QString fileName = QString("%1/%2.ini").arg(path, applicationName());
    static QSettings settings(fileName, QSettings::IniFormat);

    return &settings;
}

QString xApp::settingsPath()
{
    QSettings *settings = xApp::settings();
    QString path = settings->fileName();
    return path.left(path.lastIndexOf('/'));
}

QVariant xApp::value(QAnyStringView key, const QVariant &defaultValue)
{
    QSettings *settings = xApp::settings();
    return settings->value(key, defaultValue);
}

void xApp::setValue(QAnyStringView key, const QVariant &value)
{
    QSettings *settings = xApp::settings();
    settings->setValue(key, value);
}

QSplashScreen *xApp::splashScreen()
{
    if (!qApp) {
        return Q_NULLPTR;
    }

    static QSplashScreen *splashScreen = Q_NULLPTR;
    if (!splashScreen) {
        QFont font = qApp->font();
        font.setPixelSize(52);

        QFontMetrics fontMetrics(font);
        const QString displayName = applicationName();
        int width = fontMetrics.boundingRect(displayName).width() * 1.2;
        QPixmap pixMap(width < 600 ? 600 : width, 260);

        pixMap.fill(QColor(0x1f1f1f));

        QPainter painter(&pixMap);
        painter.setPen(QColor(Qt::white));
        painter.setFont(font);
        painter.drawText(pixMap.rect(), Qt::AlignHCenter | Qt::AlignVCenter, displayName);
        painter.setPen(QColor(0x1f2c9f));
        painter.drawRect(pixMap.rect() - QMargins(1, 1, 1, 1));

        splashScreen = new QSplashScreen(pixMap);
        splashScreen->setDisabled(true);
    }

    return splashScreen;
}

void xApp::showSplashScreenMessage(const QString &msg)
{
    QSplashScreen *splashScreen = xApp::splashScreen();
    if (!splashScreen) {
        return;
    }

    splashScreen->show();
    splashScreen->showMessage(msg, Qt::AlignBottom | Qt::AlignLeft, Qt::white);
    QApplication::processEvents();
}

void xApp::execMs(int ms)
{
    QTimer tmp;
    QEventLoop loop;
    tmp.setSingleShot(true);
    QObject::connect(&tmp, &QTimer::timeout, &loop, &QEventLoop::quit);
    tmp.start(ms);
    loop.exec();
}

void xApp::tryToReboot()
{
    QString title = tr("Need to Reboot");
    QString text = tr("The operation need to reboot to effected, reboot the application now?");
    int ret = QMessageBox::information(nullptr, title, text, QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
        execMs(100);
        closeAllWindows();
    }
}

void xApp::setupHdpi()
{
    QSettings *settings = xApp::settings();
    int defaultPolicy = static_cast<int>(QApplication::highDpiScaleFactorRoundingPolicy());
    int policy = settings->value(SettingKeys().hdpi, defaultPolicy).toInt();

    const auto cookedPolicy = static_cast<Qt::HighDpiScaleFactorRoundingPolicy>(policy);
    QApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
    qInfo() << "The current high dpi policy is:" << cookedPolicy;
}

void setupLanguage(const QString &qmFile)
{
    QTranslator *translator = new QTranslator();
    if (!translator->load(qmFile)) {
        auto info = QString("The language file(%1) can not be loaded, English will be used.")
                        .arg(qmFile);
        qWarning() << info;
        return;
    }

    if (!qApp->installTranslator(translator)) {
        qWarning() << "The language has been setup, English will be used.";
    } else {
        qInfo() << "The language has been setup, current language file is:" << qmFile;
    }
}

void xApp::setupLanguage()
{
    SettingKeys keys;
    QSettings *settings = xAPP->settings();
    QString defaultLanguage = QLocale::system().name();
    QString language = settings->value(keys.language, defaultLanguage).toString();

    QString appPath = QApplication::applicationDirPath();
    QString qtQmFile = QString("%1/translations/qt_%2.qm").arg(appPath, language);
    ::setupLanguage(qtQmFile);

    QString appQmFile = QString("%1/translations/%2_%3.qm").arg(appPath, applicationName(), language);
    ::setupLanguage(appQmFile);
}

void xApp::setupTheme()
{
    SettingKeys keys;
    QSettings *settings = xApp::settings();
    auto def = qApp->styleHints()->colorScheme();
    int theme = settings->value(keys.theme, static_cast<int>(def)).toInt();
    Qt::ColorScheme cookedColorScheme = static_cast<Qt::ColorScheme>(theme);
    QStyleHints *hints = qApp->styleHints();
    hints->setColorScheme(cookedColorScheme);
    qInfo() << "The current color scheme is:" << qApp->styleHints()->colorScheme();
}
