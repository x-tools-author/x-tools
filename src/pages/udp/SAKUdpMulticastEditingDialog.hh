/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKUDPMUTICASTEDITINGDIALOG_HH
#define SAKUDPMUTICASTEDITINGDIALOG_HH

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class SAKUdpMulticastEditingDialog;
}

/// @brief 组播编辑弹窗
class SAKUdpMulticastEditingDialog : public QDialog
{
    Q_OBJECT
public:
    SAKUdpMulticastEditingDialog(QWidget *parent = Q_NULLPTR);
    ~SAKUdpMulticastEditingDialog();

    /**
     * @brief address 获取组播地址
     * @return 组播地址
     */
    QString address();

    /**
     * @brief port 获取组播端口
     * @return 组播端口
     */
    quint16 port();
private:
    Ui::SAKUdpMulticastEditingDialog *ui;
};

#endif
