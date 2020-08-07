/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
