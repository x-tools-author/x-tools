/***************************************************************************************************
 * Copyright 2018-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
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
#include <QDateTime>
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
#include <QPalette>
#include <QPixmap>
#include <QProcess>
#include <QScreen>
#include <QScrollBar>
#include <QStackedLayout>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleHints>
#include <QSvgRenderer>
#include <QTextBrowser>
#include <QVariant>
#include <QWidgetAction>

#include "application.h"
#include "common/x.h"
#include "page/page.h"
#include "tools/assistantfactory.h"
#include "utilities/hdpimanager.h"
#include "utilities/i18n.h"
#include "utilities/stylemanager.h"
#include "x/xmanager.h"

#ifdef Q_OS_WIN
#include "systemtrayicon.h"
#endif

#if defined(_MSC_VER)
#include <dwmapi.h>

#include <QColor>
#include <QSysInfo>
#include <QWindow>
#endif

struct MainWindowParameterKeys
{
    const QString showMax{"showMax"};
    const QString layoutManager{"layoutManager"};

    const QString exitToSystemTray{"MainWindow/exitToSystemTray"};
    const QString useSystemProxy{"MainWindow/useSystemProxy"};
    const QString staysOnTop{"MainWindow/staysOnTop"};
};

MainWindow::MainWindow(QWidget* parent)
    : xUi(parent)
{
#if defined(X_MAGIC)
    setWindowOpacity(0.4);
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
        connect(systemTrayIcon, &SystemTrayIcon::invokeShowMainWindow, this, [=]() {
            showNormal();
            activateWindow();
            raise();
        });
        systemTrayIcon->show();
    }
#endif

    winId();
    QSettings* settings = Application::settings();
    xAPP->showSplashScreenMessage(QString("Create main window..."));

    auto* centralWidget = new QWidget();
    QStackedLayout* sl = new QStackedLayout(centralWidget);
    sl->setContentsMargins(0, 0, 0, 0);
    centralWidget->setLayout(sl);
    setCentralWidget(centralWidget);

    xAPP->showSplashScreenMessage(QString("Create menu bar of main window..."));
    setWindowIcon(QIcon(":/res/icons/logo.svg"));
    setWindowTitle(qApp->applicationName() + " v" + qApp->applicationVersion());
    setObjectName("MainWindow");

    // Initialize layout manager
    m_xMgr = new xManager(sl, menuBar(), this);

    // ---------------------------------------------------------------------------------------------
    initMenuBar();
}

MainWindow::~MainWindow() {}

void MainWindow::load(const QString& fileName)
{
    QString filePath = fileName;
    if (fileName.isEmpty()) {
        filePath = defalutDataJsonFile();
    }

    if (!QFile::exists(filePath)) {
        qInfo() << "The file does not exist:" << filePath;
        m_xMgr->load(QJsonObject());
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file: " << filePath << file.errorString();
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    MainWindowParameterKeys keys;
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse json file: " << filePath << error.errorString();
    }

    QJsonObject obj = doc.object();
    m_xMgr->load(obj.value(keys.layoutManager).toObject());

    bool showMax = obj.value(keys.showMax).toBool(false);
    if (showMax) {
        showMaximized();
        move(QPoint(0, 0));
    }

    int index = m_xMgr->currentIndex();
}

void MainWindow::save(const QString& fileName) const
{
    QJsonObject obj;
    MainWindowParameterKeys keys;
    obj.insert(keys.layoutManager, m_xMgr->save());
    obj.insert(keys.showMax, isMaximized());

    QJsonDocument doc;
    doc.setObject(obj);

    QString filePath = fileName;
    if (fileName.isEmpty()) {
        filePath = defalutDataJsonFile();
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Failed to open file: " << filePath << file.errorString();
        return;
    }

    file.write(doc.toJson());
    file.close();
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

void MainWindow::showLiteMode()
{
    QWidget* cornerWidget = menuBar()->cornerWidget(Qt::TopRightCorner);
    if (cornerWidget) {
        cornerWidget->hide();
    }
    m_xMgr->setCurrentIndex(0);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    save();

    QSettings* settings = xAPP->settings();
#ifdef Q_OS_WIN
    MainWindowParameterKeys keys;
    if (settings->value(keys.exitToSystemTray).toBool()) {
        hide();
        event->accept();
        return;
    }
#endif

    QMainWindow::closeEvent(event);
}

QUrl MainWindow::storeUrl() const
{
    return QUrl("https://apps.microsoft.com/detail/9p29h1ndnkbb");
}

void MainWindow::initMenuBar()
{
    xAPP->showSplashScreenMessage(QString("Create menus..."));
    initFileMenu();
#if !defined(Q_OS_ANDROID)
    initToolMenu();
#endif
    initOptionMenu();
    initHelpMenu();
}

void MainWindow::initFileMenu()
{
    QList<QAction*> existingActions = m_fileMenu->actions();
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    QMenu* newMenu = m_fileMenu->addMenu(tr("New Window"));
    m_fileMenu->addMenu(newMenu);
    const QList<QAction*> actions = m_xMgr->newWindowActions();
    for (int i = 0; i < actions.count(); i++) {
        newMenu->addAction(actions.at(i));
    }
    m_fileMenu->addSeparator();
#endif
    QAction* action = m_fileMenu->addAction(tr("Save Parameters"),
                                            this,
                                            &MainWindow::onSaveActionTriggered);
    action->setShortcut(QKeySequence::Save);
    action = m_fileMenu->addAction(tr("Import Parameters"),
                                   this,
                                   &MainWindow::onImportActionTriggered);
    action->setShortcut(QKeySequence::Open);
    action = m_fileMenu->addAction(tr("Export Parameters"),
                                   this,
                                   &MainWindow::onExportActionTriggered);
    action->setShortcut(QKeySequence::SaveAs);
    // Add back existing actions
    for (int i = 0; i < existingActions.count(); i++) {
        m_fileMenu->addAction(existingActions.at(i));
    }
}

void MainWindow::initToolMenu()
{
    QMenuBar* mb = menuBar();
    auto toolMenu = new QMenu(tr("&Tools"));
    mb->insertMenu(m_optionMenu->menuAction(), toolMenu);

    QList<int> supportedAssistants = AssistantFactory::instance()->supportedAssistants();
    QMenu* newMenu = toolMenu->addMenu(tr("New"));
    toolMenu->addSeparator();

    for (auto& type : supportedAssistants) {
        QString name = AssistantFactory::instance()->assistantName(type);
        auto* action = new QAction(name, this);
        QWidget* assistant = AssistantFactory::instance()->newAssistant(type);
        if (!assistant) {
            continue;
        }

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
#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0)
    QAction* action = m_optionMenu->addAction(tr("Using System Proxy"));
    action->setCheckable(true);
    MainWindowParameterKeys keys;
    connect(action, &QAction::toggled, this, [=](bool checked) {
        xAPP->settings()->setValue(keys.useSystemProxy, checked);
        action->setChecked(checked);
        if (checked) {
            QNetworkProxyFactory::setUseSystemConfiguration(true);
        } else {
            QNetworkProxyFactory::setUseSystemConfiguration(false);
        }
    });
    const QString key = keys.useSystemProxy;
    bool defaultValue = QNetworkProxyFactory::usesSystemConfiguration();
    bool useSystemProxy = xAPP->settings()->value(key, defaultValue).toBool();
    action->setChecked(useSystemProxy);
    QNetworkProxyFactory::setUseSystemConfiguration(useSystemProxy);
#endif
}

void MainWindow::initHelpMenu()
{
    m_helpMenu->addAction(tr("Online Documentation"), this, []() {
        QDesktopServices::openUrl(QUrl("https://x-tools-author.github.io/x-tools/"));
    });
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(tr("Release History"), this, &MainWindow::showHistory);
    m_helpMenu->addAction(tr("Join in QQ Group"), this, &MainWindow::showQrCode);

    // clang-format off
    QList<QPair<QString, QString>> ctxs;
    ctxs.append(qMakePair(QString("lua"), QString("https://github.com/lua/lua")));
    ctxs.append(qMakePair(QString("glog"), QString("https://github.com/google/glog")));
    ctxs.append(qMakePair(QString("zint"), QString("https://github.com/zint/zint")));
    ctxs.append(qMakePair(QString("libiconv"), QString("https://ftp.gnu.org/pub/gnu/libiconv")));
    ctxs.append(qMakePair(QString("libqrencode"), QString("https://github.com/fukuchi/libqrencode")));
    ctxs.append(qMakePair(QString("qmdnsengine"), QString("https://github.com/nitroshare/qmdnsengine")));
    ctxs.append(qMakePair(QString("SingleApplication"), QString("https://github.com/itay-grudev/SingleApplication")));
    ctxs.append(qMakePair(QString("hidapi"), QString("https://github.com/libusb/hidapi")));
    ctxs.append(qMakePair(QString("mongoose"), QString("https://github.com/cesanta/mongoose")));
    ctxs.append(qMakePair(QString("usrsctp"), QString("https://github.com/sctplab/usrsctp")));
    ctxs.append(qMakePair(QString("simpleble"), QString("https://github.com/simpleble/simpleble")));
    ctxs.append(qMakePair(QString("libcoap"), QString("https://github.com/obgm/libcoap")));
    ctxs.append(qMakePair(QString("open62541"), QString("https://github.com/open62541/open62541")));
    ctxs.append(qMakePair(QString(""), QString("")));
    ctxs.append(qMakePair(QString("QXlsx"), QString("https://github.com/QtExcel/QXlsx")));
    ctxs.append(qMakePair(QString("nodeeditor"), QString("https://github.com/paceholder/nodeeditor")));
    ctxs.append(qMakePair(QString("QCustomPlot"), QString("https://www.qcustomplot.com/")));
    ctxs.append(qMakePair(QString("QCRC_Calc"), QString("https://github.com/KoynovStas/QCRC_Calc")));
    // clang-format on

    m_helpMenu->addSeparator();
    QMenu* menu = m_helpMenu->addMenu(tr("Third Party Open Source"));
    for (auto& ctx : ctxs) {
        if (ctx.first.isEmpty() && ctx.second.isEmpty()) {
            menu->addSeparator();
            continue;
        }
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
    for (const QrCodeInfo& var : const_cast<QList<QrCodeInfo>&>(qrCodeInfoList)) {
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

QString MainWindow::defalutDataJsonFile() const
{
    const QString path = xAPP->settingsPath();
    return path + "/data_v7_7.json";
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
    QString buildDateTimeFormat = xSystemDateFormat();
    buildDateTimeFormat += " ";
    buildDateTimeFormat += xSystemTimeFormat();
    QString dtString = xBuildDateTimeString(buildDateTimeFormat);
    QString year = xBuildDateTimeString("yyyy");

    // X_GIT_COMMIT_TIME(Tue Mar 4 11:33:30 2025 +0800) to QDateTime
    // X_GIT_COMMIT_TIME is defined in CMakeLists.txt
    QString commitTime;
#if defined(X_GIT_COMMIT_TIME)
    commitTime = QString(X_GIT_COMMIT_TIME);
    commitTime = commitTime.left(commitTime.lastIndexOf(" "));
    QDateTime commitDateTime = QDateTime::fromString(commitTime, "yyyy.MM.dd-hh:mm:ss");
    if (commitDateTime.isValid()) {
        commitTime = commitDateTime.toString(buildDateTimeFormat);
    }
#endif
    if (commitTime.isEmpty()) {
        commitTime = tr("Unknown");
    }

    QString info;
    info += qApp->applicationName() + " v" + qApp->applicationVersion() + "\n\n";
#ifdef X_GIT_COMMIT
    info += tr("Commit hash") + ": " + X_GIT_COMMIT + "\n\n";
#endif
    info += tr("Commit time") + ": " + commitTime + "\n\n";
    info += tr("Build Date") + ": " + dtString + "\n\n";
    info += tr("Copyright") + QString(" 2018-%1 x-tools-author(x-tools@outlook.com).\n").arg(year);
    info += tr("All rights reserved.");
    QMessageBox::about(this, tr("About") + " " + qApp->applicationName(), info);
}
