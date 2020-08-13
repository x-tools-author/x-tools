/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMAINWINDOWQRCODVIEW_HH
#define SAKMAINWINDOWQRCODVIEW_HH

#include <QDialog>
#include <QTabWidget>

namespace Ui {
    class SAKMainWindowQrCodeView;
}
class SAKMainWindowQrCodeView : public QDialog
{
    Q_OBJECT
public:
    SAKMainWindowQrCodeView(QWidget *parent = Q_NULLPTR);
    ~SAKMainWindowQrCodeView();
private:
    Ui::SAKMainWindowQrCodeView *ui;
    QTabWidget *tabWidget;
private:
    /// Setup QR code
    void addQRCode(QString name, QString image);
};

#endif
