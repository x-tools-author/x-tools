/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKOTHERTRANSMISSIONITEMUDP_HH
#define SAKOTHERTRANSMISSIONITEMUDP_HH

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QUdpSocket>
#include "SAKOtherTransmissionItem.hh"

namespace Ui {
    class SAKOtherTransmissionItemUdp;
}

class SAKOtherTransmissionItemUdp:public SAKOtherTransmissionItem
{
    Q_OBJECT
public:
    SAKOtherTransmissionItemUdp(SAKDebugPage *debugPage, QWidget *parent);
    ~SAKOtherTransmissionItemUdp();

    virtual void write(QByteArray data);
private:
    Ui::SAKOtherTransmissionItemUdp *mUi;
    QCheckBox *mEnableCheckBox;
    QCheckBox *mCustomAddressCheckBox;
    QComboBox *mAddressComboBox;
    QLineEdit *mPortLineEdit;
    QCheckBox *mHandleReceiveDataCheckBox;
    QLineEdit *mTargetAddressLineEdit;
    QLineEdit *mTargetPortLineEdit;

    QUdpSocket *mUdpSocket;
private slots:
    void on_enableCheckBox_clicked();
private:
    void read();
    void setUiEnable(bool enable);
};

#endif
