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
    /// 文本改变
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(TextFormatControl()));
    /// 重置计数
    connect(ui->pushButton_resetCount, SIGNAL(clicked(bool)), this, SLOT(ResetCount()));
}

InputWindow::~InputWindow()
{

}

QByteArray InputWindow::Data()
{
    QByteArray data;
    QString str = ui->textEdit->toPlainText();
    if (str.isEmpty()){
        qDebug() << tr("输入内容为空，忽略本次发送！");
        return QByteArray("");
    }

    if (ui->radioButton_bin->isChecked()){
        QStringList strList = str.split(' ');
        foreach (QString str, strList){
            data.append((uint8_t)QString(str).toInt(NULL, 2));
        }
    }else if (ui->radioButton_oct->isChecked()){
        QStringList strList = str.split(' ');
        foreach (QString str, strList){
            data.append((uint8_t)QString(str).toInt(NULL, 8));
        }
    }else if (ui->radioButton_dec->isChecked()){
        QStringList strList = str.split(' ');
        foreach (QString str, strList){
            data.append((uint8_t)QString(str).toInt(NULL, 10));
        }
    }else if (ui->radioButton_hex->isChecked()){
        QStringList strList = str.split(' ');
        foreach (QString str, strList){
            data.append((uint8_t)QString(str).toInt(NULL, 16));
        }
    }else if (ui->radioButton_ascii->isChecked()){
        data = ui->textEdit->toPlainText().toLatin1();
    }else if (ui->radioButton_utf8->isChecked()){
        data = ui->textEdit->toPlainText().toUtf8();
    }else {
        qWarning() << tr("未知输入各式！");
    }

    static uint64_t frameCount = 0;
    static uint64_t bytesCount = 0;

    frameCount  = (uint64_t)ui->label_frame->text().toLongLong();
    frameCount += 1;
    ui->label_frame->setText(QString::number(frameCount));

    bytesCount = (uint64_t)ui->label_bytes->text().toLongLong();
    bytesCount += data.length();
    ui->label_bytes->setText(QString::number(bytesCount));


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

void InputWindow::TextFormatControl()
{
    disconnect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(TextFormatControl()));
    QString plaintext = ui->textEdit->toPlainText();
    if (plaintext.isEmpty()){
        connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(TextFormatControl()));
        return;
    }else {
        if (ui->radioButton_bin->isChecked()){  /// 二进制输入
            QString strTemp;
            plaintext.remove(QRegExp("[^0-1]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 8 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            ui->textEdit->setText(strTemp);
            ui->textEdit->moveCursor(QTextCursor::End);
        }else if(ui->radioButton_oct->isChecked()) { /// 八进制输入
            QString strTemp;
            plaintext.remove(QRegExp("[^0-7]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            ui->textEdit->setText(strTemp);
            ui->textEdit->moveCursor(QTextCursor::End);
        }else if(ui->radioButton_dec->isChecked()) { /// 十进制
            QString strTemp;
            plaintext.remove(QRegExp("[^0-9]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            ui->textEdit->setText(strTemp);
            ui->textEdit->moveCursor(QTextCursor::End);
        }else if(ui->radioButton_hex->isChecked()) { /// 十六进制
            QString strTemp;
            plaintext.remove(QRegExp("[^0-9a-fA-F]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            ui->textEdit->setText(strTemp.toUpper());
            ui->textEdit->moveCursor(QTextCursor::End);
        }else if(ui->radioButton_ascii->isChecked()) { /// ascii
            plaintext.remove(QRegExp("[^\u0021-\u007E]"));
            ui->textEdit->setText(plaintext);
            ui->textEdit->moveCursor(QTextCursor::End);
        }else if(ui->radioButton_utf8->isChecked()) {    /// utf-8

        }else {
            qWarning() << tr("未知输入选项！");
        }
    }
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(TextFormatControl()));
}

void InputWindow::ResetCount()
{
    ui->label_frame->setText("0");
    ui->label_bytes->setText("0");
}
