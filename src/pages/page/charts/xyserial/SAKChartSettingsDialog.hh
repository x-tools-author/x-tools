/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKCHARTSETTINGSDIALOG_HH
#define SAKCHARTSETTINGSDIALOG_HH

#include <QMap>
#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

namespace Ui {
    class SAKChartSettingsDialog;
};

/// @brief 图标参数设置类
class SAKChartSettingsDialog:public QDialog
{
    Q_OBJECT
public:
    SAKChartSettingsDialog(QWidget *parent = Q_NULLPTR);
    ~SAKChartSettingsDialog();
private:
    Ui::SAKChartSettingsDialog *mUi;
};

#endif
