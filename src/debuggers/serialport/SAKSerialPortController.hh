/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKSERIALPORTCONTROLLER_HH
#define SAKSERIALPORTCONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QSerialPort>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKSerialPortController;
}

class SAKDebugger;
class SAKSerialPortController : public SAKDebugPageController
{
    Q_OBJECT
public:
    struct SerialPortParameters {
        enum QSerialPort::DataBits dataBits;
        enum QSerialPort::StopBits stopBits;
        enum QSerialPort::Parity parity;
        enum QSerialPort::FlowControl flowControl;
        QString name;
        quint32 baudRate;
    };

    SAKSerialPortController(SAKDebugger *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKSerialPortController();

    QVariant parameters() final;
    void setUiEnable(bool opened) final;
    void refreshDevice() final;
private:
    QMutex mParametersMutex;
    SerialPortParameters mParameters;
    SAKDebugger *mDebugPage;
private:
    void setBaudRate(quint32 bd);
private:
    Ui::SAKSerialPortController *ui;
    QComboBox *serialportsComboBox;
    QComboBox *baudrateComboBox;
    QComboBox *databitsComboBox;
    QComboBox *stopbitsComboBox;
    QComboBox *parityComboBox;
    QComboBox *flowControlComboBox;
    QCheckBox *customBaudrateCheckBox;
private slots:
    void on_customBaudrateCheckBox_clicked();
    void on_serialportsComboBox_currentTextChanged(const QString &arg1);
    void on_baudrateComboBox_currentIndexChanged(int index);
    void on_databitsComboBox_currentIndexChanged(int index);
    void on_stopbitsComboBox_currentIndexChanged(int index);
    void on_parityComboBox_currentIndexChanged(int index);
    void on_flowControlComboBox_currentIndexChanged(int index);
    void on_baudrateComboBox_editTextChanged(const QString &arg1);
};

Q_DECLARE_METATYPE(SAKSerialPortController::SerialPortParameters);
#endif
