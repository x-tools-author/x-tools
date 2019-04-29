/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef AUTORESPONSEITEM_HH
#define AUTORESPONSEITEM_HH

#include <QWidget>
#include <QTimer>

namespace Ui {
class SAKAutoResponseItem;
}

class SAKAutoResponseItem:public QWidget
{
    Q_OBJECT
public:
    SAKAutoResponseItem(QWidget *parent = Q_NULLPTR);
    ~SAKAutoResponseItem();

    void setText(QString receiveDataString, QString sendDataString, QString description);
    void showModify();
protected:
    void closeEvent(QCloseEvent *event);
private:
    bool isModify = false;
    QTimer *clearOutputInfoTimer = nullptr;
    Ui::SAKAutoResponseItem *ui;
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
