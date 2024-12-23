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
#include <QNetworkProxyFactory>
#include <QPainter>
#include <QPixmap>
#include <QScrollBar>
#include <QTextBrowser>
#include <QVariant>

#include "AssistantFactory.h"
#include "Common/xTools.h"
#include "IOPage/IOPage.h"

#ifdef Q_OS_WIN
#include "SystemTrayIcon.h"
#endif

MainWindow::MainWindow(QWidget* parent)
    : xTools::MainWindow(parent)
    , m_toolMenu(nullptr)
{
    xTools::xTools& xTools = xTools::xTools::singleton();
    m_ioPage00 = new IOPage(IOPage::Left, xTools.settings(), this);
    m_ioPage01 = new IOPage(IOPage::Right, xTools.settings(), this);
    m_ioPage10 = new IOPage(IOPage::Left, xTools.settings(), this);
    m_ioPage11 = new IOPage(IOPage::Right, xTools.settings(), this);
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

    auto* centralWidget = new QWidget();
    auto* layout = new QGridLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_ioPage00, 0, 0);
    layout->addWidget(m_ioPage01, 0, 1);
    layout->addWidget(m_ioPage10, 1, 0);
    layout->addWidget(m_ioPage11, 1, 1);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    setWindowIcon(QIcon(":/resources/Images/Logo.png"));
    initMenuBar();

    constexpr int defaultGrid = static_cast<int>(WindowGrid::Grid1x1);
    const QString key = m_settingsKey.windowGrid;
    int rawGrid = xTools.settings()->value(key, defaultGrid).toInt();
    m_windowGrid = static_cast<WindowGrid>(rawGrid);
    qInfo() << "The value of window grid is:" << static_cast<int>(m_windowGrid);
    updateGrid(m_windowGrid);

    load();
}

MainWindow::~MainWindow()
{
    qInfo() << "MainWindow is destroyed!";
}

void MainWindow::initMenuBar()
{
    initFileMenu();
    initToolMenu();
    initOptionMenu();
    initViewMenu();
    initLinksMenu();
    initHelpMenu();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    save();

    g_xTools.settings()->setValue(m_settingsKey.windowGrid, static_cast<int>(m_windowGrid));
#ifdef Q_OS_WIN
    if (g_xTools.settings()->value(m_settingsKey.exitToSystemTray).toBool()) {
        hide();
        event->ignore();
        return;
    }
#endif
    QMainWindow::closeEvent(event);
}

void MainWindow::initFileMenu()
{
    auto newWindowAction = m_fileMenu->addAction(tr("New Window"), this, []() {
        auto* w = new IOPage(IOPage::Left, g_xTools.settings());
        w->setWindowTitle("xTools");
        w->show();
    });
    newWindowAction->setShortcut(QKeySequence::New);

    m_fileMenu->addSeparator();
    auto saveAction = m_fileMenu->addAction(tr("Save Parameters"),
                                            this,
                                            &MainWindow::onSaveActionTriggered);
    saveAction->setShortcut(QKeySequence::Save);

    auto importActioni = m_fileMenu->addAction(tr("Import Parameters"),
                                               this,
                                               &MainWindow::onImportActionTriggered);
    importActioni->setShortcut(QKeySequence::Open);

    auto exportAction = m_fileMenu->addAction(tr("Export Parameters"),
                                              this,
                                              &MainWindow::onExportActionTriggered);
    exportAction->setShortcuts(QKeySequence::SaveAs);

    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);
}

