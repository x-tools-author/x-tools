/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTOOLQRCODECREATOR_HH
#define SAKTOOLQRCODECREATOR_HH

#include <QPixmap>
#include <QWidget>
#include <QPushButton>
#include <QPlainTextEdit>

namespace Ui {
    class SAKToolQRCodeCreator;
}

class SAKQRCode;
/// @brief 二维码生成工具
class SAKToolQRCodeCreator:public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKToolQRCodeCreator(QWidget *parent = Q_NULLPTR);
    ~SAKToolQRCodeCreator();

    /**
     * @brief encodeString 将字符串编码成二维码图片
     * @param text 输入信息
     * @param width 二维码宽度
     * @return 二维码图片
     */
    QPixmap encodeString(const QString &text, int width = 100);
private:
    Ui::SAKToolQRCodeCreator *ui;
    SAKQRCode *qrCodeWidget;
    QPlainTextEdit *plainTextEdit;
    QPushButton *savePushButton;
    QPushButton *createPushButton;
private slots:
    void on_savePushButton_clicked();
    void on_createPushButton_clicked();
};

#endif
