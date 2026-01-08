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

#include "utilities/hdpimanager.h"
#include "utilities/i18n.h"
#include "utilities/stylemanager.h"
#include "utilities/thememanager.h"

#include "xapp.h"
#include "xtools.h"

xUi::xUi(QWidget *parent)
    : QMainWindow(parent)
{
    winId(); // Ensure the window is created before moving it to the center
    initMenuBar();
#ifdef X_MAGIC
    setWindowOpacity(0.3);
#endif
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
    initMenuBarFile();
    initMenuBarOption();
    initMenuBarLanguage();
    initMenuBarHelp();
}

void xUi::initMenuBarFile()
{
    m_fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(m_fileMenu);

    QAction *action = m_fileMenu->addAction(tr("Exit Application"), this, []() {
        QApplication::closeAllWindows();
        QApplication::quit();
    });
    action->setShortcut(QKeySequence("Ctrl+Q"));
}

void xUi::initMenuBarOption()
{
    m_optionMenu = new QMenu(tr("&Option"), this);
    menuBar()->addMenu(m_optionMenu);
    initMenuBarOptionHdpi();
    initMenuBarOptionStyle();
    initMenuBarOptionTheme();
    initMenuBarOptionSetting();
    initMenuBarOptionStaysOnTop();
}

void xUi::initMenuBarOptionHdpi()
{
    QMenu *hdpiMenu = xHdpiMgr.hdpiMenu();
    if (hdpiMenu) {
        hdpiMenu->setTitle(tr("High DPI Scaling Policy"));
        m_optionMenu->addMenu(hdpiMenu);
    }
}

void xUi::initMenuBarOptionStyle()
{
    QMenu *styleMenu = xStyleMgr.styleMenu();
    if (styleMenu) {
        styleMenu->setTitle(tr("Application Style"));
        m_optionMenu->addMenu(styleMenu);
    }
}

void xUi::initMenuBarOptionTheme()
{
    QMenu *themeMenu = xThemeMgr.themeMenu();
    if (themeMenu) {
        themeMenu->setTitle(tr("Theme"));
        m_optionMenu->addMenu(themeMenu);
    }
}

void xUi::initMenuBarOptionSetting()
{
    m_optionMenu->addSeparator();
    m_optionMenu->addAction(tr("Open Settings Directory"), this, []() {
        QString path = xAPP->settingsPath();
        QDesktopServices::openUrl(QUrl(path));
    });

    m_optionMenu->addAction(tr("Clear Settings File"), this, []() {
        xAPP->settings()->clear();
        xAPP->settings()->sync();
    });
}

void xUi::initMenuBarOptionStaysOnTop()
{
    m_optionMenu->addSeparator();
    QAction *action = m_optionMenu->addAction(tr("Stays on Top"));
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

void xUi::initMenuBarLanguage()
{
    m_languageMenu = xI18n.languageMenu();
    menuBar()->addMenu(m_languageMenu);
    m_languageMenu->setTitle(tr("&Language"));
}

void xUi::initMenuBarHelp()
{
    m_helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(m_helpMenu);
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

    m_helpMenu->addSeparator();
    const QString feedback = tr("User Feedback");
    QString name = QLocale::system().name();
    QString url = "https://github.com/x-tools-author/x-tools/issues";
    if (name == "zh_CN" || name == "zh_TW") {
        url = "https://gitee.com/x-tools-author/x-tools/issues";
    }
    QAction *feedbackAction = m_helpMenu->addAction(feedback);
    connect(feedbackAction, &QAction::triggered, this, [=]() {
        QDesktopServices::openUrl(QUrl(url));
    });
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
    info += qApp->applicationName() + " " + qApp->applicationVersion() + "\n\n";

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
    QString dtString = xBuildDateTimeString(buildDateTimeFormat);
    QString year = xBuildDateTimeString("yyyy");
    info += tr("Build Date") + ": " + dtString + "\n\n";
    info += tr("Copyright") + ": " + QString("2025-%1 x-tools@outlook.com. ").arg(year);
    info += tr("All rights reserved.");
    QMessageBox::about(this, tr("About") + QString(" ") + xAPP->applicationName(), info);
}
