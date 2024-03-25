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
#include <QActionGroup>
#include <QButtonGroup>
#include <QClipboard>
#include <QCloseEvent>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QJsonParseError>
#include <QLocale>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QProcess>
#include <QScrollBar>
#include <QSizePolicy>
#include <QStatusBar>
#include <QStyleFactory>
#include <QSysInfo>
#include <QTextBrowser>
#include <QToolBar>
#include <QToolButton>
#include <QVariant>

#include "xToolsAssistantFactory.h"
#include "xToolsInterface.h"
#include "xToolsSettings.h"
#include "xToolsToolBoxUi.h"
#include "xToolsUiInterface.h"
#ifdef X_TOOLS_IMPORT_MODULE_CANBUS_STUDIO
#include "xToolsCanBusStudioUi.h"
#endif
#ifdef X_TOOLS_IMPORT_MODULE_MODBUS_STUDIO
#include "xToolsModbusStudioUi.h"
#endif

#ifdef Q_OS_WIN
#include "SystemTrayIcon.h"
#endif

#define SAK_QT_CONF (qApp->applicationDirPath() + "/qt.conf")

QString palettePath()
{
    QString fileName = xToolsSettings::instance()->fileName();
    QUrl url(fileName);
    QString path = fileName;
    QString logPath = path.remove(url.fileName());
    logPath += "palette";
    return logPath;
}

MainWindow::MainWindow(QWidget* parent)
    : xToolsMainWindow(parent)
{
#ifdef Q_OS_WIN
    // Setup system tray icon.
    auto systemTrayIcon = new SystemTrayIcon(this);
    QObject::connect(systemTrayIcon, &SystemTrayIcon::invokeExit, this, &MainWindow::close);
    QObject::connect(systemTrayIcon, &SystemTrayIcon::invokeShowMainWindow, this, &MainWindow::show);
#endif

    QStackedWidget* stackedWidget = new QStackedWidget();
    setCentralWidget(stackedWidget);

    QDir dir;
    if (dir.exists(palettePath())) {
        qInfo() << "The palette path is:" << palettePath();
    } else {
        if (dir.mkdir(palettePath())) {
            qInfo() << palettePath() << "has been created";
        } else {
            qWarning() << palettePath() << "has been created";
        }
    }

#if 0
#ifdef Q_OS_ANDROID
    setWindowFlags(Qt::FramelessWindowHint);
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);
    scrollArea->setWidget(mTabWidget);
#endif
#endif

    QString title = QString("xTools");
#ifndef X_TOOLS_IMPORT_MODULE_PRIVATE
    title.append(tr("(Community)"));
#endif
    title.append(QString(" "));
    title.append(QString("v"));
    title.append(qApp->applicationVersion());
#ifndef X_TOOLS_IMPORT_MODULE_PRIVATE
    title.append(" ");
    title.append("Beta1");
#endif
    setWindowTitle(title);
    setWindowIcon(QIcon(":/resources/images/SAKLogo.png"));

    initMenuBar();
    initNav();
    initStatusBar();
}

MainWindow::~MainWindow() {}

void MainWindow::initMenuBar()
{
    menuBar()->setPalette(qApp->palette());

    initFileMenu();
    initToolMenu();
    initOptionMenu();
    initLanguageMenu();
    initLinksMenu();
    initHelpMenu();
}

#ifdef Q_OS_WIN
void MainWindow::closeEvent(QCloseEvent* event)
{
    auto key = m_settingsKey.exitToSystemTray;
    bool ignore = xToolsSettings::instance()->value(key).toBool();
    if (ignore) {
        this->hide();
        event->ignore();
    }
}
#endif

