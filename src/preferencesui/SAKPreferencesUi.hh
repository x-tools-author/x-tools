/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKPREFERENCESUI_HH
#define SAKPREFERENCESUI_HH

#include <QWidget>

namespace Ui {
class SAKPreferencesUi;
}

class SAKPreferencesUi : public QWidget
{
    Q_OBJECT

public:
    explicit SAKPreferencesUi(QWidget *parent = nullptr);
    ~SAKPreferencesUi();

private:
    Ui::SAKPreferencesUi *ui;
};

#endif // SAKPREFERENCESUI_HH
