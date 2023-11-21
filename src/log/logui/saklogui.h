/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKLOGUI_H
#define SAKLOGUI_H

#include <QWidget>

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

#endif // SAKLOGUI_H
