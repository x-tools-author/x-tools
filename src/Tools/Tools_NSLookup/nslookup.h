/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SAK_NSLOOKUP_H
#define SAK_NSLOOKUP_H

#include <QWidget>
#include <QHostInfo>
#include <QTimer>
#include <QDebug>

namespace Ui {
class ToolsNsLookup;
}

class ToolsNsLookup:public QWidget
{
    Q_OBJECT
public:
    ToolsNsLookup(QWidget *parent = Q_NULLPTR);
    ~ToolsNsLookup();
private:
    Ui::ToolsNsLookup *ui;
    QTimer *hideErrorTimer = nullptr;
    double errorLabelOpacity = 1;
    ///----------------------------------------------------------------
    void Connect();
private slots:
    void lookupHost();
    void lookedUpResult(const QHostInfo &host);
    void copyAddress();

    void outputMsg(QString msg, QString color = "green");
    void hideErrorTimerTimeout();
};

#endif
