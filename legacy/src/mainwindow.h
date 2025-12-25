#pragma once

#include "common/xui.h"

class MainWindow : public xUi
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
};