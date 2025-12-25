/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "application.h"

#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QJsonObject>
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

#if defined(_MSC_VER)
#include <dwmapi.h>

#include <QColor>
#include <QSysInfo>
#include <QWindow>
#endif

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{}

Application::~Application() {}

QString Application::gitCommit()
{
#if defined(X_GIT_COMMIT)
    return QStringLiteral(X_GIT_COMMIT);
#else
    return QStringLiteral("unknown");
#endif
}

QString Application::buildDateTime(const QString &format)
{
    QString dateString = QString(__DATE__);
    QString timeString = QString(__TIME__);
    dateString = dateString.replace(QString("  "), " 0");
    QString dateTimeString = dateString + " " + timeString;
    QDateTime dateTime = QLocale(QLocale::English).toDateTime(dateTimeString, "MMM dd yyyy hh:mm:ss");
    if (format.isEmpty()) {
        QString dateFormat = QLocale::system().dateFormat();
        QString timeFormat = QLocale::system().timeFormat();
        return dateTime.toString(dateFormat + " " + timeFormat);
    } else {
        return dateTime.toString(format);
    }
}

QJsonArray Application::supportedLanguages()
{
    QString appPath = QApplication::applicationDirPath();
    appPath += "/translations";
    QDir dir(appPath);
    QStringList filters;
    filters << QString("xTools_*.qm");
    QList<QFileInfo> fileNames = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
    QJsonArray languages;
    for (const QFileInfo &fileName : const_cast<QList<QFileInfo> &>(fileNames)) {
        QString baseName = fileName.baseName();
        baseName.remove("xTools_");
        baseName.remove(".qm");
        QLocale locale(baseName);
        if (!locale.language()) {
            continue;
        }
        QJsonObject lang;
        lang["value"] = baseName;
        lang["text"] = locale.nativeLanguageName();
        languages.append(lang);
    }
    return languages;
}

QString Application::appLanguageFlag()
{
    QSettings *settings = xApp->settings();
    QString defaultLanguage = QLocale::system().name();
    QString language = settings->value(SettingKeys().language, defaultLanguage).toString();
    QString appPath = QApplication::applicationDirPath();
    QString qmFile = QString("%1/translations/%2_%3.qm").arg(appPath, applicationName(), language);
    if (!QFile::exists(qmFile)) {
        return "en";
    }

    return language;
}

QSettings *Application::settings()
{
    QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
    QString path = QStandardPaths::writableLocation(type);
    QString fileName = QString("%1/%2.ini").arg(path, applicationName());
    static QSettings settings(fileName, QSettings::IniFormat);

    return &settings;
}

QString Application::settingsPath()
{
    QSettings *settings = Application::settings();
    QString path = settings->fileName();
    return path.left(path.lastIndexOf('/'));
}

QVariant Application::settingsValue(const QString &key, const QVariant &dv)
{
    QSettings *settings = Application::settings();
    return settings->value(key, dv);
}

void Application::setSettingsValue(const QString &key, const QVariant &value)
{
    QSettings *settings = Application::settings();
    settings->setValue(key, value);
}

QSplashScreen *Application::splashScreen()
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

void Application::showSplashScreenMessage(const QString &msg)
{
    QSplashScreen *splashScreen = Application::splashScreen();
    if (!splashScreen) {
        return;
    }

    splashScreen->show();
    splashScreen->showMessage(msg, Qt::AlignBottom | Qt::AlignLeft, Qt::white);
    QApplication::processEvents();
}

void Application::updateWindowStyle(QWindow *window, const QColor &color)
{
    if (!window) {
        return;
    }

    if (!color.isValid()) {
        return;
    }

#if defined(_MSC_VER)
    QColor c(color);
    COLORREF colorref = c.red() | (c.green() << 8) | (c.blue() << 16);
    DwmSetWindowAttribute((HWND) window->winId(), DWMWA_CAPTION_COLOR, &colorref, sizeof(colorref));
#endif
}

void Application::execMs(int ms)
{
    QTimer tmp;
    QEventLoop loop;
    tmp.setSingleShot(true);
    QObject::connect(&tmp, &QTimer::timeout, &loop, &QEventLoop::quit);
    tmp.start(ms);
    loop.exec();
}

void Application::tryToReboot()
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

void Application::setupHdpi()
{
    QSettings *settings = Application::settings();
    int defaultPolicy = static_cast<int>(QApplication::highDpiScaleFactorRoundingPolicy());
    int policy = settings->value(SettingKeys().hdpi, defaultPolicy).toInt();

    const auto cookedPolicy = static_cast<Qt::HighDpiScaleFactorRoundingPolicy>(policy);
    QApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
    qInfo() << "The current high dpi policy is:" << cookedPolicy;
}

void Application::setupLanguage()
{
    if (m_translators.count() > 0) {
        for (QTranslator *translator : const_cast<QList<QTranslator *> &>(m_translators)) {
            qApp->removeTranslator(translator);
            translator->deleteLater();
        }
        m_translators.clear();
    }

    QString language = appLanguageFlag();
    QString appPath = QApplication::applicationDirPath();
    QString qtQmFile = QString("%1/translations/qt_%2.qm").arg(appPath, language);
    setupLanguage(qtQmFile);

    QString appQmFile = QString("%1/translations/%2_%3.qm").arg(appPath, applicationName(), language);
    setupLanguage(appQmFile);

    emit languageChanged();
}

void Application::setupLanguage(const QString &qmFile)
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
        m_translators.append(translator);
        qInfo() << "The language has been setup, current language file is:" << qmFile;
    }
}

void Application::setupTheme()
{
    SettingKeys keys;
    QSettings *settings = Application::settings();
    auto def = qApp->styleHints()->colorScheme();
    int theme = settings->value(keys.theme, static_cast<int>(def)).toInt();
    Qt::ColorScheme cookedColorScheme = static_cast<Qt::ColorScheme>(theme);
    QStyleHints *hints = qApp->styleHints();
    hints->setColorScheme(cookedColorScheme);
    qInfo() << "The current color scheme is:" << qApp->styleHints()->colorScheme();
}
