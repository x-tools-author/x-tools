/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xui.h"

#include <QActionGroup>
#include <QClipboard>
#include <QDateTime>
#include <QDesktopServices>
#include <QFileDialog>
#include <QLocale>
#include <QMenuBar>
#include <QMessageBox>
#include <QScreen>
#include <QStandardPaths>
#include <QStyleHints>
#include <QUrl>

#if defined(_MSC_VER)
#include <dwmapi.h>

#include <QColor>
#include <QSysInfo>
#include <QWindow>
#endif

#ifdef Q_OS_MACOS
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#endif

#include "common/xtools.h"
#include "xapp.h"

xUi::xUi(QWidget *parent)
    : QMainWindow(parent)
{
    winId(); // Ensure the window is created before moving it to the center
    initMenuBar();
}

xUi::~xUi() {}

void xUi::moveToCenter()
{
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}

void xUi::initMenuBar()
{
    m_optionMenu = menuBar()->addMenu(tr("Option"));
    m_viewMenu = menuBar()->addMenu(tr("View"));
    m_helpMenu = menuBar()->addMenu(tr("Help"));

    initMenuBarOption();
    initMenuBarView();
    initMenuBarHelp();
}

void xUi::initMenuBarOption()
{
    initMenuBarOptionLanguage();
    m_optionMenu->addSeparator();
    initMenuBarOptionSetting();
}

void xUi::initMenuBarOptionLanguage()
{
    QMenu *menu = m_optionMenu->addMenu(tr("Language"));
    QMap<QString, QString> languageFlagNameMap;
    languageFlagNameMap.insert("ar", "العربية");         // 阿拉伯语
    languageFlagNameMap.insert("cs", "Čeština");         // 捷克语
    languageFlagNameMap.insert("da", "Dansk");           // 丹麦语
    languageFlagNameMap.insert("de", "Deutsch");         // 德语
    languageFlagNameMap.insert("es", "Español");         // 西班牙语
    languageFlagNameMap.insert("en", "English");         // 英语
    languageFlagNameMap.insert("fa", "فارسی");           // 波斯语
    languageFlagNameMap.insert("fi", "Suomi");           // 芬兰语
    languageFlagNameMap.insert("fr", "Français");        // 法语
    languageFlagNameMap.insert("he", "עִבְרִית");           // 希伯来语
    languageFlagNameMap.insert("uk", "українська мова"); // 乌克兰语
    languageFlagNameMap.insert("it", "Italiano");        // 意大利语
    languageFlagNameMap.insert("ja", "日本语");          // 日语
    languageFlagNameMap.insert("ko", "한글");            // 韩语
    languageFlagNameMap.insert("lt", "Lietuvių kalba");  // 立陶宛语
    languageFlagNameMap.insert("nl", "Nederlands");      // 荷兰语
    languageFlagNameMap.insert("pl", "Polski");          // 波兰语
    languageFlagNameMap.insert("pt", "Português");       // 葡萄牙语
    languageFlagNameMap.insert("ru", "русский язык");    // 俄语
    languageFlagNameMap.insert("sk", "Slovenčina");      // 斯洛伐克语
    languageFlagNameMap.insert("sl", "Slovenščina");     // 斯洛文尼亚语
    languageFlagNameMap.insert("sv", "Svenska");         // 瑞典语
    languageFlagNameMap.insert("zh_CN", "简体中文");     // 简体中文
    languageFlagNameMap.insert("zh_TW", "繁體中文");     // 繁體中文

    QString appLanguage = xAPP->appLanguageFlag();
    static QActionGroup *actionGroup = new QActionGroup(this);
    QStringList nameKeys = languageFlagNameMap.keys();
    for (const QString &lang : std::as_const(nameKeys)) {
        QString name = languageFlagNameMap.value(lang);
        QAction *action = menu->addAction(name);
        action->setCheckable(true);
        action->setData(lang);
        actionGroup->addAction(action);
        if (lang == appLanguage) {
            action->setChecked(true);
        }

        connect(action, &QAction::triggered, this, [=]() {
            QString lang = action->data().toString();
            xAPP->setValue(xApp::SettingKeys().language, lang);
            xAPP->setupLanguage();
            xAPP->tryToReboot();
        });
    }
}

