#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "serialportwidget.h"
#include "ui_serialportwidget.h"

SerialPortWidget::SerialPortWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::SerialportWidget),
    mpOutputWindow(new OutputWindow),
    mpInputWindow(new InputWindow),
    mpSendDataPanel(new SendDataPanel)
{
    /// 初始化UI
    ui->setupUi(this);
    QHBoxLayout *pOutputLayout = new QHBoxLayout;
    ui->widget_output->setLayout(pOutputLayout);
    pOutputLayout->addWidget(mpOutputWindow);
    pOutputLayout->setMargin(0);
    QHBoxLayout *pInputLayout = new QHBoxLayout;
    ui->widget_input->setLayout(pInputLayout);
    pInputLayout->addWidget(mpInputWindow);
    pInputLayout->setMargin(0);
    QHBoxLayout *pSendLayout = new QHBoxLayout;
    ui->widget_sendPanel->setLayout(pSendLayout);
    pSendLayout->addWidget(mpSendDataPanel);
    pSendLayout->setMargin(0);

}

SerialPortWidget::~SerialPortWidget()
{

}
