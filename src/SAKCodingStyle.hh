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
 */
#ifndef SAKCODINGSTYLE_HH
#define SAKCODINGSTYLE_HH

#include <QObject>

class SAKCodingStyle:public QObject
{
    Q_OBJECT
public:
    /**
     * @brief SAKCodingStyle 函数说明，公有函数原则上都需要添加
     * @param parent 参数parent说明
     */
    SAKCodingStyle(QObject* parent = nullptr);

private:
    /*
     * 对以下代码段的说明，对以下代码段的说明，对以下代码段的说明
     */
    QString string1;
    QString string2;

    /// 这是对数据成员obj1的说明
    QObject *obj1;
    /// 这是对数据成员obj1的说明
    QObject *obj2;
    QObject *obj3;

private:
    void helloWorld();
};

#endif
