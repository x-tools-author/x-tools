/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKSERIALPORTTRANSMISSIONITEMWIDGET_HH
#define SAKSERIALPORTTRANSMISSIONITEMWIDGET_HH

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QIODevice>
#include <QPushButton>

#ifdef SAK_IMPORT_COM_MODULE
#include <QSerialPort>
#endif

#include "SAKBaseTransmissionItemWidget.hh"

namespace Ui {
    class SAKSerialPortTransmissionItemWidget;
}

class SAKDebugPage;
class SAKSerialPortTransmissionItemWidget:public SAKBaseTransmissionItemWidget
{
    Q_OBJECT
public:
    SAKSerialPortTransmissionItemWidget(SAKDebugPage *_debugPage, QWidget *parent = Q_NULLPTR);

#ifdef SAK_IMPORT_COM_MODULE
     ~SAKSerialPortTransmissionItemWidget();
protected:
    virtual void write(QByteArray data);
    virtual void read();
private:
    Ui::SAKSerialPortTransmissionItemWidget *ui;
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
