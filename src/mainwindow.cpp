/***************************************************************************************************
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
#include "common/xtools.h"
#include "layoutmanager.h"
#include "page/page.h"
#include "tools/assistantfactory.h"
#include "utilities/hdpimanager.h"
#include "utilities/i18n.h"
#include "utilities/stylemanager.h"

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

    const QString page00{"page00"};
    const QString page01{"page01"};
    const QString page10{"page10"};
    const QString page11{"page11"};
    const QString layoutManager{"layoutManager"};
};

MainWindow::MainWindow(QWidget* parent)
    : xUi(parent)
    , m_ioPage00(Q_NULLPTR)
    , m_ioPage01(Q_NULLPTR)
    , m_ioPage10(Q_NULLPTR)
    , m_ioPage11(Q_NULLPTR)
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
        connect(systemTrayIcon, &SystemTrayIcon::invokeShowMainWindow, this, &MainWindow::show);
        systemTrayIcon->show();
    }
#endif

    winId();
    QSettings* settings = Application::settings();
    xAPP->showSplashScreenMessage(QString("Create page 1..."));
    m_ioPage00 = new Page(Page::Left, settings, this);
    xAPP->showSplashScreenMessage(QString("Create page 2..."));
    m_ioPage01 = new Page(Page::Right, settings, this);
    xAPP->showSplashScreenMessage(QString("Create page 3..."));
    m_ioPage10 = new Page(Page::Left, settings, this);
    xAPP->showSplashScreenMessage(QString("Create page 4..."));
    m_ioPage11 = new Page(Page::Right, settings, this);

    xAPP->showSplashScreenMessage(QString("Create main window..."));
    QWidget* ioLayoutWidget = new QWidget();
    auto* layout = new QGridLayout(ioLayoutWidget);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_ioPage00, 0, 0);
    layout->addWidget(m_ioPage01, 0, 1);
    layout->addWidget(m_ioPage10, 1, 0);
    layout->addWidget(m_ioPage11, 1, 1);
    ioLayoutWidget->setLayout(layout);

    auto* centralWidget = new QWidget();
    QStackedLayout* sl = new QStackedLayout(centralWidget);
    sl->setContentsMargins(0, 0, 0, 0);
    centralWidget->setLayout(sl);
    setCentralWidget(centralWidget);

    const int defaultGrid = static_cast<int>(WindowGrid::Grid1x1);
    const QString key = m_settingsKey.windowGrid;
    int rawGrid = settings->value(key, defaultGrid).toInt();
    m_windowGrid = static_cast<WindowGrid>(rawGrid);
    updateGrid(m_windowGrid);
    qInfo() << "The value of window grid is:" << static_cast<int>(m_windowGrid);

    xAPP->showSplashScreenMessage(QString("Create menu bar of main window..."));
    setWindowIcon(QIcon(":/res/icons/logo.svg"));
    setWindowTitle(qApp->applicationName() + " v" + qApp->applicationVersion());
    setObjectName("MainWindow");

    // Initialize layout manager
    m_layoutManager = new LayoutManager(sl, menuBar(), this);
    QToolButton* button = m_layoutManager->addLayoutPage(QString("xTools"), ioLayoutWidget);
    m_layoutManager->setupPages();
    button->setChecked(true);

    connect(&xI18n.singleton(), &xTools::I18n::languageChanged, this, [=]() { tryToReboot(); });
    connect(&xStyleMgr.singleton(), &xTools::StyleManager::styleChanged, this, [=]() {
        tryToReboot();
    });
    connect(&xHdpiMgr.singleton(), &xTools::HdpiManager::hdpiChanged, this, [=]() {
        tryToReboot();
    });

    // ---------------------------------------------------------------------------------------------
    initMenuBar();
    connect(sl, &QStackedLayout::currentChanged, this, [=](int index) {
        this->m_viewMenu->menuAction()->setEnabled(index == 0);
    });
}

MainWindow::~MainWindow() {}

void MainWindow::load(const QString& fileName)
{
    QString filePath = fileName;
    if (fileName.isEmpty()) {
        const QString path = xAPP->settingsPath();
        filePath = path + "/data.json";
    }

    if (!QFile::exists(filePath)) {
        m_layoutManager->load(QJsonObject());
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    MainWindowParameterKeys keys;
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    m_ioPage00->load(obj.value(keys.page00).toObject().toVariantMap());
    m_ioPage01->load(obj.value(keys.page01).toObject().toVariantMap());
    m_ioPage10->load(obj.value(keys.page10).toObject().toVariantMap());
    m_ioPage11->load(obj.value(keys.page11).toObject().toVariantMap());
    m_layoutManager->load(obj.value(keys.layoutManager).toObject());

    bool showMax = obj.value(keys.showMax).toBool(false);
    if (showMax) {
        showMaximized();
        move(QPoint(0, 0));
    }

    int index = m_layoutManager->currentIndex();
    m_viewMenu->menuAction()->setEnabled(index == 0);
}

void MainWindow::save(const QString& fileName) const
{
    QJsonObject obj;
    MainWindowParameterKeys keys;
    obj.insert(keys.page00, QJsonObject::fromVariantMap(m_ioPage00->save()));
    obj.insert(keys.page01, QJsonObject::fromVariantMap(m_ioPage01->save()));
    obj.insert(keys.page10, QJsonObject::fromVariantMap(m_ioPage10->save()));
    obj.insert(keys.page11, QJsonObject::fromVariantMap(m_ioPage11->save()));
    obj.insert(keys.layoutManager, m_layoutManager->save());
    obj.insert(keys.showMax, isMaximized());

    QJsonDocument doc;
    doc.setObject(obj);

    QString filePath = fileName;
    if (fileName.isEmpty()) {
        const QString path = xAPP->settingsPath();
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

void MainWindow::updateGrid(WindowGrid grid)
{
    if (grid == WindowGrid::Grid1x2) {
        m_ioPage00->show();
        m_ioPage01->show();
        m_ioPage10->hide();
        m_ioPage11->hide();
    } else if (grid == WindowGrid::Grid2x1) {
        m_ioPage00->show();
        m_ioPage01->hide();
        m_ioPage10->show();
        m_ioPage11->hide();
    } else if (grid == WindowGrid::Grid2x2) {
        m_ioPage00->show();
        m_ioPage01->show();
        m_ioPage10->show();
        m_ioPage11->show();
    } else {
        m_ioPage00->show();
        m_ioPage01->hide();
        m_ioPage10->hide();
        m_ioPage11->hide();
    }

    m_windowGrid = grid;
    xAPP->settings()->setValue(m_settingsKey.windowGrid, static_cast<int>(grid));
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

void MainWindow::hideHistoryAction()
{
    if (m_historyAction) {
        m_historyAction->setVisible(false);
    }
}

void MainWindow::showLiteMode()
{
    m_ioPage00->showLiteMode();
    m_ioPage01->showLiteMode();
    m_ioPage10->showLiteMode();
    m_ioPage11->showLiteMode();

    QWidget* cornerWidget = menuBar()->cornerWidget(Qt::TopRightCorner);
    if (cornerWidget) {
        cornerWidget->hide();
    }
    m_layoutManager->setCurrentIndex(0);
}

void MainWindow::outputBytes(const QString& txt, int channel)
{
    m_layoutManager->outputBytes(txt, channel);
}

void MainWindow::clearOutput(int channel)
{
    m_layoutManager->clearOutput(channel);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    m_ioPage00->aboutToClose();
    m_ioPage01->aboutToClose();
    m_ioPage10->aboutToClose();
    m_ioPage11->aboutToClose();

    save();

    QSettings* settings = xAPP->settings();
    settings->setValue(m_settingsKey.windowGrid, static_cast<int>(m_windowGrid));

#ifdef Q_OS_WIN
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
    return QUrl("https://apps.microsoft.com/detail/9p29h1ndnkbb");
}

void MainWindow::initMenuBar()
{
    xAPP->showSplashScreenMessage(QString("Create menus..."));
    initFileMenu();
#if !defined(Q_OS_ANDROID)
    initToolMenu();
#endif
    initViewMenu();
    initHelpMenu();
}

void MainWindow::initFileMenu()
{
    QList<QAction*> existingActions = m_fileMenu->actions();
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    QMenu* newMenu = m_fileMenu->addMenu(tr("New Window"));
    m_fileMenu->addMenu(newMenu);
    newMenu->addAction(QString("xTools"), this, []() {
        auto* w = new Page(Page::Left, xAPP->settings());
        w->setWindowTitle("xTools");
        w->show();
    });
    const QList<QAction*> actions = m_layoutManager->newWindowActions();
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

void MainWindow::initViewMenu()
{
    m_viewMenu = new QMenu(tr("&View"), this);
    menuBar()->insertMenu(m_languageMenu->menuAction(), m_viewMenu);
    m_viewMenu->setObjectName("ViewMenu");
    initViewMenuGrid(m_viewMenu);
    QAction* action = m_viewMenu->addSeparator();
    action->setObjectName("PageViewAction");
}

void MainWindow::initViewMenuGrid(QMenu* viewMenu)
{
    static QActionGroup* group = new QActionGroup(this);
    if (!group->actions().isEmpty()) {
        return;
    }

    auto a1x1 = viewMenu->addAction("1x1", this, [=]() { updateGrid(WindowGrid::Grid1x1); });
    auto a1x2 = viewMenu->addAction("1x2", this, [=]() { updateGrid(WindowGrid::Grid1x2); });
    auto a2x1 = viewMenu->addAction("2x1", this, [=]() { updateGrid(WindowGrid::Grid2x1); });
    auto a2x2 = viewMenu->addAction("2x2", this, [=]() { updateGrid(WindowGrid::Grid2x2); });

    a1x1->setObjectName("PageViewAction");
    a1x2->setObjectName("PageViewAction");
    a2x1->setObjectName("PageViewAction");
    a2x2->setObjectName("PageViewAction");

    a1x1->setCheckable(true);
    a1x2->setCheckable(true);
    a2x1->setCheckable(true);
    a2x2->setCheckable(true);

    group->addAction(a1x1);
    group->addAction(a1x2);
    group->addAction(a2x1);
    group->addAction(a2x2);

    int defaultGrid = static_cast<int>(WindowGrid::Grid1x1);
    int windowGrid = xAPP->settings()->value(m_settingsKey.windowGrid, defaultGrid).toInt();
    m_windowGrid = static_cast<WindowGrid>(windowGrid);
    updateGrid(m_windowGrid);
    if (windowGrid == static_cast<int>(WindowGrid::Grid1x2)) {
        a1x2->setChecked(true);
    } else if (windowGrid == static_cast<int>(WindowGrid::Grid2x1)) {
        a2x1->setChecked(true);
    } else if (windowGrid == static_cast<int>(WindowGrid::Grid2x2)) {
        a2x2->setChecked(true);
    } else {
        a1x1->setChecked(true);
    }
}

void MainWindow::initHelpMenu()
{
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

void MainWindow::tryToReboot(bool doNotReboot)
{
    QString title = tr("Need to Reboot");
    QString text = tr("The operation need to reboot to effected, reboot the application now?");
    if (doNotReboot) {
        text += tr("(Please reboot your application manually.)");
    }

    int ret = QMessageBox::information(nullptr, title, text, QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        if (!doNotReboot) {
            QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
        }

        xAPP->execMs(100);
        qApp->closeAllWindows();
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
