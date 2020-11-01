/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSCOMMONDEVICECONTROLLER_HH
#define SAKMODBUSCOMMONDEVICECONTROLLER_HH

#include <QWidget>
#include <QHBoxLayout>

class SAKModbusCommonController : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusCommonController(QWidget *parent = nullptr);

    void appendSection(QWidget *section);
private:
    QVBoxLayout *mSectionLayout;
};

#endif // SAKMODBUSCOMMONDEVICECONTROLLER_HH
