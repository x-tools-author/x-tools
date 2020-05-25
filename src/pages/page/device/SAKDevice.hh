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
#ifndef SAKINPUTDATAITEM_HH
#define SAKINPUTDATAITEM_HH

#include <QTimer>
#include <QWidget>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

#include "SAKDebugPageInputManager.hh"

namespace Ui {
    class SAKInputDataItem;
}

class SAKDebugPage;
class SAKCRCInterface;
class SAKInputDataFactory;
class SAKDebugPageInputManager;
class SAKInputDataItem:public QWidget
{
    Q_OBJECT
public:
    SAKInputDataItem(SAKDebugPage *debugPage, SAKDebugPageInputManager *inputManager, QWidget *parent = Q_NULLPTR);
    ~SAKInputDataItem();

private:
    Ui::SAKInputDataItem *ui;

    QComboBox   *textFormatComboBox;
    QLineEdit   *descriptionLineEdit;
    QTextEdit   *inputDataTextEdit;

private:
    QPushButton *menuPushButton;
    QAction *action;
    SAKDebugPage *debugPage;
    SAKDebugPageInputManager *inputManager;

    SAKDebugPageInputManager::InputParameters inputParameters;
private:
    void addDataAction(QPushButton *menuPushButton);
    void removeDataAction(QPushButton *menuPushButton);
    void updateActionTitle(const QString &title);
    void updateTextFormat();
    void sendRawData();

signals:
    void rawDataChanged(QString rawData, SAKDebugPageInputManager::InputParameters parameters);
};

#endif
