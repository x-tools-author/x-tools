/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
 */
#ifndef SAKGLOBAL_HPP
#define SAKGLOBAL_HPP

#include <QObject>

class SAKGlobal:public QObject
{
    Q_OBJECT
public:
    /**
     * @brief SAKGlobal -- 构造函数
     * @param parent    -- 父控件
     */
    SAKGlobal(QObject* parent = nullptr);

    /**
     * @brief logFile   -- 获取日志文件（全路径）
     * @return          -- 日志文件全路径
     */
    static QString logFile();

    /**
     * @brief mkMutiDir -- 递归创建指定的目录路径
     * @param path      -- 目录路径
     * @return          -- 已创建的路径
     */
    static QString mkMutiDir(const QString path);
};

#endif
