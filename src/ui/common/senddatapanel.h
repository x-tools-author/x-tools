#ifndef SENDDATAPANEL_H
#define SENDSATAPANEL_H

#include <QWidget>

namespace Ui {
class SendDataPanel;
}

class SendDataPanel : public QWidget
{
    Q_OBJECT
public:
    SendDataPanel(QWidget *parent = Q_NULLPTR);
    ~SendDataPanel();
private:
    Ui::SendDataPanel *ui;
signals:

};

#endif
