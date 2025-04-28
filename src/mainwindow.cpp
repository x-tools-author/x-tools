﻿/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mainwindow.h"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QFileDialog>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QMetaEnum>
#include <QNetworkProxyFactory>
#include <QPainter>
#include <QPixmap>
#include <QProcess>
#include <QScreen>
#include <QScrollBar>
#include <QStackedWidget>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleHints>
#include <QSvgRenderer>
#include <QTextBrowser>
#include <QVariant>

#include "application.h"
#include "common/xtools.h"
#include "page/page.h"
#include "page/page4x4.h"
#include "systemtrayicon.h"
#include "tools/assistantfactory.h"

#ifdef X_TOOLS_ENABLE_QSS
#include "qss/qssmgr.h"
#endif

#ifdef Q_OS_WIN
#include "systemtrayicon.h"
#endif

#ifdef X_ENABLE_MQTT
#include "mqtt/client/mqttclientui.h"
#include "mqtt/server/mqttserverui.h"
#endif

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_page4x4(Q_NULLPTR)
{
#if defined(X_TOOLS_MAGIC)
    setWindowOpacity(0.3);
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    qApp->styleHints()->setColorScheme(Qt::ColorScheme::Dark);
#endif
#endif

#ifdef Q_OS_WIN
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        auto* systemTrayIcon = new SystemTrayIcon(this);
        connect(systemTrayIcon, &SystemTrayIcon::invokeExit, this, [=]() {
            QApplication::closeAllWindows();
            QApplication::quit();
        });
        connect(systemTrayIcon, &SystemTrayIcon::invokeShowMainWindow, this, &MainWindow::show);
        systemTrayIcon->show();
    }
#endif

    QSettings* settings = Application::settings();
    m_toolBarActionGroup = new QActionGroup(this);
    m_stackedWidget = new QStackedWidget(this);
    m_toolBar = new QToolBar(this);
    m_toolBar->setMovable(false);
    m_toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_toolBar->setMovable(false);
    addToolBar(Qt::TopToolBarArea, m_toolBar);
    setCentralWidget(m_stackedWidget);

    setupPages();
    initMenuBar();
    setWindowIcon(QIcon(":/res/icons/logo.svg"));
    setWindowTitle(qApp->applicationName() + " v" + qApp->applicationVersion());
}

MainWindow::~MainWindow()
{
    qInfo() << "MainWindow is destroyed!";
}

void MainWindow::load(const QString& fileName) const
{
    QString filePath = fileName;
    if (fileName.isEmpty()) {
        const QString path = xApp->settingsPath();
        filePath = path + "/data.json";
    }

    if (!QFile::exists(filePath)) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    m_page4x4->load(obj.value("general").toObject().toVariantMap());
}

void MainWindow::save(const QString& fileName) const
{
    QJsonObject obj;
    obj.insert("general", QJsonObject::fromVariantMap(m_page4x4->save()));

    QJsonDocument doc;
    doc.setObject(obj);

    QString filePath = fileName;
    if (fileName.isEmpty()) {
        const QString path = xApp->settingsPath();
        filePath = path + "/data.json";
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Failed to open file: " << filePath << file.errorString();
        return;
    }

    QTextStream out(&file);
    out << doc.toJson();
    file.close();
}

void MainWindow::updateGrid(int grid)
{
    m_page4x4->updateGrid(static_cast<Page4x4::WindowGrid>(grid));
}

