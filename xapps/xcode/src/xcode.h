/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMAinWindow>

namespace Ui {
class xCode;
}

class xCode : public QMainWindow
{
    Q_OBJECT
public:
    explicit xCode(QWidget *parent = nullptr);
    ~xCode() override;

private:
    Ui::xCode *ui;

private:
    void onExportButtonClicked();
    void onRefreshButtonClicked();
};