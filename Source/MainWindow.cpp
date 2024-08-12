/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "MainWindow.h"

#include <QAction>
#include <QButtonGroup>
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
#include <QPainter>
#include <QPixmap>
#include <QScrollBar>
#include <QSizePolicy>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTextBrowser>
#include <QToolBar>
#include <QToolButton>
#include <QVariant>

#ifdef X_TOOLS_ENABLE_MODULE_ASSISTANTS
#include "xToolsAssistantFactory.h"
#endif
#include "xToolsSettings.h"
#include "IOPage/IOPage.h"

#ifdef Q_OS_WIN
#include "SystemTrayIcon.h"
#endif

MainWindow::MainWindow(QWidget* parent)
#ifdef X_TOOLS_ENABLE_MODULE_PRIVATE
    : xToolsPrivateMainWindow(parent)
#else
    : xToolsMainWindow(parent)
#endif
    , m_ioPage00(new IOPage(IOPage::Left, xToolsSettings::instance(), this))
    , m_ioPage01(new IOPage(IOPage::Right, xToolsSettings::instance(), this))
    , m_ioPage10(new IOPage(IOPage::Left, xToolsSettings::instance(), this))
    , m_ioPage11(new IOPage(IOPage::Right, xToolsSettings::instance(), this))
{
#ifdef Q_OS_WIN
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        auto systemTrayIcon = new SystemTrayIcon(this);
        QObject::connect(systemTrayIcon, &SystemTrayIcon::invokeExit, this, [=]() {
            QApplication::closeAllWindows();
            QApplication::quit();
        });
        QObject::connect(systemTrayIcon,
                         &SystemTrayIcon::invokeShowMainWindow,
                         this,
                         &MainWindow::show);
        systemTrayIcon->show();
    }
#endif

    QWidget* centralWidget = new QWidget();
    QGridLayout* layout = new QGridLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_ioPage00, 0, 0);
    layout->addWidget(m_ioPage01, 0, 1);
    layout->addWidget(m_ioPage10, 1, 0);
    layout->addWidget(m_ioPage11, 1, 1);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    setWindowIcon(QIcon(":/Resources/Images/Logo.png"));
    initMenuBar();

    const int defaultGrid = static_cast<int>(WindowGrid::Grid1x1);
    const QString key = m_settingsKey.windowGrid;
    m_windowGrid = xToolsSettings::instance()->value(key, defaultGrid).value<WindowGrid>();
    updateGrid(m_windowGrid);

    load();
}

MainWindow::~MainWindow() {}

void MainWindow::initMenuBar()
{
    initFileMenu();
    initToolMenu();
    initOptionMenu();
    initViewMenu();
    initLanguageMenu();
    initLinksMenu();
    initHelpMenu();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    save();
#ifdef Q_OS_WIN
    if (xToolsSettings::instance()->value(m_settingsKey.exitToSystemTray).toBool()) {
        close();
        event->ignore();
        return;
    }
#endif
    xToolsSettings::instance()->setValue(m_settingsKey.windowGrid, static_cast<int>(m_windowGrid));
    QMainWindow::closeEvent(event);
}

void MainWindow::initFileMenu()
{
    // Tool box
    auto* windowMenu = new QMenu(tr("New Window"), this);
#if 0
    m_fileMenu->addMenu(windowMenu);
    QList<int> toolTypeList = xToolsToolBoxUi::supportedCommunicationTools();
    for (auto& toolType : toolTypeList) {
        const QString name = xToolsToolBoxUi::communicationToolName(toolType);
        auto* action = new QAction(name, this);
        windowMenu->addAction(action);
        connect(action, &QAction::triggered, this, [=]() {
            auto* w = new xToolsToolBoxUi();
            w->setContentsMargins(0, 0, 0, 0);
            w->setAttribute(Qt::WA_DeleteOnClose, true);
            w->initialize(toolType);
            w->adjustSize();
            w->show();
        });
    }
#endif

    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);
}

void MainWindow::initToolMenu()
{
#ifdef X_TOOLS_ENABLE_MODULE_ASSISTANTS
    m_toolMenu = new QMenu(tr("&Tools"));
    menuBar()->insertMenu(m_languageMenu->menuAction(), m_toolMenu);

    for (auto& type : SAKAssistantsFactory::instance()->supportedAssistants()) {
        QString name = SAKAssistantsFactory::instance()->assistantName(type);
        auto* action = new QAction(name, this);
        QWidget* assistant = SAKAssistantsFactory::instance()->newAssistant(type);

        Q_ASSERT_X(assistant, __FUNCTION__, "A null assistant widget!");

        assistant->hide();
        m_toolMenu->addAction(action);
        connect(action, &QAction::triggered, this, [=]() {
            if (assistant->isHidden()) {
                assistant->show();
            } else {
                assistant->activateWindow();
            }
        });
    }
#endif
}

