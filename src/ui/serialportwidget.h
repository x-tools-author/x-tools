#ifndef SERIALPORTWIDGET_H
#define SERIALPORTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>

#include "outputwindow.h"
#include "inputwindow.h"
#include "senddatapanel.h"

namespace Ui {
class SerialportWidget;
}

class SerialPortWidget : public QWidget
{
    Q_OBJECT
public:
    SerialPortWidget(QWidget *parent = Q_NULLPTR);
    ~SerialPortWidget();
private:
    Ui::SerialportWidget *ui;
    OutputWindow *mpOutputWindow;
    InputWindow *mpInputWindow;
    SendDataPanel *mpSendDataPanel;
};

#endif