void MainWindow::initFileMenu()
{
    // Tool box
    QMenu* windowMenu = new QMenu(tr("New Window"), this);
    m_fileMenu->addMenu(windowMenu);
    QList<int> toolTypeList = xToolsToolBoxUi::supportedCommunicationTools();
    for (auto& toolType : toolTypeList) {
        const QString name = xToolsToolBoxUi::communicationToolName(toolType);
        QAction* action = new QAction(name, this);
        windowMenu->addAction(action);
        connect(action, &QAction::triggered, this, [=]() {
            xToolsToolBoxUi* w = new xToolsToolBoxUi();
            w->setContentsMargins(9, 9, 9, 9);
            w->setAttribute(Qt::WA_DeleteOnClose, true);
            w->initialize(toolType);
            w->show();
        });
    }

    // Other tools
#ifdef X_TOOLS_IMPORT_MODULE_MODBUS_STUDIO
    QAction* modbusAction = new QAction("Modbus Studio", this);
    connect(modbusAction, &QAction::triggered, this, [=]() {
        xToolsModbusStudioUi* w = new xToolsModbusStudioUi();
        w->setContentsMargins(9, 9, 9, 9);
        w->setAttribute(Qt::WA_DeleteOnClose, true);
        w->resize(1024, 480);
        w->show();
    });
    windowMenu->addAction(modbusAction);
#endif

#ifdef X_TOOLS_IMPORT_MODULE_CANBUS_STUDIO
    QAction* canbusAction = new QAction("CANBus Studio", this);
    connect(canbusAction, &QAction::triggered, this, [=]() {
        xToolsModbusStudioUi* w = new xToolsModbusStudioUi();
        w->setContentsMargins(9, 9, 9, 9);
        w->setAttribute(Qt::WA_DeleteOnClose, true);
        w->resize(1024, 480);
        w->show();
    });
    windowMenu->addAction(canbusAction);
#endif

    m_fileMenu->addSeparator();
    QAction* importAction = new QAction(tr("Import Palette"), m_fileMenu);
    m_fileMenu->addAction(importAction);
    connect(importAction, &QAction::triggered, this, &MainWindow::onImportActionTriggered);

    QAction* exportAction = new QAction(tr("Export Palette"), m_fileMenu);
    m_fileMenu->addAction(exportAction);
    connect(exportAction, &QAction::triggered, this, &MainWindow::onExportActionTriggered);

    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);
}

void MainWindow::initToolMenu()
{
    QMenu* toolMenu = new QMenu(tr("&Tools"));
    menuBar()->insertMenu(m_languageMenu->menuAction(), toolMenu);

    for (auto& type : SAKAssistantsFactory::instance()->supportedAssistants()) {
        QString name = SAKAssistantsFactory::instance()->assistantName(type);
        QAction* action = new QAction(name, this);
        QWidget* assistant = SAKAssistantsFactory::instance()->newAssistant(type);

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
    }
}

