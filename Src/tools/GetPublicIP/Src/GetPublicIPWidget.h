#ifndef GETPUBLICIPWIDGET_H
#define GETPUBLICIPWIDGET_H

#include <QWidget>
#include <QTimer>

#include "GetPublicIPThread.h"

namespace Ui {
class GetPublicIPWidget;
}

class GetPublicIPWidget : public QWidget
{
    Q_OBJECT
public:
    GetPublicIPWidget(QWidget *parent = Q_NULLPTR);
    ~GetPublicIPWidget();
private:
    QTimer *clearOutputInfoTimer = nullptr;
    GetPublicIpThread *getPublicIpThread = nullptr;
    Ui::GetPublicIPWidget *ui;
private slots:
    void setText(QString ipAddress);
    void outputInfo(QString info, QString color = QString("green"));
    void clearOutputInfo();
    void copyIPAddress();
};

#endif
