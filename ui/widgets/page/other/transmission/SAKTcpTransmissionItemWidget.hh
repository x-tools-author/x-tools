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
    SAKTcpTransmissionItemWidget(SAKDebugPage *_debugPage, QWidget *parent = nullptr);
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
