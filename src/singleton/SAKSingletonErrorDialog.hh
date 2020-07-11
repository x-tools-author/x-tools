/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKSINGLETONERRORDIALOG_HH
#define SAKSINGLETONERRORDIALOG_HH

#include <QDialog>

namespace Ui {
    class SAKSingletonErrorDialog;
};

/// @brief 警告弹窗
class SAKSingletonErrorDialog : public QDialog
{
    Q_OBJECT
public:
    SAKSingletonErrorDialog(QWidget *parent = Q_NULLPTR);
    ~SAKSingletonErrorDialog();
private:
    Ui::SAKSingletonErrorDialog *mUi;
};

#endif
