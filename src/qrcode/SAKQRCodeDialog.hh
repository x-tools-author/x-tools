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
