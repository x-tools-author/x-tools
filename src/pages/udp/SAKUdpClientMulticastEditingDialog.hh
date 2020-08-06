/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPCLIENTMUTICASTEDITINGDIALOG_HH
#define SAKUDPCLIENTMUTICASTEDITINGDIALOG_HH

#include <QDialog>
#include <QLineEdit>

namespace Ui {
    class SAKUdpClientMulticastEditingDialog;
}

/// @brief 组播编辑弹窗
class SAKUdpClientMulticastEditingDialog : public QDialog
{
    Q_OBJECT
public:
    SAKUdpClientMulticastEditingDialog(QWidget *parent = Q_NULLPTR);
    ~SAKUdpClientMulticastEditingDialog();

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
    Ui::SAKUdpClientMulticastEditingDialog *ui;
};

#endif