void MainWindow::initOptionMenu()
{
    auto* mainWindowMenu = new QMenu(tr("Main Window"), this);
    auto* action = new QAction(tr("Exit to System Tray"), this);
    action->setCheckable(true);
    mainWindowMenu->addAction(action);
    m_optionMenu->addSeparator();
    m_optionMenu->addMenu(mainWindowMenu);

    QVariant v = xToolsSettings::instance()->value(m_settingsKey.exitToSystemTray);
    if (!v.isNull()) {
        bool isExitToSystemTray = v.toBool();
        action->setChecked(isExitToSystemTray);
    }

    connect(action, &QAction::triggered, this, [=]() {
        bool keep = action->isChecked();
        xToolsSettings::instance()->setValue(m_settingsKey.exitToSystemTray, keep);
    });
}

void MainWindow::initViewMenu()
{
    QMenu* viewMenu = new QMenu(tr("&View"));
    menuBar()->insertMenu(m_toolMenu->menuAction(), viewMenu);

    auto a1x1 = viewMenu->addAction("1x1", this, [=]() { updateGrid(WindowGrid::Grid1x1); });
    auto a1x2 = viewMenu->addAction("1x2", this, [=]() { updateGrid(WindowGrid::Grid1x2); });
    auto a2x1 = viewMenu->addAction("2x1", this, [=]() { updateGrid(WindowGrid::Grid2x1); });
    auto a2x2 = viewMenu->addAction("2x2", this, [=]() { updateGrid(WindowGrid::Grid2x2); });

    a1x1->setCheckable(true);
    a1x2->setCheckable(true);
    a2x1->setCheckable(true);
    a2x2->setCheckable(true);

    static QActionGroup group(this);
    group.addAction(a1x1);
    group.addAction(a1x2);
    group.addAction(a2x1);
    group.addAction(a2x2);

    if (m_windowGrid == WindowGrid::Grid1x1) {
        a1x1->setChecked(true);
    } else if (m_windowGrid == WindowGrid::Grid1x2) {
        a1x2->setChecked(true);
    } else if (m_windowGrid == WindowGrid::Grid2x1) {
        a2x1->setChecked(true);
    } else if (m_windowGrid == WindowGrid::Grid2x2) {
        a2x2->setChecked(true);
    }

    auto windowGrid = xToolsSettings::instance()->value(m_settingsKey.windowGrid).toInt();
    if (windowGrid == int(WindowGrid::Grid1x2)) {
        a1x2->setChecked(true);
        a1x2->trigger();
    } else if (windowGrid == int(WindowGrid::Grid2x1)) {
        a2x1->setChecked(true);
        a2x1->trigger();
    } else if (windowGrid == int(WindowGrid::Grid2x2)) {
        a2x2->setChecked(true);
        a2x2->trigger();
    } else {
        a1x1->setChecked(true);
        a1x1->trigger();
    }
}

void MainWindow::initLanguageMenu() {}

void MainWindow::initHelpMenu()
{
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(tr("Get Sources from Github"), this, []() {
        QDesktopServices::openUrl(QUrl(X_TOOLS_GITHUB_REPOSITORY_URL));
    });
    m_helpMenu->addAction(tr("Get Sources from Gitee"), this, []() {
        QDesktopServices::openUrl(QUrl(X_TOOLS_GITEE_REPOSITORY_URL));
    });
    m_helpMenu->addSeparator();
#if 0
    m_helpMenu->addAction(tr("About xTools"), this, &MainWindow::aboutSoftware);
#endif
#if defined(Q_OS_WIN) && !defined(X_TOOLS_ENABLE_MODULE_PRIVATE)
    m_helpMenu->addAction(QIcon(":/Resources/Icons/IconBuy.svg"),
                          tr("Buy Ultimate Edition"),
                          this,
                          []() {
                              QUrl url("https://www.microsoft.com/store/apps/9P29H1NDNKBB");
                              QDesktopServices::openUrl(url);
                          });
#endif
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(tr("Release History"), this, &MainWindow::showHistory);
    m_helpMenu->addAction(tr("Join in QQ Group"), this, &MainWindow::showQrCode);
}

