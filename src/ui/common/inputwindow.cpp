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

    /// 读入文件
    connect(ui->pushButton_open, SIGNAL(clicked(bool)), this, SLOT(OpenFile()));
}

InputWindow::~InputWindow()
{

}

QByteArray InputWindow::Data()
{
    QByteArray data = ui->textEdit->toPlainText().toLatin1();

    return data;
}

void InputWindow::OpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"));
    if (!fileName.isEmpty()){
        QFile file(fileName);
        if(file.open(QFile::ReadOnly)){
            QByteArray data = file.readAll();
            ui->textEdit->setText(QString(data).toUtf8());
            file.close();
        }else{
            qWarning() << QString("%1 %2").arg(tr("无法打开文件")).arg(fileName);
        }
    }

}
