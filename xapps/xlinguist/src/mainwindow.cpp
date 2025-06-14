/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : xUi(parent)
{
    setWindowTitle(tr("xLinguist"));
    setMinimumSize(800, 600);
    setWindowIcon(QIcon(":/res/icons/logo.svg"));
}

MainWindow::~MainWindow() {}