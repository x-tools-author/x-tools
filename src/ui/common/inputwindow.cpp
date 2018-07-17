#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "inputwindow.h"
#include "ui_inputwindow.h"

InputWindow::InputWindow(QWidget *parent):
    QWidget(parent),
    ui(new Ui::InputWindow)
{
    ui->setupUi(this);
}

InputWindow::~InputWindow()
{

}

QByteArray InputWindow::Data()
{
    QByteArray data = ui->textEdit->toPlainText().toLatin1();

    return data;
}
