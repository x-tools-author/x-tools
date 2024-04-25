/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QObject>

class xToolsHidManager : public QObject
{
    Q_OBJECT
private:
    explicit xToolsHidManager(QObject *parent = nullptr);

public:
    ~xToolsHidManager() override;
    xToolsHidManager(const xToolsHidManager &) = delete;
    xToolsHidManager &operator=(const xToolsHidManager &) = delete;

    static xToolsHidManager &singleton();
};