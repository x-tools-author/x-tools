#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "senddatapanel.h"
#include "ui_senddatapanel.h"

#include <QIntValidator>

SendDataPanel::SendDataPanel(QWidget *parent):
    QWidget(parent),
    ui(new Ui::SendDataPanel)
{
    ui->setupUi(this);
    InitUi();
}

SendDataPanel::~SendDataPanel()
{
    /// 发送数据
    connect(ui->pushButton_send, SIGNAL(clicked(bool)), this, SLOT(SendData()));
}

void SendDataPanel::InitUi()
{
    /// 输入框输入内容限制
    ui->lineEdit_cycleTime->setMaxLength(9);
    ui->lineEdit_cycleTime->setValidator(new QIntValidator(10, 999999999, this));
}

void SendDataPanel::SendData()
{

}