void MainWindow::initToolMenu()
{
    m_toolMenu = new QMenu(tr("&Tools"));
    menuBar()->insertMenu(m_languageMenu->menuAction(), m_toolMenu);

    QList<int> supportedAssistants = AssistantFactory::instance()->supportedAssistants();
    QMenu* newMenu = m_toolMenu->addMenu(tr("New"));
    m_toolMenu->addSeparator();

    for (auto& type : supportedAssistants) {
        QString name = AssistantFactory::instance()->assistantName(type);
        auto* action = new QAction(name, this);
        QWidget* assistant = AssistantFactory::instance()->newAssistant(type);

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
    m_optionMenu->addSeparator();
    auto* proxy = m_optionMenu->addAction(tr("Use System Proxy"));
    proxy->setCheckable(true);
    bool useSystemProxy = g_xTools.settings()->value(m_settingsKey.useSystemProxy).toBool();
    proxy->setChecked(useSystemProxy);
    QNetworkProxyFactory::setUseSystemConfiguration(proxy->isChecked());
    connect(proxy, &QAction::triggered, this, [=]() {
        QNetworkProxyFactory::setUseSystemConfiguration(proxy->isChecked());
        g_xTools.settings()->setValue(m_settingsKey.useSystemProxy, proxy->isChecked());
    });

    auto* trayAction = new QAction(tr("Exit to System Tray"), this);
    trayAction->setCheckable(true);
    m_optionMenu->addAction(trayAction);

    QVariant v = g_xTools.settings()->value(m_settingsKey.exitToSystemTray);
    if (!v.isNull()) {
        bool isExitToSystemTray = v.toBool();
        trayAction->setChecked(isExitToSystemTray);
    }

    connect(trayAction, &QAction::triggered, this, [=]() {
        bool keep = trayAction->isChecked();
        g_xTools.settings()->setValue(m_settingsKey.exitToSystemTray, keep);
    });
}

void MainWindow::initViewMenu()
{
    auto* viewMenu = new QMenu(tr("&View"));
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

    auto windowGrid = g_xTools.settings()->value(m_settingsKey.windowGrid).toInt();
    if (windowGrid == static_cast<int>(WindowGrid::Grid1x2)) {
        a1x2->setChecked(true);
        a1x2->trigger();
    } else if (windowGrid == static_cast<int>(WindowGrid::Grid2x1)) {
        a2x1->setChecked(true);
        a2x1->trigger();
    } else if (windowGrid == static_cast<int>(WindowGrid::Grid2x2)) {
        a2x2->setChecked(true);
        a2x2->trigger();
    } else {
        a1x1->setChecked(true);
        a1x1->trigger();
    }
}

void MainWindow::initHelpMenu()
{
#if defined(Q_OS_WIN) && !defined(X_TOOLS_ENABLE_MODULE_PRIVATE)
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(QIcon(":/resources/Icons/IconBuy.svg"), tr("Bug from Store"), this, []() {
        QUrl url("https://www.microsoft.com/store/apps/9P29H1NDNKBB");
        QDesktopServices::openUrl(url);
    });
#endif
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(tr("Release History"), this, &MainWindow::showHistory);
    m_helpMenu->addAction(tr("Join in QQ Group"), this, &MainWindow::showQrCode);
    m_helpMenu->addSeparator();
    // clang-format off
    QList<QPair<QString, QString>> ctxs;
    ctxs.append(qMakePair("glog", "https://github.com/google/glog"));
    ctxs.append(qMakePair("hidapi", "https://github.com/libusb/hidapi"));
    ctxs.append(qMakePair("libqrencode", "https://github.com/libusb/hidapi"));
    ctxs.append(qMakePair("qmdnsengine", "https://github.com/nitroshare/qmdnsengine"));
    ctxs.append(qMakePair("Qt-Advanced-Stylesheets", "https://github.com/githubuser0xFFFF/Qt-Advanced-Stylesheets"));
    ctxs.append(qMakePair("QXlsx", "https://github.com/QtExcel/QXlsx"));
    QMenu* thirdPartyMenu = m_helpMenu->addMenu(tr("Third Party Open Source"));
    for (auto& ctx : ctxs) {
        thirdPartyMenu->addAction(ctx.first, this, [ctx]() {
            QDesktopServices::openUrl(QUrl(ctx.second));
        });
    }
    m_helpMenu->addMenu(thirdPartyMenu);
    // clang-format on
}

void MainWindow::initLinksMenu()
{
    auto* linksMenu = new QMenu(tr("Links"), this);
    menuBar()->insertMenu(m_helpMenu->menuAction(), linksMenu);

    linksMenu->addAction(tr("Get Sources from Github"), this, []() {
        QDesktopServices::openUrl(QUrl(X_TOOLS_GITHUB_REPOSITORY_URL));
    });
    linksMenu->addAction(tr("Get Sources from Gitee"), this, []() {
        QDesktopServices::openUrl(QUrl(X_TOOLS_GITEE_REPOSITORY_URL));
    });
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
    g_xTools.settings()->setValue(m_settingsKey.windowGrid, static_cast<int>(grid));
}

void MainWindow::showHistory()
{
    QDialog dialog;
    dialog.setModal(true);
    dialog.setWindowTitle(tr("Release History"));
    dialog.resize(600, 400);

    auto* textBrowser = new QTextBrowser(&dialog);
    QFile file(":/resources/Files/History.txt");
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

    qrCodeInfoList << QrCodeInfo{tr("User QQ Group"), QString(":/resources/Images/QSAKQQ.jpg")}
                   << QrCodeInfo{tr("Qt QQ Group"), QString(":/resources/Images/QtQQ.jpg")};

    auto* tabWidget = new QTabWidget(&dialog);
    for (auto& var : qrCodeInfoList) {
        auto* label = new QLabel(tabWidget);
        QPixmap pix = QPixmap::fromImage(QImage(var.qrCode));
        pix = pix.scaledToWidth(400, Qt::SmoothTransformation);
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

void MainWindow::load(const QString& fileName) const
{
    QString filePath = fileName;
    if (fileName.isEmpty()) {
        const QString path = g_xTools.settingsPath();
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
    m_ioPage00->load(obj.value("page00").toObject().toVariantMap());
    m_ioPage01->load(obj.value("page01").toObject().toVariantMap());
    m_ioPage10->load(obj.value("page10").toObject().toVariantMap());
    m_ioPage11->load(obj.value("page11").toObject().toVariantMap());
}

void MainWindow::save(const QString& fileName) const
{
    QJsonObject obj;
    obj.insert("page00", QJsonObject::fromVariantMap(m_ioPage00->save()));
    obj.insert("page01", QJsonObject::fromVariantMap(m_ioPage01->save()));
    obj.insert("page10", QJsonObject::fromVariantMap(m_ioPage10->save()));
    obj.insert("page11", QJsonObject::fromVariantMap(m_ioPage11->save()));

    QJsonDocument doc;
    doc.setObject(obj);

    QString filePath = fileName;
    if (fileName.isEmpty()) {
        const QString path = g_xTools.settingsPath();
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
