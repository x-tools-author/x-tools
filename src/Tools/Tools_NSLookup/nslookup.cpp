/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "nslookup.h"
#include "ui_nslookup.h"

ToolsNsLookup::ToolsNsLookup(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::ToolsNsLookup)
    ,hideErrorTimer(new QTimer(this))
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);
    Connect();
}

ToolsNsLookup::~ToolsNsLookup()
{

}

void ToolsNsLookup::Connect()
{
    connect(ui->pushButtonLookup, SIGNAL(clicked(bool)), this, SLOT(lookupHost()));
    connect(ui->pushButtonCopy, SIGNAL(clicked(bool)), this, SLOT(copyAddress()));
    connect(hideErrorTimer, SIGNAL(timeout()), this, SLOT(hideErrorTimerTimeout()));
}

void ToolsNsLookup::lookupHost()
{
    ui->lineEditAddress->clear();
    QString url = ui->lineEditUrl->text();
    if (url.isEmpty()){
        outputMsg(tr("请输入域名，如：www.baidu.com"));
    }else {
        QHostInfo::lookupHost(ui->lineEditUrl->text(), this, SLOT(lookedUpResult(QHostInfo)));
    }
}

void ToolsNsLookup::lookedUpResult(const QHostInfo &host)
{
    if (host.error() != QHostInfo::NoError) {
        QString errotStr = "Lookup failed:" + host.errorString();
        outputMsg(errotStr, "red");
        return;
    }else {
        const auto addresses = host.addresses();
        QString ipStrList;
        for (const QHostAddress &address : addresses){
            ipStrList += address.toString() + " ";
        }

        ipStrList = ipStrList.trimmed();
        ui->lineEditAddress->setText(ipStrList);
    }
}

void ToolsNsLookup::copyAddress()
{
    ui->lineEditAddress->selectAll();
    ui->lineEditAddress->copy();
    outputMsg(tr("已复制到剪贴板！"));
}

void ToolsNsLookup::outputMsg(QString msg, QString color)
{
    ui->labelError->setText(QString("<font color=%1>%2</font>").arg(color).arg(msg));
    errorLabelOpacity = 1;
    hideErrorTimer->start(300);
}

void ToolsNsLookup::hideErrorTimerTimeout()
{
    /// 透明度不生效....
    if (errorLabelOpacity - 0.1 < 0.00000001){    /// opacity == 0.1
        hideErrorTimer->stop();
        ui->widgetError->setWindowOpacity(1);
        ui->labelError->clear();
        errorLabelOpacity = 1;
    }else {
        errorLabelOpacity = errorLabelOpacity - 0.1;
        ui->widgetError->setWindowOpacity(errorLabelOpacity);
    }
}
