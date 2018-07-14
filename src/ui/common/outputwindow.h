#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include <QWidget>
#include <QDate>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>

namespace Ui{
class OutputWindow;
}

class OutputWindow : public QWidget
{
    Q_OBJECT
public:
    OutputWindow(QWidget *parent = Q_NULLPTR);
    ~OutputWindow();

private:
    Ui::OutputWindow *ui;
private slots:
    /// 输出数据（串口，网络接收到的数据）
    void OutputData(QByteArray data);
    /// 输出普通信息
    void OutputInfo(QString info, QString color = "black");
    /// 保存输出
    void SaveOutputData();
};

#endif  /// OUTPUTWINDOW_H