void MainWindow::moveToCenter()
{
    QRect screenRect = QApplication::primaryScreen()->geometry();
    bool tooWidth = (width() > screenRect.width());
    bool tooHeight = (height() > screenRect.height());
    if (tooWidth || tooHeight) {
        showMaximized();
        qInfo() << "The screen is too small.";
    } else {
        move((screenRect.width() - width()) / 2, (screenRect.height() - height()) / 2);
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    save();

#ifdef Q_OS_WIN
    QSettings* settings = xApp->settings();
    if (settings->value(m_settingsKey.exitToSystemTray).toBool()) {
        hide();
        event->accept();
        return;
    }
#endif

    QMainWindow::closeEvent(event);
}

QUrl MainWindow::storeUrl() const
{
    return QUrl("https://www.microsoft.com/store/apps/9P29H1NDNKBB");
}

void MainWindow::initMenuBar()
{
    initFileMenu();
    initToolMenu();
    initOptionMenu();
    initViewMenu();
    initMenuLanguage();
    initHelpMenu();
}

void MainWindow::initFileMenu()
{
    auto fileMenu = menuBar()->addMenu(tr("&File"));
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    QAction* action = fileMenu->addAction(tr("New Window"), this, []() {
        auto* w = new Page(Page::Left, xApp->settings());
        w->setWindowTitle("xTools");
        w->show();
    });
    action->setShortcut(QKeySequence::New);
    fileMenu->addSeparator();
#endif
    action = fileMenu->addAction(tr("Save Parameters"), this, &MainWindow::onSaveActionTriggered);
    action->setShortcut(QKeySequence::Save);
    action = fileMenu->addAction(tr("Import Parameters"),
                                 this,
                                 &MainWindow::onImportActionTriggered);
    action->setShortcut(QKeySequence::Open);
    action = fileMenu->addAction(tr("Export Parameters"),
                                 this,
                                 &MainWindow::onExportActionTriggered);
    action->setShortcut(QKeySequence::SaveAs);
    fileMenu->addSeparator();
    action = fileMenu->addAction(tr("Exit Application"), this, []() {
        QApplication::closeAllWindows();
        QApplication::quit();
    });
    action->setShortcut(QKeySequence::Quit);
}

void MainWindow::initToolMenu()
{
    auto toolMenu = menuBar()->addMenu(tr("&Tools"));

    QList<int> supportedAssistants = AssistantFactory::instance()->supportedAssistants();
    QMenu* newMenu = toolMenu->addMenu(tr("New"));
    toolMenu->addSeparator();

    for (auto& type : supportedAssistants) {
        QString name = AssistantFactory::instance()->assistantName(type);
        auto* action = new QAction(name, this);
        QWidget* assistant = AssistantFactory::instance()->newAssistant(type);

        Q_ASSERT_X(assistant, __FUNCTION__, "A null assistant widget!");

        assistant->hide();
        toolMenu->addAction(action);
        connect(action, &QAction::triggered, this, [=]() {
            if (assistant->isHidden()) {
                assistant->show();
            } else {
                assistant->activateWindow();
            }
        });

        action = new QAction(name, this);
        newMenu->addAction(action);
        connect(action, &QAction::triggered, this, [=]() {
            QWidget* assistant = AssistantFactory::instance()->newAssistant(type);
            if (!assistant) {
                return;
            }

            assistant->setAttribute(Qt::WA_DeleteOnClose, true);
            assistant->show();
        });
    }
}

void MainWindow::initOptionMenu()
{
    auto optionMenu = menuBar()->addMenu(tr("&Options"));

    initOptionMenuHdpiPolicy(optionMenu);
    initOptionMenuAppStyleMenu(optionMenu);
    initOptionMenuColorScheme(optionMenu);
    optionMenu->addSeparator();
    initOptionMenuSettingsMenu(optionMenu);
    optionMenu->addSeparator();

    auto* proxy = optionMenu->addAction(tr("Use System Proxy"));
    proxy->setCheckable(true);
    bool useSystemProxy = xApp->settings()->value(m_settingsKey.useSystemProxy).toBool();
    proxy->setChecked(useSystemProxy);
    QNetworkProxyFactory::setUseSystemConfiguration(proxy->isChecked());
    connect(proxy, &QAction::triggered, this, [=]() {
        QNetworkProxyFactory::setUseSystemConfiguration(proxy->isChecked());
        xApp->settings()->setValue(m_settingsKey.useSystemProxy, proxy->isChecked());
    });

    auto* trayAction = new QAction(tr("Exit to System Tray"), this);
    trayAction->setCheckable(true);
    optionMenu->addAction(trayAction);

    QVariant v = xApp->settings()->value(m_settingsKey.exitToSystemTray);
    if (!v.isNull()) {
        bool isExitToSystemTray = v.toBool();
        trayAction->setChecked(isExitToSystemTray);
    }

    connect(trayAction, &QAction::triggered, this, [=]() {
        bool keep = trayAction->isChecked();
        xApp->settings()->setValue(m_settingsKey.exitToSystemTray, keep);
    });
}

void MainWindow::initOptionMenuAppStyleMenu(QMenu* optionMenu)
{
    QMenu* appStyleMenu = optionMenu->addMenu(tr("Application Style"));
    static QActionGroup* appStyleActionGroup = new QActionGroup(this);
    for (QString& key : QStyleFactory::keys()) {
        auto* action = new QAction(key, this);
        action->setCheckable(true);
        appStyleActionGroup->addAction(action);

        if (key.toUpper() == xDefaultStyleName.toUpper()) {
            action->setChecked(true);
        }

        connect(action, &QAction::triggered, this, [=]() {
            Application::SettingsKey keys;
            xApp->settings()->setValue(keys.style, key.toLower());
            tryToReboot();
        });
    }

    appStyleMenu->addActions(appStyleActionGroup->actions());
#ifdef X_TOOLS_ENABLE_QSS
    optionMenu->addMenu(QssMgr::singleton().themeMenu());
#endif
}

void MainWindow::initOptionMenuSettingsMenu(QMenu* optionMenu)
{
    QMenu* settingsMenu = new QMenu(tr("Settings"), this);
    optionMenu->addMenu(settingsMenu);

    auto clearAction = settingsMenu->addAction(tr("Clear Settings"));
    connect(clearAction, &QAction::triggered, this, [=]() {
        xApp->settings()->setValue(Application::SettingsKey().clearSettings, true);
        tryToReboot();
    });

    auto openAction = settingsMenu->addAction(tr("Open Settings Directory"));
    connect(openAction, &QAction::triggered, this, [=]() {
        QDesktopServices::openUrl(xApp->settingsPath());
    });
}

void MainWindow::initOptionMenuHdpiPolicy(QMenu* optionMenu)
{
    QMenu* menu = optionMenu->addMenu(tr("HDPI Policy"));
    QActionGroup* actionGroup = new QActionGroup(this);

    typedef Qt::HighDpiScaleFactorRoundingPolicy Policy;
    QMap<Policy, QString> policyMap;
    policyMap.insert(Policy::Unset, QObject::tr("System"));
    policyMap.insert(Policy::Round, QObject::tr("Round up for .5 and above"));
    policyMap.insert(Policy::Ceil, QObject::tr("Always round up"));
    policyMap.insert(Policy::Floor, QObject::tr("Always round down"));
    policyMap.insert(Policy::RoundPreferFloor, QObject::tr("Round up for .75 and above"));
    policyMap.insert(Policy::PassThrough, QObject::tr("Don't round"));

    for (auto it = policyMap.begin(); it != policyMap.end(); ++it) {
        QString name = it.value();
        int policy = static_cast<int>(it.key());
        auto action = menu->addAction(name, this, [=]() {
            Application::SettingsKey settingKeys;
            xApp->settings()->setValue(settingKeys.hdpi, policy);
            tryToReboot();
        });

        actionGroup->addAction(action);
        action->setCheckable(true);
        if (policy == static_cast<int>(xApp->highDpiScaleFactorRoundingPolicy())) {
            action->setChecked(true);
        }
    }

    menu->addActions(actionGroup->actions());
    optionMenu->addMenu(menu);
}

void MainWindow::initOptionMenuColorScheme(QMenu* optionMenu)
{
#if xEnableColorScheme
    static QActionGroup* actionGroup = Q_NULLPTR;
    if (actionGroup) {
        return;
    }

    actionGroup = new QActionGroup(this);
    auto colorSchemeMenu = optionMenu->addMenu(tr("Color Scheme"));
    QMap<Qt::ColorScheme, QString> colorSchemeMap;
    colorSchemeMap.insert(Qt::ColorScheme::Dark, tr("Dark"));
    colorSchemeMap.insert(Qt::ColorScheme::Light, tr("Light"));
    colorSchemeMap.insert(Qt::ColorScheme::Unknown, tr("System"));

    for (auto it = colorSchemeMap.begin(); it != colorSchemeMap.end(); ++it) {
        auto action = new QAction(it.value(), this);
        action->setCheckable(true);
        actionGroup->addAction(action);
        colorSchemeMenu->addAction(action);

        if (it.key() == xApp->styleHints()->colorScheme()) {
            action->setChecked(true);
        }

        Qt::ColorScheme colorScheme = it.key();
        connect(action, &QAction::triggered, this, [=]() {
            Application::SettingsKey keys;
            qInfo() << colorScheme;
            xApp->settings()->setValue(keys.colorScheme, static_cast<int>(colorScheme));
            xApp->setupColorScheme();
        });
    }
#else
    Q_UNUSED(optionMenu);
#endif
}

void MainWindow::initMenuLanguage()
{
    QMap<QString, QString> languageFlagNameMap;
    languageFlagNameMap.insert("zh_CN", "简体中文");
    languageFlagNameMap.insert("en", "English");
#if 0
    languageFlagNameMap.insert("zh_TW", "繁體中文");
    languageFlagNameMap.insert("ar", "العربية");
    languageFlagNameMap.insert("cs", "Čeština");
    languageFlagNameMap.insert("da", "Dansk");
    languageFlagNameMap.insert("de", "Deutsch");
    languageFlagNameMap.insert("es", "Español");
    languageFlagNameMap.insert("fa", "فارسی");
    languageFlagNameMap.insert("fi", "Suomi");
    languageFlagNameMap.insert("fr", "Français");
    languageFlagNameMap.insert("he", "עִבְרִית");
    languageFlagNameMap.insert("uk", "українська мова");
    languageFlagNameMap.insert("it", "Italiano");
    languageFlagNameMap.insert("ja", "日本语");
    languageFlagNameMap.insert("ko", "한글");
    languageFlagNameMap.insert("lt", "Lietuvių kalba");
    languageFlagNameMap.insert("pl", "Polski");
    languageFlagNameMap.insert("pt", "Português");
    languageFlagNameMap.insert("ru", "русский язык");
    languageFlagNameMap.insert("sk", "Slovenčina");
    languageFlagNameMap.insert("sl", "Slovenščina");
    languageFlagNameMap.insert("sv", "Svenska");
#endif

    QMenu* languageMenu = menuBar()->addMenu(tr("&Languages"));
    static auto languageActionGroup = new QActionGroup(this);
    for (auto it = languageFlagNameMap.begin(); it != languageFlagNameMap.end(); ++it) {
        auto* action = new QAction(it.value(), this);
        action->setCheckable(true);
        languageMenu->addAction(action);
        languageActionGroup->addAction(action);

        Application::SettingsKey keys;
        connect(action, &QAction::triggered, this, [=]() {
            xApp->settings()->setValue(keys.language, it.key());
            tryToReboot();
        });

        QString defaultLanguage = QLocale::system().name();
        QString language = xApp->settings()->value(keys.language, defaultLanguage).toString();
        if (language == it.key()) {
            action->setChecked(true);
        }
    }
}

void MainWindow::initViewMenu()
{
    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->setObjectName("ViewMenu");
    QList<QAction*> actions = m_page4x4->actions();
    for (auto& action : actions) {
        viewMenu->addAction(action);
    }
    QAction* action = viewMenu->addSeparator();
    action->setObjectName("PageViewAction");
    initViewMenuStayOnTop(viewMenu);
}

void MainWindow::initViewMenuStayOnTop(QMenu* viewMenu)
{
    QAction* action = viewMenu->addAction(tr("Stays on Top"));
    connect(action, &QAction::triggered, this, [=]() {
        if (action->isChecked()) {
            setWindowFlag(Qt::WindowStaysOnTopHint, true);
        } else {
            setWindowFlag(Qt::WindowStaysOnTopHint, false);
        }

        xApp->settings()->setValue(m_settingsKey.staysOnTop, action->isChecked());
        show();
    });
    action->setCheckable(true);

    bool staysOnTop = xApp->settings()->value(m_settingsKey.staysOnTop, false).toBool();
    action->setChecked(staysOnTop);
}

void MainWindow::initHelpMenu()
{
    auto helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);
    auto aboutAction = helpMenu->addAction(tr("About") + " " + QApplication::applicationName());
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAboutActionTriggered);

