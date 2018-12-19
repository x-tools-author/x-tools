#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "UdpSAKIODeviceWidget.h"
#include "TcpSAKIODeviceWidget.h"
#include "TcpServerSAKIODeviceWidget.h"
#include "SAKSettings.h"
#include "nslookup.h"
#include "GetPublicIPWidget.h"
#include "UpdateDialog.h"
#include "SAKVersion.h"

#ifndef SAK_NO_SERIALPORT_ASSISTANT
#include "SerialportSAKIODeviceWidget.h"
#endif

#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mpTabWidget(new QTabWidget),
    ui(new Ui::MainWindow),
    updateDialog(new UpdateDialog),
    versionDialog(new SAKVersion)
{
    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(mpTabWidget);
    ui->centralWidget->setLayout(layout);
    setWindowTitle(tr("瑞士军刀--开发调试工具集"));

    this->resize(800, 600);
    this->setMinimumWidth(1024);

    AddTab();
    InitMenu();
    AddTool();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddTab()
{
#ifndef SAK_NO_SERIALPORT_ASSISTANT
    /// 串口页(不支持winrt、树莓派...)
    SerialportSAKIODevice *serialportDevice = new SerialportSAKIODevice;
    this->mpTabWidget->addTab(new SerialportSAKIODeviceWidget(serialportDevice, new SerialportSAKIODeviceControler), tr("串口助手"));
    serialportDevice->start();
#endif

    /// udp客户端
    UdpSAKIODevice *udpDevice = new UdpSAKIODevice;
    this->mpTabWidget->addTab(new UdpSAKIODeviceWidget(udpDevice, new UdpSAKIODeviceControler), tr("UDP客户端"));
    udpDevice->start();

    /// tcp客户端
    TcpSAKIODevice *tcpDevice = new TcpSAKIODevice;
    this->mpTabWidget->addTab(new TcpSAKIODeviceWidget(tcpDevice, new TcpSAKIODeviceControler), tr("TCP客户端"));
    tcpDevice->start();

    /// TCP 服务器
    TcpServerSAKIODevice *tcpServerDevice = new TcpServerSAKIODevice;
    this->mpTabWidget->addTab(new TcpServerSAKIODeviceWidget(tcpServerDevice, new TcpServerSAKIODeviceControler), tr("Tcp服务器"));
    tcpServerDevice->start();

#ifdef QT_DEBUG
    /// 终端
    this->mpTabWidget->addTab(new Console, tr("终端"));
#endif
}

void MainWindow::AddTool()
{
    addTool(tr("域名转IP"), new ToolsNsLookup);
    addTool(tr("公网IP获取工具"), new GetPublicIPWidget);
}

void MainWindow::InitMenu()
{
    /// 文件菜单
    QMenu *pFileMenu = new QMenu(tr("文件"));
    menuBar()->addMenu(pFileMenu);

    QAction *pExitAction = new QAction(tr("退出"));
    pFileMenu->addAction(pExitAction);
    connect(pExitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    /// 工具菜单
    QMenu *pToolMenu = new QMenu(tr("工具"));
    menuBar()->addMenu(pToolMenu);
    toolsMenu = pToolMenu;

    /// 选项
    QMenu *optionMenu = new QMenu(tr("选项"));
    menuBar()->addMenu(optionMenu);
#if 1
    QMenu *appStyleMenu = new QMenu(tr("软件风格"));
    optionMenu->addMenu(appStyleMenu);
    QStringList styleKeys = QStyleFactory::keys();
    QAction *styleAction;
    QActionGroup *styleActionGroup = new QActionGroup(this);
    foreach (QString style, styleKeys) {
        styleAction = new QAction(style);
        styleAction->setCheckable(true);
        styleActionGroup->addAction(styleAction);
        styleAction->setObjectName(style);
        appStyleMenu->addAction(styleAction);
        connect(styleAction, SIGNAL(triggered(bool)), this, SLOT(styleActionTriggered()));

        if (style.compare(sakSettings()->valueApplicationStyle()) == 0){
            styleAction->setChecked(true);
        }
    }
#endif

    // 帮助菜单
    QMenu *pHelpMenu = new QMenu(tr("帮助"));
    menuBar()->addMenu(pHelpMenu);

    QAction *pAboutQtAction = new QAction(tr("关于Qt"));
    pHelpMenu->addAction(pAboutQtAction);
    connect(pAboutQtAction, SIGNAL(triggered(bool)), this, SLOT(AboutQt()));

    QAction *pAboutAction = new QAction(tr("关于软件"));
    pHelpMenu->addAction(pAboutAction);
    connect(pAboutAction, SIGNAL(triggered(bool)), this, SLOT(About()));

    QAction *pUpdateAction = new QAction(tr("检查更新"));
    pHelpMenu->addAction(pUpdateAction);
    connect(pUpdateAction, SIGNAL(triggered(bool)), updateDialog, SLOT(checkForUpdate()));
}

void MainWindow::About()
{
    versionDialog->show();
    versionDialog->activateWindow();
}

void MainWindow::AboutQt()
{
    QMessageBox::aboutQt(this, tr("关于Qt"));
}

void MainWindow::styleActionTriggered()
{
    SAKSettings *setting = sakSettings();
    if (setting == nullptr){
        qWarning() << tr("配置文件未初始化！");
    }else {
        setting->setValueApplicationStyle(sender()->objectName());
        QApplication::setStyle(QStyleFactory::create(QString("%1").arg(sender()->objectName())));
    }
}

void MainWindow::addTool(QString toolName, QWidget *toolWidget)
{
    QAction *action = new QAction(toolName);
    toolsMenu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), toolWidget, SLOT(show()));
}
