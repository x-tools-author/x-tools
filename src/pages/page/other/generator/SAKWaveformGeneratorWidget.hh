/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKWAVEFORMGENRATOR_HH
#define SAKWAVEFORMGENRATOR_HH

#include <QWidget>

namespace Ui {
    class SAKWaveformGeneratorWidget;
}

/// @brief 波形发生器，支持正玄波、余弦波、方波、三角波
class SAKWaveformGeneratorWidget:public QWidget
{
    Q_OBJECT
public:
    SAKWaveformGeneratorWidget(QWidget *parent = Q_NULLPTR);
    ~SAKWaveformGeneratorWidget();
private:
    Ui::SAKWaveformGeneratorWidget *ui;
};

#endif
