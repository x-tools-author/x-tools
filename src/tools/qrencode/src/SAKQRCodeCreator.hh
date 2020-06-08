/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKQRCODECREATOR_HH
#define SAKQRCODECREATOR_HH

#include <QWidget>
namespace Ui {
    class SAKQRCodeCreator;
}
class SAKQRCodeCreator:public QWidget
{
    Q_OBJECT
public:
    SAKQRCodeCreator(QWidget *parent = Q_NULLPTR);
    ~SAKQRCodeCreator();
private:
    Ui::SAKQRCodeCreator *ui;
};

#endif
