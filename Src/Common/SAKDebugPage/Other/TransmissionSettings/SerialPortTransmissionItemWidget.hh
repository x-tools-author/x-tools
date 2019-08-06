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
#ifndef SERIALPORTTRANSMISSIONITEMWIDGET_HH
#define SERIALPORTTRANSMISSIONITEMWIDGET_HH

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QIODevice>
#include <QPushButton>

#include "BaseTransmissionItemWidget.hh"

namespace Ui {
class SerialPortTransmissionItemWidget;
}

class SerialPortTransmissionItemWidget:public BaseTransmissionItemWidget
{
    Q_OBJECT
public:
    SerialPortTransmissionItemWidget(SAKDebugPage *debugPage, QWidget *parent = nullptr);
private:
    Ui::SerialPortTransmissionItemWidget *ui;
    QCheckBox   *enableCheckBox;
    QCheckBox   *handleReceiveDataCheckBox;
    QComboBox   *comComboBox;
    QCheckBox   *customBaudrateCheckBox;
    QComboBox   *baudRateComboBox;
    QComboBox   *dataBitscomboBox;
    QComboBox   *stopBitscomboBox;
    QComboBox   *parityComboBox;
private slots:
    void on_enableCheckBox_clicked();
    void on_handleReceiveDataCheckBox_clicked();
    void on_customBaudrateCheckBox_clicked();
};

#endif