#if defined(QT_DEBUG)
    helpMenu->addAction(tr("Screenshot"), this, [=]() {
        QPixmap pix = this->grab();
        // copy to clipboard
        QApplication::clipboard()->setPixmap(pix);
    });
#endif

#if defined(Q_OS_WIN)
    helpMenu->addSeparator();
    helpMenu->addAction(QIcon(":/res/icons/buy.svg"), tr("Buy from Store"), this, [=]() {
        QDesktopServices::openUrl(storeUrl());
    });
#endif

    helpMenu->addSeparator();
    helpMenu->addAction(tr("Release History"), this, &MainWindow::showHistory);
    helpMenu->addAction(tr("Join in QQ Group"), this, &MainWindow::showQrCode);
    helpMenu->addSeparator();

    helpMenu->addAction(tr("Online Manual"), this, []() {
        QDesktopServices::openUrl(QUrl("https://x-tools-author.github.io/x-tools"));
    });
    helpMenu->addAction(tr("Get Sources from Github"), this, []() {
        QDesktopServices::openUrl(QUrl("https://github.com/x-tools-author/x-tools"));
    });
    helpMenu->addAction(tr("Get Sources from Gitee"), this, []() {
        QDesktopServices::openUrl(QUrl("https://gitee.com/x-tools-author/x-tools"));
    });
    helpMenu->addSeparator();

    // clang-format off
    QList<QPair<QString, QString>> ctxs;
    ctxs.append(qMakePair(QString("glog"), QString("https://github.com/google/glog")));
    ctxs.append(qMakePair(QString("QXlsx"), QString("https://github.com/QtExcel/QXlsx")));
    ctxs.append(qMakePair(QString("libqrencode"), QString("https://github.com/fukuchi/libqrencode")));
    ctxs.append(qMakePair(QString("qmdnsengine"), QString("https://github.com/nitroshare/qmdnsengine")));
    ctxs.append(qMakePair(QString("Qt-Advanced-Stylesheets"), QString("https://github.com/githubuser0xFFFF/Qt-Advanced-Stylesheets")));
    // clang-format on

    QMenu* menu = helpMenu->addMenu(tr("Third Party Open Source"));
    for (auto& ctx : ctxs) {
        menu->addAction(ctx.first, this, [ctx]() { QDesktopServices::openUrl(QUrl(ctx.second)); });
    }
}

