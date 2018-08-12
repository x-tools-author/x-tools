#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "outputwindow.h"
#include "ui_outputwindow.h"

OutputWindow::OutputWindow(QWidget *parent):
    QWidget(parent),
    ui(new Ui::OutputWindow)
{
    ui->setupUi(this);
    /// 保存输出
    connect(ui->pushButton_save, SIGNAL(clicked(bool)), this, SLOT(SaveOutputData()));
    /// 重置计数
    connect(ui->pushButton_resetCount, SIGNAL(clicked(bool)), this, SLOT(ResetCount()));
}

OutputWindow::~OutputWindow()
{

}

/**
 * @brief 输出数据
 * @param data  --需要输出的数据
 * @param color --输出文本颜色
 */
void OutputWindow::OutputData(QByteArray data)
{
    if (ui->checkBox_pause->isChecked()){
        return;
    }

    QString str, color = "green", datetimeColor = "silver";
    /// 添加日期信息
    if (ui->checkBox_date->isChecked()){
        str.append(QString("<font color=%1>%2</font>").arg(datetimeColor).arg(QDate::currentDate().toString("yyyy/MM/dd ")));
    }
    /// 添加时间信息
    if (ui->checkBox_time->isChecked()){
        str.append(QString("<font color=%1>%2</font>").arg(datetimeColor).arg(QTime::currentTime().toString("hh:mm:ss ")));
    }

    if (ui->radioButton_bin->isChecked()){  /// 二进制
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(data.at(i), 2), 8, '0'));
        }
    }else if (ui->radioButton_oct->isChecked()){    /// 八进制
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(data.at(i), 8), 2, '0'));
        }
    }else if (ui->radioButton_dec->isChecked()){    /// 十进制
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(data.at(i), 10), 2, '0'));
        }
    }else if (ui->radioButton_hex->isChecked()){    /// 16进制
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(data.at(i), 16), 2, '0'));
        }
    }else if (ui->radioButton_ascii->isChecked()){    /// ascii进制
        str.append(QString(data));
    }else if (ui->radioButton_utf8->isChecked()){    /// utf-8
        str.append(QString(data));
    }else{
        qWarning() << tr("未知显示各式！");
    }

    /// 输出信息
    ui->textBrowser->append(QString("<font color=%1>%2</font>").arg(color).arg(str));

    /// 帧数
    qulonglong frame = ui->label_frame->text().toULongLong();
    frame += 1;
    ui->label_frame->setText(QString::number(frame));
    /// 字节数
    qulonglong bytes = ui->label_bytes->text().toULongLong();
    bytes += data.length();
    ui->label_bytes->setText(QString::number(bytes));
}

/**
 * @brief 输出信息
 * @param info  --需要输出的信息
 * @param color --输出文本颜色
 */
void OutputWindow::OutputInfo(QString info, QString color)
{
    QString str;
    /// 添加日期信息
    if (ui->checkBox_date->isChecked()){
        str.append(QDate::currentDate().toString("yyyy/MM/dd "));
    }
    /// 添加时间信息
    if (ui->checkBox_time->isChecked()){
        str.append(QTime::currentTime().toString("hh:mm:ss "));
    }

    str.append(info);

    /// 输出信息
    ui->textBrowser->append(QString("<font color%1>%2</font>").arg(color).arg(str));
}

void OutputWindow::SaveOutputData()
{
    QString outFileName = QFileDialog::getSaveFileName();
    QFile outFile(outFileName);

    outFile.open(QIODevice::WriteOnly|QIODevice::Text);

    QTextStream outStream(&outFile);
    outStream << ui->textBrowser->toPlainText();

    outFile.flush();
    outFile.close();
}

void OutputWindow::ResetCount()
{
    ui->label_bytes->setText("0");
    ui->label_frame->setText("0");
}
