/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
