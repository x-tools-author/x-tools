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
#ifndef SAKCOMMONAPI_HH
#define SAKCOMMONAPI_HH

#include <QObject>
#include <QComboBox>

class SAKCommonApi:public QObject
{
    Q_OBJECT
public:
    static SAKCommonApi *instance();
private:
    SAKCommonApi(QObject *parent = nullptr);
    static SAKCommonApi *_this;

public:
    /*
     * 串口相关参数显示初始化
     */
    void initComComboBox(QComboBox *comboBox);
    void initBaudRateComboBox(QComboBox *comboBox);
    void initDataBitsComboBox(QComboBox *comboBox);
    void initStopBitsComboBox(QComboBox *comboBox);
    void initParityComboBox(QComboBox *comboBox);
};

#endif
