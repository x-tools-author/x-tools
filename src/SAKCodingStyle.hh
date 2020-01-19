/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is an open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKCODINGSTYLE_HH
#define SAKCODINGSTYLE_HH

#include <QObject>

/**
 * @brief 每个类都需要有相应的说明，说明包括功能说明，注意事项等。
 * 例如：这个是工程编码风格示例类
 */
class SAKCodingStyle:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString stringProperty1 READ stringProperty1 WRITE setStringProperty1 NOTIFY stringProperty1Changed)
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
    /// 这是对数据成员obj2的说明
    QObject *obj2;
    QObject *obj3;
private:
    QString _stringProperty1;
    QString stringProperty1();
    void setStringProperty1(QString str);
signals:
    void stringProperty1Changed();
private:
    void helloWorld();
};

#endif