void MainWindow::initOptionMenu()
{
    QMenu* mainWindowMenu = new QMenu(tr("Main Window"), this);
    QAction* action = new QAction(tr("Exit to Sysytem Tray"), this);
    action->setCheckable(true);
    mainWindowMenu->addAction(action);
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

void MainWindow::initLanguageMenu() {}

void MainWindow::initHelpMenu()
{
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(QIcon(":/Resources/Icons/GitHub.svg"),
                          tr("Get Sources from Github"),
                          this,
                          []() { QDesktopServices::openUrl(QUrl(X_TOOLS_GITHUB_REPOSITORY_URL)); });
    m_helpMenu->addAction(QIcon(":/Resources/Icons/Gitee.svg"),
                          tr("Get Sources from Gitee"),
                          this,
                          []() { QDesktopServices::openUrl(QUrl(X_TOOLS_GITEE_REPOSITORY_URL)); });
    m_helpMenu->addSeparator();
#if 0
    m_helpMenu->addAction(tr("About xTools"), this, &MainWindow::aboutSoftware);
#endif
#ifndef X_TOOLS_BUILD_FOR_STORE
#ifdef Q_OS_WIN
    m_helpMenu->addAction(QIcon(":/Resources/Icons/IconBuy.svg"),
                          tr("Buy from Microsoft App Store"),
                          this,
                          []() {
                              QUrl url("https://www.microsoft.com/store/apps/9P29H1NDNKBB");
                              QDesktopServices::openUrl(url);
                          });
#endif
#endif
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(QIcon(":/Resources/Icons/GitHub.svg"),
                          tr("Release History"),
                          this,
                          &MainWindow::showHistory);
    m_helpMenu->addAction(QIcon(":/Resources/Icons/GitHub.svg"),
                          tr("Join in QQ Group"),
                          this,
                          &MainWindow::showQrCode);
}

void MainWindow::initLinksMenu()
{
    QMenu* linksMenu = new QMenu(tr("&Links"), this);
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
             << Link{tr("Download xTools from Github"),
                     QString("%1/releases").arg(X_TOOLS_GITHUB_REPOSITORY_URL),
                     QString(":/resources/images/GitHub.png")}
             << Link{tr("Download xTools from Gitee"),
                     QString("%1/releases").arg(X_TOOLS_GITEE_REPOSITORY_URL),
                     QString(":/Resources/Icons/IconQQ.svg")}
             << Link{tr("Office Web Site"),
                     QString("https://qsaker.gitee.io/qsak/"),
                     QString(":/Resources/Icons/IconQQ.svg")};

    for (auto& var : linkList) {
        QAction* action = new QAction(QIcon(var.iconPath), var.name, this);
        action->setObjectName(var.url);
        linksMenu->addAction(action);

        connect(action, &QAction::triggered, this, [=]() {
            QDesktopServices::openUrl(QUrl(sender()->objectName()));
        });
    }
}

void MainWindow::initNav()
{
    QToolBar* tb = new QToolBar(this);
    addToolBar(Qt::LeftToolBarArea, tb);
    tb->setFloatable(false);
    tb->setMovable(false);
    tb->setOrientation(Qt::Vertical);
    tb->setAllowedAreas(Qt::LeftToolBarArea);

    static QButtonGroup navButtonGroup;
    QList<int> types = xToolsToolBoxUi::supportedCommunicationTools();
    for (int i = 0; i < types.count(); i++) {
        int type = types.at(i);
        xToolsToolBoxUi* toolBoxUi = new xToolsToolBoxUi(this);
        toolBoxUi->initialize(type);

        initNav({&navButtonGroup,
                 xToolsUiInterface::cookedIcon(toolBoxUi->windowIcon()),
                 toolBoxUi->windowTitle(),
                 toolBoxUi,
                 tb});
    }

    tb->addSeparator();

    QString path = ":/Resources/Icons/IconModbus.svg";
#ifdef X_TOOLS_IMPORT_MODULE_MODBUS_STUDIO
    xToolsModbusStudioUi* modbus = new xToolsModbusStudioUi(this);
    initNav(
        {&navButtonGroup, xToolsUiInterface::cookedIcon(QIcon(path)), "Modbus Studio", modbus, tb});
#endif
#ifdef X_TOOLS_IMPORT_MODULE_CANBUS_STUDIO
    xToolsCanBusStudioUi* canbus = new xToolsCanBusStudioUi(this);
    path = ":/Resources/Icons/IconCanBus.svg";
    initNav(
        {&navButtonGroup, xToolsUiInterface::cookedIcon(QIcon(path)), "CANBus Studio", canbus, tb});
#endif
    QLabel* lb = new QLabel(" ");
    tb->addWidget(lb);
    lb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

#if 1
    tb->addSeparator();
    bool isTextBesideIcon = xToolsSettings::instance()->isTextBesideIcon();
    auto style = isTextBesideIcon ? Qt::ToolButtonTextBesideIcon : Qt::ToolButtonIconOnly;
    QToolButton* tbt = new QToolButton(this);
    path = ":/Resources/Icons/IconListWithIcon.svg";
    tbt->setIcon(xToolsUiInterface::cookedIcon(QIcon(path)));
    tbt->setCheckable(true);
    tbt->setText(" " + tr("Hide Text"));
    tbt->setToolTip(tr("Click to show(hide) nav text"));
    tbt->setAutoRaise(true);
    tbt->setChecked(isTextBesideIcon);
    tbt->setToolButtonStyle(style);
    tbt->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    tb->addWidget(tbt);
    auto bg = &navButtonGroup;
    connect(tbt, &QToolButton::clicked, tbt, [=]() {
        auto bts = bg->buttons();
        auto style = tbt->isChecked() ? Qt::ToolButtonTextBesideIcon : Qt::ToolButtonIconOnly;
        tbt->setToolButtonStyle(style);
        for (auto& bt : bts) {
            auto cookedBt = qobject_cast<QToolButton*>(bt);

            cookedBt->setToolButtonStyle(style);
        }
        xToolsSettings::instance()->setIsTextBesideIcon(tbt->isChecked());
    });
    tb->addSeparator();
#endif
}

void MainWindow::initNav(const NavContext& ctx)
{
    bool isTextBesideIcon = xToolsSettings::instance()->isTextBesideIcon();
    auto style = isTextBesideIcon ? Qt::ToolButtonTextBesideIcon : Qt::ToolButtonIconOnly;
    QToolButton* bt = new QToolButton();
    bt->setAutoRaise(true);
    bt->setIcon(ctx.icon);
    bt->setCheckable(true);
    bt->setToolButtonStyle(style);
    bt->setToolTip(ctx.name);
    bt->setText(" " + ctx.name);
    bt->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
#if 0
    bt->setIconSize(QSize(32, 32));
#endif
    ctx.bg->addButton(bt);
    ctx.tb->addWidget(bt);

    if (ctx.page->layout()) {
        ctx.page->layout()->setContentsMargins(0, 0, 0, 0);
    }
    auto stackedWidget = qobject_cast<QStackedWidget*>(centralWidget());
    stackedWidget->addWidget(ctx.page);

    int pageCount = ctx.bg->buttons().count();
    QObject::connect(bt, &QToolButton::clicked, bt, [=]() {
        stackedWidget->setCurrentIndex(pageCount - 1);
        xToolsSettings::instance()->setPageIndex(pageCount - 1);
    });

    if (xToolsSettings::instance()->pageIndex() == (pageCount - 1)) {
        bt->setChecked(true);
        stackedWidget->setCurrentIndex(pageCount - 1);
    }
}

void MainWindow::initStatusBar()
{
    statusBar()->showMessage("Hello world", 10 * 1000);
}

void MainWindow::aboutSoftware()
{
    struct Info
    {
        QString name;
        QString value;
        bool valueIsUrl;
    };

    QString format = QLocale::system().dateFormat();
    format = format + " " + QLocale::system().timeFormat();
    QString dateTimeString = xToolsInterface::buildDateTime(format);
    QList<Info> infoList;
    infoList << Info{tr("Version"), QString(qApp->applicationVersion()), false}
#ifndef SAK_RELEASE_FOR_APP_STORE
             << Info{tr("Edition"), SAK_EDITION, false}
#endif
             << Info{tr("Author"), QString(X_TOOLS_AUTHOR), false}
             << Info{tr("Email"), QString(X_TOOLS_AUTHOR_EMAIL), false}
             << Info{tr("QQ"), QString("QQ:2869470394"), false}
             << Info{tr("QQ Group"), QString("QQ:952218522"), false}
             << Info{tr("Build Time"), dateTimeString, false}
#ifndef SAK_RELEASE_FOR_APP_STORE
             << Info{tr("Gitee Url"),
                     QString("<a href=%1>%1</a>").arg(X_TOOLS_GITEE_REPOSITORY_URL),
                     true}
             << Info{tr("Gitbub Url"),
                     QString("<a href=%1>%1</a>").arg(X_TOOLS_GITHUB_REPOSITORY_URL),
                     true}
#endif
             << Info{tr("Copyright"),
                     tr("Copyright 2018-%1 x-tools-author(x-tools@outlook.com)."
                        " All rights reserved.")
                         .arg(xToolsInterface::buildDateTime("yyyy")),
                     false};

    QDialog dialog(this);
    dialog.setWindowTitle(tr("About QSAK"));

    QGridLayout* gridLayout = new QGridLayout(&dialog);
    int i = 0;
    for (auto& var : infoList) {
        QLabel* nameLabel = new QLabel(QString("<font color=green>%1</font>").arg(var.name),
                                       &dialog);
        QLabel* valueLabel = new QLabel(var.value, &dialog);
        gridLayout->addWidget(nameLabel, i, 0, 1, 1);
        gridLayout->addWidget(valueLabel, i, 1, 1, 1);
        i += 1;

        if (var.valueIsUrl) {
            connect(valueLabel, &QLabel::linkActivated, [](QString url) {
                QDesktopServices::openUrl(QUrl(url));
            });
        }
    }
    dialog.setLayout(gridLayout);
    dialog.setModal(true);
    dialog.show();
    dialog.exec();
}

void MainWindow::clearConfiguration()
{
    xToolsSettings::instance()->setClearSettings(true);
    rebootRequestion();
}

void MainWindow::rebootRequestion()
{
    int ret = QMessageBox::information(this,
                                       tr("Reboot application to effective"),
                                       tr("Need to reboot, reboot to effective now?"),
                                       QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        QProcess::startDetached(QCoreApplication::applicationFilePath());

        qApp->closeAllWindows();
        qApp->exit();
    }
}

void MainWindow::showHistory()
{
    QDialog dialog;
    dialog.setModal(true);
    dialog.setWindowTitle(tr("Release History"));
    dialog.resize(600, 400);

    QTextBrowser* textBrowser = new QTextBrowser(&dialog);
    QFile file(":/resources/files/History.txt");
    if (file.open(QFile::ReadOnly)) {
        QByteArray data = file.readAll();
        textBrowser->setText(QString::fromUtf8(data));
    }

    QHBoxLayout* layout = new QHBoxLayout(&dialog);
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

    QTabWidget* tabWidget = new QTabWidget(&dialog);
    for (auto& var : qrCodeInfoList) {
        QLabel* label = new QLabel(tabWidget);
        label->setPixmap(QPixmap::fromImage(QImage(var.qrCode)));
        tabWidget->addTab(label, var.title);
    }

    QHBoxLayout* layout = new QHBoxLayout(&dialog);
    layout->addWidget(tabWidget);
    dialog.setLayout(layout);
    dialog.setModal(true);
    dialog.show();
    dialog.exec();
}

void MainWindow::showDonation()
{
    QDialog dialog(this);
    dialog.setModal(true);
    QHBoxLayout* hBoxLayout = new QHBoxLayout(&dialog);
    QString image = ":/resources/images/WeChat.jpg";
    QLabel* label = new QLabel(&dialog);
    QPixmap pixMap = QPixmap::fromImage(QImage(image));
    label->setPixmap(pixMap.scaledToHeight(400, Qt::SmoothTransformation));
    hBoxLayout->addWidget(label);
    dialog.layout()->addWidget(label);
    dialog.show();
    dialog.exec();
}

void MainWindow::createQtConf()
{
    QString fileName = SAK_QT_CONF;
    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
        QTextStream out(&file);
        out << "[Platforms]\nWindowsArguments = dpiawareness=0\n";
        file.close();
    } else {
        qWarning() << fileName;
        qWarning() << "can not open file:" << file.errorString();
    }
}

