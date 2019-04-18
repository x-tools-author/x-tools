/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef AUTORESPONSEITEM_H
#define AUTORESPONSEITEM_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class AutoResponseItem;
}

class AutoResponseItem:public QWidget
{
    Q_OBJECT
public:
    AutoResponseItem(QWidget *parent = Q_NULLPTR);
    ~AutoResponseItem();

    void setText(QString receiveDataString, QString sendDataString, QString description);
    void showModify();
protected:
    void closeEvent(QCloseEvent *event);
private:
    bool isModify = false;
    QTimer *clearOutputInfoTimer = nullptr;
    Ui::AutoResponseItem *ui;
    ///-----------------------------------------------------------
    void Connect();
private slots:
    void cancle();
    void addAotoResponseItem();
    void outputInfo(QString info, QString color = "green");
    void clearOutputInfo();
signals:
    void need2addAotoResponseItem(QString receiveDataString, QString sendDataString, QString Description);
    void need2modifyResponseItem(QString receiveDataString, QString sendDataString, QString Description);
};

#endif