void MainWindow::showHistory()
{
    QDialog dialog(this);
    dialog.setModal(true);
    dialog.setWindowTitle(tr("Release History"));
    dialog.resize(600, 400);

    auto* textBrowser = new QTextBrowser(&dialog);
    QFile file(":/res/files/history.txt");
    if (file.open(QFile::ReadOnly)) {
        QByteArray data = file.readAll();
        textBrowser->setText(QString::fromUtf8(data));
    }

    auto* layout = new QHBoxLayout(&dialog);
    layout->addWidget(textBrowser);
    dialog.setLayout(layout);
    dialog.show();
    dialog.exec();
}

void MainWindow::showQrCode()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("QR Code"));

    struct QrCodeInfo
    {
        QString title;
        QString qrCode;
    };
    QList<QrCodeInfo> qrCodeInfoList;

    qrCodeInfoList << QrCodeInfo{tr("User QQ Group"), QString(":/res/images/x_tools_qq.jpg")}
                   << QrCodeInfo{tr("Qt QQ Group"), QString(":/res/images/qt_qq.jpg")};

    auto* tabWidget = new QTabWidget(&dialog);
    for (auto& var : qrCodeInfoList) {
        auto* label = new QLabel(tabWidget);
        QPixmap pix = QPixmap::fromImage(QImage(var.qrCode));
        pix = pix.scaledToHeight(600, Qt::SmoothTransformation);
        label->setPixmap(pix);
        tabWidget->addTab(label, var.title);
    }

    auto* layout = new QHBoxLayout(&dialog);
    layout->addWidget(tabWidget);
    dialog.setLayout(layout);
    dialog.setModal(true);
    dialog.show();
    dialog.exec();
}

