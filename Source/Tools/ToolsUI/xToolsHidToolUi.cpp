/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsHidToolUi.h"
#include "ui_xToolsHidToolUi.h"

xToolsHidToolUi::xToolsHidToolUi(QWidget *parent)
    : xToolsCommunicationToolUi{parent}
    , ui{new Ui::xToolsHidToolUi}
{}

xToolsHidToolUi::~xToolsHidToolUi() {}