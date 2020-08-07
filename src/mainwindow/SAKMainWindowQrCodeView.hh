/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
