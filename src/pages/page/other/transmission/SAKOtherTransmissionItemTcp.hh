/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOTHERTRANSMISSIONITEMTCP_HH
#define SAKOTHERTRANSMISSIONITEMTCP_HH

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
