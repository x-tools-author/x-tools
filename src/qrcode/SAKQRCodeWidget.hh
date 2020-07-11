/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
