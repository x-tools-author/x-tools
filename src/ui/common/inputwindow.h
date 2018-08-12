/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QDebug>

namespace Ui {
class InputWindow;
}

class InputWindow : public QWidget
{
    Q_OBJECT
public:
    InputWindow(QWidget *parent = Q_NULLPTR);
    ~InputWindow();
    /// 输入框数据(会根据输入设置选项来提取数据)
    QByteArray Data();
private:
    /// ui界面
    Ui::InputWindow *ui;
    /// ------------------------------------------
private slots:
    /// 输入格式控制
    void TextFormatControl();
    /// 重置计数
    void ResetCount();
private slots:
    /// 读入文本数据
    void OpenFile();
};

#endif
