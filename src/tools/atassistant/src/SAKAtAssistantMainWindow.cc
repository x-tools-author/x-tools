/******************************************************************************
 * Copyright 2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKAtAssistantMainWindow.hh"
#include "atassistant/SAKAtAssistant.hh"

SAKAtAssistantMainWindow::SAKAtAssistantMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    SAKAtAssistant* atAssistant = new SAKAtAssistant;
    setCentralWidget(atAssistant);
}

SAKAtAssistantMainWindow::~SAKAtAssistantMainWindow()
{

}