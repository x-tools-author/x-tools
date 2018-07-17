#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QWidget>

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
    Ui::InputWindow *ui;
};

#endif
