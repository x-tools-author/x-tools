/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKQRCODE_HH
#define SAKQRCODE_HH

#include <QWidget>

/// @brief 显示一张二维码图片
class SAKQRCode : public QWidget
{
    Q_OBJECT
public:
    SAKQRCode(QSize size, QString image, QWidget *parent = Q_NULLPTR);
    ~SAKQRCode();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QSize size;
    QString image;
};

#endif