void MainWindow::initLinksMenu()
{
    auto* linksMenu = new QMenu(tr("Links"), this);
    menuBar()->insertMenu(m_helpMenu->menuAction(), linksMenu);

    struct Link
    {
        QString name;
        QString url;
        QString iconPath;
    };
    QList<Link> linkList;
    linkList << Link{tr("Qt Official Download"),
                     QString("http://download.qt.io/official_releases/qt"),
                     QString(":/resources/images/Qt.png")}
             << Link{tr("Qt Official Blog"),
                     QString("https://www.qt.io/blog"),
                     QString(":/resources/images/Qt.png")}
             << Link{tr("Qt Official Release"),
                     QString("https://wiki.qt.io/Qt_5.15_Release"),
                     QString(":/resources/images/Qt.png")}
             << Link{QString(""), QString(""), QString("")}
             << Link{tr("Download xTools from Github"),
                     QString("%1/releases").arg(X_TOOLS_GITHUB_REPOSITORY_URL),
                     QString(":/Resources/Icons/GitHub.svg")}
             << Link{tr("Download xTools from Gitee"),
                     QString("%1/releases").arg(X_TOOLS_GITEE_REPOSITORY_URL),
                     QString(":/Resources/Icons/Gitee.svg")}
             << Link{QString(""), QString(""), QString("")}
             << Link{tr("Office Web Site"),
                     QString("https://qsaker.gitee.io/qsak/"),
                     QString(":/Resources/Images/I18n.png")};

    for (auto& var : linkList) {
        if (var.url.isEmpty()) {
            linksMenu->addSeparator();
            continue;
        }

        auto* action = new QAction(var.name, this);
        action->setObjectName(var.url);
        linksMenu->addAction(action);

        connect(action, &QAction::triggered, this, [=]() {
            QDesktopServices::openUrl(QUrl(sender()->objectName()));
        });
    }
}

void MainWindow::initNav()
{

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

    xToolsSettings::instance()->setValue(m_settingsKey.windowGrid, int(grid));
}

void MainWindow::showHistory()
{
    QDialog dialog;
    dialog.setModal(true);
    dialog.setWindowTitle(tr("Release History"));
    dialog.resize(600, 400);

    auto* textBrowser = new QTextBrowser(&dialog);
    QFile file(":/Resources/Files/History.txt");
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
    QDialog dialog;
    dialog.setWindowTitle(tr("QR Code"));

    struct QrCodeInfo
    {
        QString title;
        QString qrCode;
    };
    QList<QrCodeInfo> qrCodeInfoList;

    qrCodeInfoList << QrCodeInfo{tr("User QQ Group"), QString(":/Resources/Images/QSAKQQ.jpg")}
                   << QrCodeInfo{tr("Qt QQ Group"), QString(":/Resources/Images/QtQQ.jpg")};

    auto* tabWidget = new QTabWidget(&dialog);
    for (auto& var : qrCodeInfoList) {
        auto* label = new QLabel(tabWidget);
        label->setPixmap(QPixmap::fromImage(QImage(var.qrCode)));
        tabWidget->addTab(label, var.title);
    }

    auto* layout = new QHBoxLayout(&dialog);
    layout->addWidget(tabWidget);
    dialog.setLayout(layout);
    dialog.setModal(true);
    dialog.show();
    dialog.exec();
}

void MainWindow::load()
{
    const QString path = xToolsSettings::instance()->settingsPath();
    QString filePath = path + "/data.json";
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
    m_ioPage00->load(obj.value("page00").toObject().toVariantMap());
    m_ioPage01->load(obj.value("page01").toObject().toVariantMap());
    m_ioPage10->load(obj.value("page10").toObject().toVariantMap());
    m_ioPage11->load(obj.value("page11").toObject().toVariantMap());
}

void MainWindow::save()
{
    QJsonObject obj;
    obj.insert("page00", QJsonObject::fromVariantMap(m_ioPage00->save()));
    obj.insert("page01", QJsonObject::fromVariantMap(m_ioPage01->save()));
    obj.insert("page10", QJsonObject::fromVariantMap(m_ioPage10->save()));
    obj.insert("page11", QJsonObject::fromVariantMap(m_ioPage11->save()));

    QJsonDocument doc;
    doc.setObject(obj);

    const QString path = xToolsSettings::instance()->settingsPath();
    QString filePath = path + "/data.json";
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return;
    }

    QTextStream out(&file);
    out << doc.toJson();
    file.close();
}