void xUi::initMenuBarOptionSetting()
{
    m_optionMenu->addAction(tr("Open Settings Directory"), this, []() {
        QString path = xAPP->settingsPath();
        QDesktopServices::openUrl(QUrl(path));
    });

    m_optionMenu->addAction(tr("Clear Settings File"), this, []() {
        xAPP->settings()->clear();
        xAPP->settings()->sync();
    });
}

void xUi::initMenuBarView()
{
    initMenuBarViewHdpi();
    initMenuBarViewTheme();
    m_viewMenu->addSeparator();
    initializeMenuBarViewStaysOnTop();
}

void xUi::initMenuBarViewHdpi()
{
    struct item
    {
        int policy;
        QString name;
    };
    QList<item> items = {{static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::Round),
                          tr("Round up for .5 and above")},
                         {static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor),
                          tr("Round up for .75 and above")},
                         {static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::Ceil),
                          tr("Always round up")},
                         {static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::Floor),
                          tr("Always round down")},
                         {static_cast<int>(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough),
                          tr("Don't round")}};

    int defaultPolicy = static_cast<int>(xAPP->highDpiScaleFactorRoundingPolicy());
    int currentPolicy = xAPP->value(xApp::SettingKeys().hdpi, defaultPolicy).toInt();
    static QActionGroup *actionGroup = new QActionGroup(this);
    QMenu *menu = m_viewMenu->addMenu(tr("High DPI Settings"));
    for (const item &item : items) {
        int policy = item.policy;
        QString name = item.name;
        QAction *action = menu->addAction(name);
        action->setCheckable(true);
        action->setData(policy);
        if (policy == currentPolicy) {
            action->setChecked(true);
        }

        connect(action, &QAction::triggered, this, [=]() {
            auto policy = static_cast<Qt::HighDpiScaleFactorRoundingPolicy>(action->data().toInt());
            qApp->setHighDpiScaleFactorRoundingPolicy(policy);
            xAPP->setValue(xApp::SettingKeys().hdpi, static_cast<int>(policy));
        });
    }
}

void xUi::initMenuBarViewTheme()
{
    struct item
    {
        int theme;
        QString name;
    };
    QList<item> items = {{static_cast<int>(Qt::ColorScheme::Dark), tr("Dark")},
                         {static_cast<int>(Qt::ColorScheme::Light), tr("Light")},
                         {static_cast<int>(Qt::ColorScheme::Unknown), tr("System Default")}};

    int defaultTheme = static_cast<int>(qApp->styleHints()->colorScheme());
    int currentTheme = xAPP->value(xApp::SettingKeys().theme, defaultTheme).toInt();
    static QActionGroup *actionGroup = new QActionGroup(this);
    QMenu *menu = m_viewMenu->addMenu(tr("Application Theme"));
    for (const item &item : items) {
        int theme = item.theme;
        QString name = item.name;
        QAction *action = menu->addAction(name);
        action->setCheckable(true);
        action->setData(static_cast<int>(theme));
        if (theme == static_cast<int>(currentTheme)) {
            action->setChecked(true);
        }

        actionGroup->addAction(action);
        connect(action, &QAction::triggered, this, [=]() {
            auto theme = static_cast<Qt::ColorScheme>(action->data().toInt());
            qApp->styleHints()->setColorScheme(theme);
            xAPP->setValue(xApp::SettingKeys().theme, static_cast<int>(theme));
            updateWindowTitleArea(this);
        });
    }

    updateWindowTitleArea(this);
}

void xUi::initializeMenuBarViewStaysOnTop()
{
    QAction *action = m_viewMenu->addAction(tr("Stays on Top"));
    action->setCheckable(true);
    connect(action, &QAction::triggered, this, [=]() {
        bool staysOnTop = action->isChecked();
        if (staysOnTop) {
            setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        } else {
            setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
        }
        show(); // Refresh the window flags
    });
}

