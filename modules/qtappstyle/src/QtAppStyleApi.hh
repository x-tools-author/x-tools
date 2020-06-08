/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef QTAPPSTYLEAPI_HH
#define QTAPPSTYLEAPI_HH

#include <QList>
#include <QObject>
#include <QAction>

class QtAppStyleApi:public QObject
{
    Q_OBJECT
public:
    /**
     * @brief instance  -- 实例化接口，该类必须通过该接口来实例化
     * @return          -- 实例指针（对象指针）
     */
    static QtAppStyleApi *instance();

    /**
     * @brief setStyle 设置样式
     * @param style 样式名称
     */
    void setStyle(QString style);
    /**
     * @brief actions   -- 获取用于软件风格的action列表
     * @return          -- action列表
     */
    QList<QAction *> actions();
signals:
    /// 软件风格发生改变时发射该信号
    void appStyleChanged(QString appStyleName);
private:
    static QtAppStyleApi *_this;
    QList<QAction *> appStyleActions;
private:
    QtAppStyleApi(QObject *parent = Q_NULLPTR);
    void changeAppStyle();
};

#endif
