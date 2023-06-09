/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKLOGUI_HH
#define SAKLOGUI_HH

#include <QWidget>

#include "SAKLog.hh"

namespace Ui {
class SAKLogUi;
}

class SAKLogUi : public QWidget
{
    Q_OBJECT
public:
    explicit SAKLogUi(QWidget *parent = nullptr);
    ~SAKLogUi();

private:
    Ui::SAKLogUi *ui;

private slots:
    void onPushButtonClearClicked();
    void onPushButtonDirectoryClicked();
    void onComboBoxLevelCurrentIndexChanged();
    void onComboBoxLifeCycleCurrentIndexChanged();
    void onCheckBoxPauseClicked();
};

#endif // SAKLOGUI_HH
