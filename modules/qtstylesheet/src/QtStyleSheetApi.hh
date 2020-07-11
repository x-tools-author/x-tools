/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef QTSTYLESHEETAPI_HH
#define QTSTYLESHEETAPI_HH

#include <QMap>
#include <QList>
#include <QObject>
#include <QAction>

class QtStyleSheetApi:public QObject
{
    Q_OBJECT
public:
    /**
     * @brief instance  -- 该类必须调用该函数进行实例化
     * @return          -- 实例化指针
     */
    static QtStyleSheetApi* instance();
    /**
     * @brief actions   -- 获取用于改变样式的action列表
     * @return          -- 用于切换软件样式的action列表
     */
    QList<QAction *> actions();

    void setStyleSheet(QString styleSheetName);
signals:
    /// 应用程序样式改变时，发射该信号
    void styleSheetChanged(QString styleSheetName);
private:
    static QtStyleSheetApi *_this;
    QMap<QString, QString> styleSheetMap;
    QList<QAction *> styleSheetActions;
private:
    QtStyleSheetApi(QObject *parent = Q_NULLPTR);
    void changeStyleSheet();
};

#endif
