/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKUDPTRANSMISSSIONITEMWIDGET_HH
#define SAKUDPTRANSMISSSIONITEMWIDGET_HH

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QUdpSocket>

#include "SAKBaseTransmissionItemWidget.hh"

namespace Ui {
    class SAKUdpTransmissionItemWidget;
}

class SAKUdpTransmissionItemWidget:public SAKBaseTransmissionItemWidget
{
    Q_OBJECT
public:
    SAKUdpTransmissionItemWidget(SAKDebugPage *_debugPage, QWidget *parent);
    ~SAKUdpTransmissionItemWidget();

    virtual void write(QByteArray data);
private:
    Ui::SAKUdpTransmissionItemWidget *ui;
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
