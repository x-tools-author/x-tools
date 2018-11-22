#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "console.h"
#include "ui_console.h"


Console *appConsole(){
    return Console::_console;
}

Console* Console::_console = nullptr;

Console::Console(QWidget *parent):
    QWidget(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);

    _console = this;

    connect(ui->pushButton_save, SIGNAL(clicked(bool)), this, SLOT(SaveConsoleOutput2File()));
}

Console::~Console()
{

}

void Console::OutputInfo(QString info)
{
    ui->textBrowser->append(info);
}

void Console::SaveConsoleOutput2File()
{
    if (ui->textBrowser->toPlainText().isEmpty()){
        qWarning() << tr("终端输出内容为空，忽略本次保存操作！");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName();

    if (!fileName.isEmpty()){
        QFile file(fileName);

        if (file.open(QFile::ReadWrite | QFile::Text)){
            QString str = ui->textBrowser->toPlainText();
            QTextStream outStream(&file);
            outStream << str;

            file.close();
        }
    }
}
