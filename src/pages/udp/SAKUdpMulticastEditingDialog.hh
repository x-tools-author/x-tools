/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
