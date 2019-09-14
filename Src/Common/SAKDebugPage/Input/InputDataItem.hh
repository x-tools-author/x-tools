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
#ifndef INPUTDATAITEM_HH
#define INPUTDATAITEM_HH

#include <QTimer>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

#include "DebugPageInputManager.hh"

namespace Ui {
    class InputDataItem;
}

class SAKDebugPage;
class SAKCRCInterface;
class InputDataFactory;
class DebugPageInputManager;
class InputDataItem:public QWidget
{
    Q_OBJECT
public:
    InputDataItem(SAKDebugPage *debugPage, DebugPageInputManager *inputManager, QWidget *parent = Q_NULLPTR);
    ~InputDataItem();

private:
    Ui::InputDataItem *ui;

    QComboBox   *textFormatComboBox;
    QCheckBox   *timingCheckBox;
    QLineEdit   *timingTimeLineEdit;
    QComboBox   *crcModelComboBox;
    QCheckBox   *addCrcCheckBox;
    QCheckBox   *bigEndianCheckBox;
    QPushButton *sendPushButton;
    QTextEdit   *inputDataTextEdit;
private slots:
    void on_textFormatComboBox_currentIndexChanged(int index);
    void on_timingCheckBox_clicked();
    void on_sendPushButton_clicked();
    void on_inputDataTextEdit_textChanged();

private:
    SAKDebugPage *debugPage;
    SAKCRCInterface *crcInterface;
    InputDataFactory *factory;
    DebugPageInputManager *inputManager;

    QTimer sendTimer;
    DebugPageInputManager::InputParameters inputParameters;
private:
    void sendTimerTimeout();
signals:
    void rawDataChanged(QString rawData, DebugPageInputManager::InputParameters parameters);
};

#endif
