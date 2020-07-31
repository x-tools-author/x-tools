/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
