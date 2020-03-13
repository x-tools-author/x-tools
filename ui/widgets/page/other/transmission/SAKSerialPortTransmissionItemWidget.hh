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
    SAKSerialPortTransmissionItemWidget(SAKDebugPage *_debugPage, QWidget *parent = nullptr);

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