void xUi::initMenuBarHelp()
{
    QAction *aboutQtAction = m_helpMenu->addAction(tr("About Qt"));
    connect(aboutQtAction, &QAction::triggered, this, []() { QApplication::aboutQt(); });

    QAction *aboutAction = m_helpMenu->addAction(tr("About") + " " + qApp->applicationName());
    connect(aboutAction, &QAction::triggered, this, &xUi::showAboutInfo);

    m_helpMenu->addSeparator();
    initMenuBarHelpGrab();
    m_helpMenu->addSeparator();

    struct Item
    {
        QString name;
        QString url;
    };
    QList<Item> items = {
        {tr("Get Source from Gitee"), "https://gitee.com/x-tools-author/x-tools"},
        {tr("Get Source from Github"), "https://github.com/x-tools-author/x-tools"},
        {tr("Visit Author Home Page"), "https://github.com/x-tools-author"},
        {tr("Visit App Store"), "https://apps.microsoft.com/search/publisher?name=x-tools-author"},
    };
    for (const Item &item : items) {
        QAction *action = m_helpMenu->addAction(item.name);
        QString url = item.url;
        connect(action, &QAction::triggered, this, [=]() { QDesktopServices::openUrl(QUrl(url)); });
    }
}

void xUi::initMenuBarHelpGrab()
{
    m_helpMenu->addAction(tr("Grab and Save"), this, [=]() {
        QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString fileName
            = QFileDialog::getSaveFileName(this,
                                           tr("Save Grabbed Image"),
                                           desktop + "/" + xAPP->applicationName() + ".png",
                                           QString("PNG (*.png );; JPEG (*.jpg);; BMP (*.bmp)"));
        if (fileName.isEmpty()) {
            return; // User canceled the save dialog
        }

        QPixmap pix = grab();
        pix.save(fileName);
    });

    m_helpMenu->addAction(tr("Grab and Copy"), this, [=]() {
        QPixmap pix = grab();
        QApplication::clipboard()->setPixmap(pix);
    });
}

void xUi::showAboutInfo()
{
    QString info;
    info += qApp->applicationName() + " v" + qApp->applicationVersion() + "\n\n";

#if defined(X_GIT_COMMIT_TIME)
    QString commitTime = QString(X_GIT_COMMIT_TIME);
    if (!commitTime.isEmpty()) {
        info += tr("Commit time") + ": " + commitTime + "\n\n";
    }
#endif

#ifdef X_GIT_COMMIT
    QString commit = QString(X_GIT_COMMIT);
    if (!commit.isEmpty()) {
        info += tr("Commit") + ": " + commit + "\n\n";
    }
#endif

    QLocale locale = QLocale::system();
    QString buildDateTimeFormat = locale.dateFormat();
    buildDateTimeFormat += " ";
    buildDateTimeFormat += locale.timeFormat();
    QString dtString = buildDateTimeString(buildDateTimeFormat);
    QString year = buildDateTimeString("yyyy");
    info += tr("Build Date") + ": " + dtString + "\n\n";
    info += tr("Copyright") + QString(" 2025-%1 x-tools-author(x-tools@outlook.com).").arg(year);
    info += tr("All rights reserved.");
    QMessageBox::about(this, tr("About") + QString(" ") + xAPP->applicationName(), info);
}

void xUi::updateWindowTitleArea(QWidget *widget)
{
#if defined(_MSC_VER)
    if (QSysInfo::productVersion().contains("11")) {
        // const DWORD attribute = 35;
        // DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR
        QWindow *window = widget->windowHandle();
        if (window) {
            QColor c = xAPP->palette().color(QPalette::Window);
            COLORREF colorref = c.red() | (c.green() << 8) | (c.blue() << 16);
            DwmSetWindowAttribute((HWND) window->winId(),
                                  DWMWA_CAPTION_COLOR,
                                  &colorref,
                                  sizeof(colorref));
#if 0
            c = currentTheme.primaryColor;
            colorref = c.red() | (c.green() << 8) | (c.blue() << 16);
            DwmSetWindowAttribute((HWND) window->winId(),
                                  DWMWA_BORDER_COLOR,
                                  &colorref,
                                  sizeof(colorref));
#endif
        }
    }
#endif

#if defined(Q_OS_LINUX)
    // Nothing to do yet...
#endif

#if defined(Q_OS_MACOS)
    NSView *view = reinterpret_cast<NSView *>(winId());
    NSWindow *w = [view window];

    [w setStyleMask:([w styleMask] | NSWindowStyleMaskFullSizeContentView)];
    [w setTitlebarAppearsTransparent:YES];
    [w setTitleVisibility:NSWindowTitleHidden];

    NSButton *zoomButton = [w standardWindowButton:NSWindowZoomButton];
    [zoomButton setEnabled:YES];
#endif
}