/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#ifndef SAKSERIALPORTASSISTANTCONTROLLER_HH
#define SAKSERIALPORTASSISTANTCONTROLLER_HH

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QSerialPort>

namespace Ui {
class SAKSerialportAssistantController;
}

class SAKSerialportAssistantController:public QWidget
{
    Q_OBJECT
public:
    SAKSerialportAssistantController(QWidget *parent = Q_NULLPTR);
    ~SAKSerialportAssistantController();

    enum QSerialPort::DataBits  dataBits();
    enum QSerialPort::StopBits  stopBits();
    enum QSerialPort::Parity    parity();

    QString                     name();
    qint32                      baudRate();

    void refresh();
    void setUiEnable(bool enable);
private:
    Ui::SAKSerialportAssistantController *ui;

    QComboBox *comboBoxSerialports      = nullptr;
    QComboBox *comboBoxBaudrate         = nullptr;
    QComboBox *comboBoxDatabits         = nullptr;
    QComboBox *comboBoxStopbits         = nullptr;
    QComboBox *comboBoxParity           = nullptr;
    QCheckBox *checkBoxCustomBaudrate   = nullptr;
private slots:
    void on_checkBoxCustomBaudrate_clicked();
};

#endif
