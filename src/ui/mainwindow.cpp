#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serialportwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mpTabWidget(new QTabWidget)
{
    ui->setupUi(this);

    /// 设置中央部件
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(mpTabWidget);
    ui->centralWidget->setLayout(layout);

    /// 设置窗口大小
    this->resize(1280, 720);

    /// 添加页
    this->AddTab();
    /// 初始化菜单
    InitMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddTab()
{
    /// 串口页
    this->mpTabWidget->addTab(new SerialPortWidget, tr("串口助手"));
}

void MainWindow::InitMenu()
{
    /// 文件菜单
    QMenu *pFileMenu = new QMenu(tr("文件"));
    menuBar()->addMenu(pFileMenu);

    QAction *pExitAction = new QAction(tr("退出"));
    pFileMenu->addAction(pExitAction);
    connect(pExitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
}
