/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef AUTORESPONSESETTINGPANEL_H
#define AUTORESPONSESETTINGPANEL_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTimer>

#include "AutoResponseItem.h"

namespace Ui {
class AutoResponseSettingPanel;
}

typedef struct {
    QString receiveDataString;
    QString sendDataString;
    QString description;
    QListWidgetItem *listWidgetItem;
}AutoResponseNode;

class AutoResponseSettingPanel:public QWidget
{
   Q_OBJECT
public:
    AutoResponseSettingPanel(QWidget *parent = Q_NULLPTR);
    ~AutoResponseSettingPanel();
private:
    int need2modifyItemRow = 0;
    QTimer *clearOutputInfoTimer = NULL;
    AutoResponseItem *autoResponseItemWidget;
    QList <AutoResponseNode> autoResponseItemList;
    Ui::AutoResponseSettingPanel *ui;
    ///----------------------------------------------------------------------
    void Connect();
private slots:
    QString packetItemString(QString receiveData, QString sendData);
    void resortTheAutoResponseItemList(int currentRow);
    void outputInfo(QString info, QString color = QString("green"));
    void clearOutputInfo();

    void addAutoResponseItem(QString receiveData, QString sendData, QString description);
    void deleteAutoResponseItem();
    void modifyAutoResponseItem();
    void modifyResponseItem(QString receiveDataString, QString sendDataString, QString description);
};

#define ERR_STR_LIST_IS_EMPTY tr("当前列表为空，无法执行该操作！")

#endif