void MainWindow::tryToReboot()
{
    QString title = tr("Need to Reboot");
    QString text = tr("The operation need to reboot to effected, reboot the application now?");
    int ret = QMessageBox::information(nullptr, title, text, QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
        xApp->execMs(100);
        qApp->closeAllWindows();
    }
}

void MainWindow::setupPages()
{
    m_page4x4 = new Page4x4(xApp->settings(), this);
    addPage(tr("General"), m_page4x4, ":/res/icons/device_hub.svg", tr("General Device"));

    auto* mqttClient = new MqttClientUi(this);
    addPage(tr("MQTT Client"), mqttClient, ":/res/icons/mqtt.svg", tr("MQTT Client"));
    auto* mqttServer = new MqttServerUi(this);
    addPage(tr("MQTT Server"), mqttServer, ":/res/icons/mqtt.svg", tr("MQTT Server"));
}

void MainWindow::addPage(const QString& name,
                         QWidget* page,
                         const QString& iconName,
                         const QString& tooltip)
{
    m_stackedWidget->addWidget(page);
    QAction* action = m_toolBar->addAction(name, page, [=]() {
        m_stackedWidget->setCurrentWidget(page);
    });

    action->setCheckable(true);
    action->setIcon(QIcon(iconName));
    action->setToolTip(tooltip);
    m_toolBarActionGroup->addAction(action);

    int pageIndex = xApp->settings()->value(m_settingsKey.pageIndex, 0).toInt();
    if (pageIndex == m_stackedWidget->count() - 1) {
        action->setChecked(true);
        m_stackedWidget->setCurrentWidget(page);
    } else {
        action->setChecked(false);
    }
}

