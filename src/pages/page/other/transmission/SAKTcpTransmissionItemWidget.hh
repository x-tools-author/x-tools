/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKTCPTRANSMISSIONITEMWIDGET_HH
#define SAKTCPTRANSMISSIONITEMWIDGET_HH

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTcpSocket>
#include "SAKBaseTransmissionItemWidget.hh"

namespace Ui {
    class SAKTcpTransmissionItemWidget;
}

class SAKTcpTransmissionItemWidget:public SAKBaseTransmissionItemWidget
{
    Q_OBJECT
public:
    SAKTcpTransmissionItemWidget(SAKDebugPage *_debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKTcpTransmissionItemWidget();

    virtual void write(QByteArray data);
private:
    Ui::SAKTcpTransmissionItemWidget *ui;
    QCheckBox *enableCheckBox;
    QCheckBox *customAddressCheckBox;
    QComboBox *localAddressComboBox;
    QLineEdit *localPortLineEdit;
    QCheckBox *handleReceiveDataCheckBox;
    QLineEdit *serverAddressLineEdit;
    QLineEdit *serverPortLineEdit;

    QTcpSocket  *tcpSocket;
private slots:
    void on_enableCheckBox_clicked();
private:
    void read();
private:
    void setUiEnable(bool enable);
};

#endif
