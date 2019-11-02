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
 */
#ifndef UDPTRANSMISSSIONITEMWIDGET_HH
#define UDPTRANSMISSSIONITEMWIDGET_HH

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QUdpSocket>

#include "BaseTransmissionItemWidget.hh"

namespace Ui {
    class UdpTransmissionItemWidget;
}

class UdpTransmissionItemWidget:public BaseTransmissionItemWidget
{
    Q_OBJECT
public:
    UdpTransmissionItemWidget(SAKDebugPage *_debugPage, QWidget *parent);
    ~UdpTransmissionItemWidget();

    virtual void write(QByteArray data);
private:
    Ui::UdpTransmissionItemWidget *ui;
    QCheckBox *enableCheckBox;
    QCheckBox *customAddressCheckBox;
    QComboBox *addressComboBox;
    QLineEdit *portLineEdit;
    QCheckBox *handleReceiveDataCheckBox;
    QLineEdit *targetAddressLineEdit;
    QLineEdit *targetPortLineEdit;

    QUdpSocket  *udpSocket;
private slots:
    void on_enableCheckBox_clicked();
private:
    void read();
    void setUiEnable(bool enable);
};

#endif