QString MainWindow::qtConfFileName()
{
    return QCoreApplication::applicationDirPath() + "/qt.conf";
}

void MainWindow::createQtConf()
{
    QString fileName = qtConfFileName();
    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
        QTextStream out(&file);
        out << "[Platforms]\nWindowsArguments = dpiawareness=0\n";
        file.close();
    } else {
        auto info = QString("Open file(%1) failed: %2").arg(fileName, file.errorString());
        qWarning() << qPrintable(info);
    }
}

void MainWindow::onSaveActionTriggered() const
{
    save();
}

void MainWindow::onImportActionTriggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import Parameters"),
                                                    "",
                                                    tr("Json Files (*.json)"));
    if (fileName.isEmpty()) {
        return;
    }

    load(fileName);
}

void MainWindow::onExportActionTriggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export Parameters"),
                                                    "",
                                                    tr("Json Files (*.json)"));

    if (fileName.isEmpty()) {
        return;
    }

    save(fileName);
}

void MainWindow::onAboutActionTriggered()
{
    QString buildDateTimeFormat = systemDateFormat();
    buildDateTimeFormat += " ";
    buildDateTimeFormat += systemTimeFormat();
    QString dtString = buildDateTimeString(buildDateTimeFormat);
    QString year = buildDateTimeString("yyyy");

    // X_TOOLS_GIT_COMMIT_TIME(Tue Mar 4 11:33:30 2025 +0800) to QDateTime
    // X_TOOLS_GIT_COMMIT_TIME is defined in CMakeLists.txt
#if defined(X_TOOLS_GIT_COMMIT_TIME)
    QString commitTime = QString(X_TOOLS_GIT_COMMIT_TIME);
    commitTime = commitTime.left(commitTime.lastIndexOf(" "));
    QDateTime commitDateTime = QDateTime::fromString(commitTime, "ddd MMM d hh:mm:ss yyyy");
    commitTime = commitDateTime.toString(buildDateTimeFormat);
#else
    QString commitTime = tr("Unknown");
#endif

    QString info;
    info += qApp->applicationName() + " v" + qApp->applicationVersion() + "\n\n";
#ifdef X_TOOLS_GIT_COMMIT
    info += tr("Commit hash") + ": " + X_TOOLS_GIT_COMMIT + "\n\n";
#endif
    info += tr("Commit time") + ": " + commitTime + "\n\n";
    info += tr("Build Date") + ": " + dtString + "\n\n";
    info += tr("Copyright") + QString(" 2018-%1 x-tools-author(x-tools@outlook.com).\n").arg(year);
    info += tr("All rights reserved.");
    QMessageBox::about(this, tr("About"), info);
}
