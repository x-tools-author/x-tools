#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <QDateTime>
#include <QApplication>
#include <QStyle>

#include "GetPublicIPWidget.h"
#include "ui_GetPublicIPWidget.h"

GetPublicIPWidget::GetPublicIPWidget(QWidget *parent):
    QWidget(parent),
    clearOutputInfoTimer(new QTimer),
    getPublicIpThread(new GetPublicIpThread),
    ui(new Ui::GetPublicIPWidget)
{
    ui->setupUi(this);
    ui->pushButtonInfoIcon->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
    ui->pushButtonInfoIcon->setIconSize(QSize(ui->pushButtonInfoIcon->width(), ui->pushButtonInfoIcon->height()));

    connect(getPublicIpThread, SIGNAL(publicIPChanged(QString)), this, SLOT(setText(QString)));
    connect(ui->pushButtonGet, SIGNAL(clicked(bool)), getPublicIpThread, SLOT(getPublicIP()));
    connect(ui->pushButtonCopy, SIGNAL(clicked(bool)), this, SLOT(copyIPAddress()));
    connect(clearOutputInfoTimer, SIGNAL(timeout()), this, SLOT(clearOutputInfo()));

    getPublicIpThread->start();
}

GetPublicIPWidget::~GetPublicIPWidget()
{

}

void GetPublicIPWidget::outputInfo(QString info, QString color)
{
    QString dateTimeString = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ");
    dateTimeString = QString("<font color=silver>%1</font>").arg(dateTimeString);

    ui->labelInfo->setText(QString("%1<font color=%2>%3</font>").arg(dateTimeString).arg(color).arg(info));

    const int inventor = 3;
    if (clearOutputInfoTimer->isActive()){
        clearOutputInfoTimer->stop();
        clearOutputInfoTimer->start(inventor*1000);
    }else {
        clearOutputInfoTimer->start(inventor*1000);
    }
}

void GetPublicIPWidget::clearOutputInfo()
{
    ui->labelInfo->clear();
}

void GetPublicIPWidget::setText(QString ipAddress)
{
    if (ipAddress.isEmpty()){
        outputInfo(tr("无法获取公网IP，可能网址已失效！"), QString("red"));
    }else {
        ui->lineEditPublicIP->setText(ipAddress);
        outputInfo(tr("已获取公网IP！"));
    }
}

void GetPublicIPWidget::copyIPAddress()
{
    ui->lineEditPublicIP->selectAll();
    ui->lineEditPublicIP->copy();

    outputInfo(tr("输入框内容已复制到系统剪贴板！"));
}
