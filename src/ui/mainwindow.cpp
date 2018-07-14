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
