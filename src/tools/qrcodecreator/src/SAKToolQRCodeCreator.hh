/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKTOOLQRCODECREATOR_HH
#define SAKTOOLQRCODECREATOR_HH

#include <QPixmap>
#include <QWidget>
#include <QPushButton>
#include <QPlainTextEdit>

#include "SAKToolBase.hh"

namespace Ui {
    class SAKToolQRCodeCreator;
}

class SAKQRCode;
/// @brief 二维码生成工具
class SAKToolQRCodeCreator:public SAKToolBase
{
    Q_OBJECT
public:
    SAKToolQRCodeCreator(QWidget *parent = Q_NULLPTR);
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
