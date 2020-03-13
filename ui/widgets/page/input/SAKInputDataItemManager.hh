/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
class SAKDebugPageInputManager;
class SAKInputDataItemManager:public QWidget
{
    Q_OBJECT
public:
    SAKInputDataItemManager(SAKDebugPage *debugPage, SAKDebugPageInputManager *inputManager, QWidget *parent = Q_NULLPTR);
    ~SAKInputDataItemManager();

private:
    Ui::SAKInputDataItemManager *ui;
    QPushButton                 *deletePushButton;
    QPushButton                 *addPushButton;
    QLabel                      *infoLabel;
private slots:
    void on_deletePushButton_clicked();
    void on_addPushButton_clicked();

private:
    SAKDebugPage            *debugPage;
    SAKCRCInterface         *crcInterface;
    SAKInputDataFactory        *factory;
    SAKDebugPageInputManager   *inputManager;
    QListWidget             *listWidget;

    QTimer sendTimer;
    SAKDebugPageInputManager::InputParameters inputParameters;
signals:
    void rawDataChanged(QString rawData, SAKDebugPageInputManager::InputParameters parameters);
};

#endif