void MainWindow::onImportActionTriggered()
{
    auto str = QFileDialog::getOpenFileName(this, tr("Save Palette"), "Palete", tr("All (*)"));
    if (str.isEmpty()) {
        qInfo() << "cancle to import the palette";
    } else {
        QFile inFile(str);
        if (inFile.open(QFile::ReadOnly)) {
            QByteArray bytes = inFile.readAll();
            inFile.close();

            QUrl url(str);
            QString fn = url.fileName();
            QString path = palettePath();
            QString outFileName = path + "/" + fn;
            if (QFile::exists(outFileName)) {
                QMessageBox::warning(this,
                                     tr("File Exists"),
                                     tr("The file is exists, "
                                        "import operaion failed"));
                return;
            }

            QFile outFile(outFileName);
            if (outFile.open(QFile::WriteOnly)) {
                QDataStream out(&outFile);
                out << bytes;
                outFile.close();
            } else {
                qWarning() << "open out file failed:" << inFile.errorString();
            }
        } else {
            qWarning() << "open in file failed:" << inFile.errorString();
        }
    }
}

void MainWindow::onExportActionTriggered()
{
    auto str = QFileDialog::getSaveFileName(this, tr("Save Palette"), "Palete", tr("All (*)"));
    if (str.isEmpty()) {
        qInfo() << "cancle to export the palette";
    } else {
        QFile file(str);
        if (file.open(QFile::WriteOnly)) {
            QPalette p = qApp->palette();
            QDataStream out(&file);
            out << p;
            file.close();
        } else {
            qWarning() << "can not open file:" << file.errorString();
        }
    }
}
