#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "TCPClientWidget.h"
#include "TCPServerWidget.h"

#include "SerialportSAKIODeviceWidget.h"
#include "UdpSAKIODeviceWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mpTabWidget(new QTabWidget)
{
    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(mpTabWidget);
    ui->centralWidget->setLayout(layout);

    this->resize(1280, 720);

    AddTab();
    InitMenu();

    setWindowTitle(tr("瑞士军刀--开发调试工具集"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddTab()
{
    /// 串口页
    SerialportSAKIODevice *serialportDevice = new SerialportSAKIODevice;
    this->mpTabWidget->addTab(new SerialportSAKIODeviceWidget(serialportDevice, new SerialportSAKIODeviceControler), tr("串口助手"));
    serialportDevice->start();

    /// udp客户端
    UdpSAKIODevice *udpDevice = new UdpSAKIODevice;
    this->mpTabWidget->addTab(new UdpSAKIODeviceWidget(udpDevice, new UdpSAKIODeviceControler), tr("UDP客户端"));
    udpDevice->start();

    /// tcp 客户端
    this->mpTabWidget->addTab(new TcpClientWidget, tr("TCP客户端"));
    /// tcp 服务器
    this->mpTabWidget->addTab(new TcpServerWidget, tr("TCP服务器"));
    /// 终端
    this->mpTabWidget->addTab(new Console, tr("终端"));
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

    // 帮助菜单
    QMenu *pHelpMenu = new QMenu(tr("帮助"));
    menuBar()->addMenu(pHelpMenu);

    QAction *pAboutQtAction = new QAction(tr("关于Qt"));
    pHelpMenu->addAction(pAboutQtAction);
    connect(pAboutQtAction, SIGNAL(triggered(bool)), this, SLOT(AboutQt()));

    QAction *pAboutAction = new QAction(tr("关于软件"));
    pHelpMenu->addAction(pAboutAction);
    connect(pAboutAction, SIGNAL(triggered(bool)), this, SLOT(About()));
}

void MainWindow::About()
{
    QMessageBox::about(this, tr("关于软件"), QString("%1: %2\n"
                                                 "%3: %4\n"
                                                 "%5: %6\n"
                                                 "%7: %8\n"
                                                 "%9: %10")
                       .arg(tr("软件版本")).arg("1.0.0")
                       .arg(tr("编译时间")).arg(QString(__DATE__) + " " + QString(__TIME__))
                       .arg(tr("软件作者")).arg(tr("不想敲代码的程序员"))
                       .arg(tr("联系邮箱")).arg("wh19931117@126.com")
                       .arg(tr("软件定做")).arg(tr("软件定做，质量高，价格低，详情联系wh19931117@126.com")));

}

void MainWindow::AboutQt()
{
    QMessageBox::aboutQt(this, tr("关于Qt"));
}
