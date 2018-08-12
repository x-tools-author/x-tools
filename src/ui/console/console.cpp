#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "console.h"
#include "ui_console.h"


Console *appConsole(){
    return Console::_console;
}

Console* Console::_console = NULL;

Console::Console(QWidget *parent):
    QWidget(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);

    _console = this;
}

Console::~Console()
{

}

void Console::OutputInfo(QString info)
{
    ui->textBrowser->append(info);
}
