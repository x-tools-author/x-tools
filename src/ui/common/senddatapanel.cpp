#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "senddatapanel.h"
#include "ui_senddatapanel.h"

SendDataPanel::SendDataPanel(QWidget *parent):
    QWidget(parent),
    ui(new Ui::SendDataPanel)
{
    ui->setupUi(this);
}

SendDataPanel::~SendDataPanel()
{

}
