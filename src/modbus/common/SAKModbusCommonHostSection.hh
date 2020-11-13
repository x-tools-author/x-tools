/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSCOMMONHOSTSECTION_HH
#define SAKMODBUSCOMMONHOSTSECTION_HH

#include <QWidget>
#include <QModbusDevice>

namespace Ui {
    class SAKModbusCommonHostSection;
}
class SAKModbusCommonHostSection : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusCommonHostSection(QWidget *parent = Q_NULLPTR);
    ~SAKModbusCommonHostSection();

    struct ParametersContext{
        QString host;
        quint16 port;
    };
    ParametersContext parametersContext();

    void initModbusDeviceParamerers(QModbusDevice *dev);
private:
    ParametersContext mParametersContext;
private:
    Ui::SAKModbusCommonHostSection *ui;
};

#endif // SAKMODBUSCOMMONHOSTSECTION_HH
