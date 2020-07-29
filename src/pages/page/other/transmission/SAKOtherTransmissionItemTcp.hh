/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKOTHERTRANSMISSIONITEMTCP_HH
#define SAKOTHERTRANSMISSIONITEMTcp_HH

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTcpSocket>
#include "SAKOtherTransmissionItem.hh"

namespace Ui {
    class SAKOtherTransmissionItemTcp;
}

class SAKOtherTransmissionItemTcp:public SAKOtherTransmissionItem
{
    Q_OBJECT
public:
    SAKOtherTransmissionItemTcp(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKOtherTransmissionItemTcp();

    virtual void write(QByteArray data);
private:
    Ui::SAKOtherTransmissionItemTcp *mUi;
    QCheckBox *mEnableCheckBox;
    QCheckBox *mCustomAddressCheckBox;
    QComboBox *mLocalAddressComboBox;
    QLineEdit *mLocalPortLineEdit;
    QCheckBox *mHandleReceiveDataCheckBox;
    QLineEdit *mServerAddressLineEdit;
    QLineEdit *mServerPortLineEdit;

    QTcpSocket *mTcpSocket;
private slots:
    void on_enableCheckBox_clicked();
private:
    void read();
private:
    void setUiEnable(bool enable);
};

#endif
