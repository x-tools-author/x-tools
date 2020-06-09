/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKINPUTDATAITEMMANAGER_HH
#define SAKINPUTDATAITEMMANAGER_HH

#include <QLabel>
#include <QTimer>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>

#include "SAKDebugPageInputManager.hh"

namespace Ui {
    class SAKInputDataItemManager;
}

class SAKDebugPage;
/// @brief 预设数据管理窗口
class SAKInputDataItemManager:public QWidget
{
    Q_OBJECT
public:
    SAKInputDataItemManager(SAKDebugPage *debugPage, SAKDebugPageInputManager *inputManager, QWidget *parent = Q_NULLPTR);
    ~SAKInputDataItemManager();
private:
    SAKDebugPage *debugPage;
    SAKCRCInterface *crcInterface;
    SAKInputDataFactory *factory;
    SAKDebugPageInputManager *inputManager;
    QListWidget *listWidget;
    SAKDebugPageInputManager::InputParameters inputParameters;
private:
    Ui::SAKInputDataItemManager *ui;
    QPushButton *deletePushButton;
    QPushButton *addPushButton;
    QLabel *infoLabel;
private slots:
    void on_deletePushButton_clicked();
    void on_addPushButton_clicked();
};

#endif
