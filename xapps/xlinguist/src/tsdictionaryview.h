/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

namespace Ui {
class TsDictionaryView;
}

class TsDictionaryView : public QWidget
{
    Q_OBJECT
public:
    explicit TsDictionaryView(QWidget *parent = nullptr);
    ~TsDictionaryView() override;

private:
    Ui::TsDictionaryView *ui;
};