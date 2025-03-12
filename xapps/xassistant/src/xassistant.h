/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "mainwindow.h"

class Page;
class xAssistant : public MainWindow
{
    Q_OBJECT
public:
    xAssistant(QWidget* parent = Q_NULLPTR);
    ~xAssistant() override;

protected:
    QUrl storeUrl() const override;
};
