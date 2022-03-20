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
    ParametersContext parametersContext();
    void setUiEnable(bool isUnconnected);
    void initModbusClientParameters(QModbusClient *client);
private:
    ParametersContext mParametersContext;
private:
    Ui::SAKModbusCommonClientSection *ui;
};

#endif // SAKMODBUSCOMMONCLIENTSECTION_HH
