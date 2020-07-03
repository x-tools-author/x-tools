/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKQRCODE_HH
#define SAKQRCODE_HH

#include <QPixmap>
#include <QWidget>

/// @brief 显示一张二维码图片
class SAKQRCode : public QWidget
{
    Q_OBJECT
public:
    SAKQRCode(QWidget *parent = Q_NULLPTR);
    ~SAKQRCode();

    /**
     * @brief updateQRCode 更新二维码
     * @param pixmap 二维码图片
     */
    void updateQRCode(QPixmap pixmap);

    /**
     * @brief qrCode 获取二维码
     * @return 二维码
     */
    QPixmap qrCode();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QPixmap innerPixmap;
};

#endif
