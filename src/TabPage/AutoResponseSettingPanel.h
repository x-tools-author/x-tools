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
#include <QStringList>

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

    void setAutoResponseFlag(bool enableAutoResponse);
    bool autoResponseFlag();
    QList <AutoResponseNode> autoResponseItems(){return  autoResponseItemList;}
private:
    int need2modifyItemRow = 0;
    Ui::AutoResponseSettingPanel *ui;
    QList <AutoResponseNode> autoResponseItemList;
    AutoResponseItem *autoResponseItemWidget;
    QTimer *clearOutputInfoTimer = nullptr;
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
    void modifyAutoResponseItem(QString receiveDataString, QString sendDataString, QString description);
    void clearAutoResponseItem();
    void saveAsFile();
    void readIn();
    void enableAutoResponseBtClicked();
signals:
    void autoResponseFlagChanged(bool enableAutoResponse);
};

#define ERR_STR_LIST_IS_EMPTY tr("当前列表为空，无法执行该操作！")

#endif
