/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKToolBoxUiParameters.hh"
#include "ui_SAKToolBoxUiParameters.h"

SAKToolBoxUiParameters::SAKToolBoxUiParameters(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SAKToolBoxUiParameters)
{
    ui->setupUi(this);
}

SAKToolBoxUiParameters::~SAKToolBoxUiParameters()
{
    delete ui;
}

void SAKToolBoxUiParameters::showDialog(int tabIndex)
{
    if (tabIndex >= 0 && tabIndex < ui->tabWidget->count()) {
        ui->tabWidget->setCurrentIndex(tabIndex);
    } else {
        qCWarning(mLoggingCategory) << "Invalid tab index:" << tabIndex;
    }

    show();
}

SAKToolBoxUiParameters::ParameterContext parameterContext()
{
    ParameterContext ctx;
    return ctx;
}
