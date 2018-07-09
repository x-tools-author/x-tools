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
private:
    Ui::InputWindow *ui;
};

#endif
