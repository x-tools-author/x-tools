/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
