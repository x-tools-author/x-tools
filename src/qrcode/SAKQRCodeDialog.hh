/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKQRCODEDIALOG_HH
#define SAKQRCODEDIALOG_HH

#include <QDialog>
#include <QTabWidget>

namespace Ui {
    class SAKQRCodeDialog;
}
class SAKQRCodeDialog : public QDialog
{
    Q_OBJECT
public:
    SAKQRCodeDialog(QWidget *parent = Q_NULLPTR);
    ~SAKQRCodeDialog();
private:
    Ui::SAKQRCodeDialog *ui;
    QTabWidget *tabWidget;
private:
    /// @brief 添加二维码
    void addQRCode(QString name, QString image);
};

#endif
