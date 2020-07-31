/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
