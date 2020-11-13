/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSCOMMONCLIENTSECTION_HH
#define SAKMODBUSCOMMONCLIENTSECTION_HH

#include <QDebug>
#include <QWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QModbusClient>
#include <QModbusDataUnit>

namespace Ui {
    class SAKModbusCommonClientSection;
}
class SAKModbusCommonClientSection : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusCommonClientSection(QWidget *parent = Q_NULLPTR);
    ~SAKModbusCommonClientSection();

    struct ParametersContext{
        int timeout;
        int numberOfRetries;
    };

    QComboBox *functionCodeComboBox(){return mFunctionCodeSpinBox;}
    QSpinBox *startAddressLineEdit(){return mStartAddressSpinBox;}
    QSpinBox *registerNumberLineEdit(){return mRegisterNumberLineEdit;}
    QSpinBox *clientAddressSpinBox(){return mClientAddressSpinBox;}
    QTableWidget *readWriteValuesTableWidget(){return mReadWriteValuesTableWidget;}
    ParametersContext parametersContext();
    void setUiEnable(bool isUnconnected);
    int slaveAddress();
    void updateTableWidget(QModbusDataUnit mdu);
    void initModbusClientParameters(QModbusClient *client);
private:
    ParametersContext mParametersContext;
private:
    void updateTableWidget();
signals:
    void invokeSendReadRequest(QModbusDataUnit mdu);
    void invokrSendWriteRequest(QModbusDataUnit mdu);
private:
    Ui::SAKModbusCommonClientSection *ui;
    QComboBox *mFunctionCodeSpinBox;
    QSpinBox *mStartAddressSpinBox;
    QSpinBox *mRegisterNumberLineEdit;
    QSpinBox *mClientAddressSpinBox;
    QTableWidget *mReadWriteValuesTableWidget;
private slots:
    void on_functionCodeComboBox_currentIndexChanged(int index);
    void on_clientAddressSpinBox_valueChanged(int arg1);
    void on_startAddressSpinBox_valueChanged(int arg1);
    void on_registerNumberSpinBox_valueChanged(int arg1);
    void on_sendReadRequestPushButton_clicked();
    void on_sendWriteRequestPushButton_clicked();
};

#endif // SAKMODBUSCOMMONCLIENTSECTION_HH
