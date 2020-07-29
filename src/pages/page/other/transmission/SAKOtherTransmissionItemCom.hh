/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOTHERTRANSMISSIONITEMCOM_HH
#define SAKOTHERTRANSMISSIONITEMCOM_HH

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QIODevice>
#include <QPushButton>

#ifdef SAK_IMPORT_COM_MODULE
#include <QSerialPort>
#endif

#include "SAKOtherTransmissionItem.hh"

namespace Ui {
    class SAKOtherTransmissionItemCom;
}

class SAKDebugPage;
class SAKOtherTransmissionItemCom:public SAKOtherTransmissionItem
{
    Q_OBJECT
public:
    SAKOtherTransmissionItemCom(SAKDebugPage *_debugPage, QWidget *parent = Q_NULLPTR);

#ifdef SAK_IMPORT_COM_MODULE
     ~SAKOtherTransmissionItemCom();
protected:
    virtual void write(QByteArray data);
    virtual void read();
private:
    Ui::SAKOtherTransmissionItemCom *ui;
    QCheckBox   *enableCheckBox;
    QCheckBox   *handleReceiveDataCheckBox;
    QComboBox   *comComboBox;
    QCheckBox   *customBaudrateCheckBox;
    QComboBox   *baudRateComboBox;
    QComboBox   *dataBitscomboBox;
    QComboBox   *stopBitscomboBox;
    QComboBox   *parityComboBox;

    QSerialPort *serialPort;
private slots:
    void on_enableCheckBox_clicked();
    void on_customBaudrateCheckBox_clicked();
private:
    void setUiEnable(bool enable);
#endif
};

#endif
