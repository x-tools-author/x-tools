/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
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
#ifndef READWRITESETTINGSWIDGET_HH
#define READWRITESETTINGSWIDGET_HH

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>


namespace Ui {
class ReadWriteSettingsWidget;
};

class SAKDebugPage;
class ReadWriteSettingsWidget:public QDialog
{
    Q_OBJECT
public:
    ReadWriteSettingsWidget(SAKDebugPage *debugPage, QWidget* parent = nullptr);
private:
    Ui::ReadWriteSettingsWidget *ui = nullptr;

    QCheckBox* readDelayEnableCheckBox  = nullptr;
    QCheckBox* writeDelayEnableCheckBox = nullptr;
    QComboBox* readDelayComboBox        = nullptr;
    QComboBox* writeDelayComboBox       = nullptr;
    QCheckBox* readCustomDelayCheckBox = nullptr;
    QCheckBox* writeCustomDelayCheckBox = nullptr;
    QLineEdit* readLineEdit             = nullptr;
    QLineEdit* writeLineEdit            = nullptr;

    void initUi();
    void changedReadDelayTime();
    void changedWriteDelayTime();

    void setEnableReadDelay();
    void setEnableWriteDelay();
private:
    SAKDebugPage *_debugPage;
signals:
    void readDelayTimeChanged(int ms);
    void writeDelayTimeChanged(int ms);
};

#endif
