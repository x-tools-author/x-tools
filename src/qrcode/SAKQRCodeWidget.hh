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
#ifndef SAKQRCODEWIDGET_HH
#define SAKQRCODEWIDGET_HH

#include <QWidget>

/// @brief 显示一张二维码图片
class SAKQRCodeWidget : public QWidget
{
    Q_OBJECT
public:
    SAKQRCodeWidget(QSize size, QString image, QWidget *parent = Q_NULLPTR);
    ~SAKQRCodeWidget();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QSize size;
    QString image;
};

#endif
