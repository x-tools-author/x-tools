/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTOOLBASE
#define SAKTOOLBASE

#include <QWidget>

/// @brief 瑞士军刀实用工具基类
class SAKToolBase : public QWidget
{
    Q_OBJECT
public:
    SAKToolBase(QWidget *parent = Q_NULLPTR);
    ~SAKToolBase();
};

#endif
