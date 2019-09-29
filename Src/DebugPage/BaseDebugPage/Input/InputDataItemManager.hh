/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#ifndef INPUTDATAITEMMANAGER_HH
#define INPUTDATAITEMMANAGER_HH

#include <QLabel>
#include <QTimer>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>

#include "DebugPageInputManager.hh"

namespace Ui {
    class InputDataItemManager;
}

class SAKDebugPage;
class DebugPageInputManager;
class InputDataItemManager:public QWidget
{
    Q_OBJECT
public:
    InputDataItemManager(SAKDebugPage *debugPage, DebugPageInputManager *inputManager, QWidget *parent = Q_NULLPTR);
    ~InputDataItemManager();

private:
    Ui::InputDataItemManager    *ui;
    QPushButton                 *deletePushButton;
    QPushButton                 *addPushButton;
    QLabel                      *infoLabel;
private slots:
    void on_deletePushButton_clicked();
    void on_addPushButton_clicked();

private:
    SAKDebugPage            *debugPage;
    SAKCRCInterface         *crcInterface;
    InputDataFactory        *factory;
    DebugPageInputManager   *inputManager;
    QListWidget             *listWidget;

    QTimer sendTimer;
    DebugPageInputManager::InputParameters inputParameters;
signals:
    void rawDataChanged(QString rawData, DebugPageInputManager::InputParameters parameters);
};

#endif
