#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QDebug>

namespace Ui {
class InputWindow;
}

class InputWindow : public QWidget
{
    Q_OBJECT
public:
    InputWindow(QWidget *parent = Q_NULLPTR);
    ~InputWindow();
    /// 输入框数据(会根据输入设置选项来提取数据)
    QByteArray Data();
private:
    /// ui界面
    Ui::InputWindow *ui;
    /// ------------------------------------------
private slots:
    /// 读入文本数据
    void OpenFile();
};

#endif
