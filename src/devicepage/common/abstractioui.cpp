/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "abstractioui.h"

#include "device/communication.h"

AbstractIOUi::AbstractIOUi(QWidget *parent)
    : QWidget(parent)
{}

AbstractIOUi::~AbstractIOUi() {}

void AbstractIOUi::setupIO(AbstractIO *io)
{
    if (io) {
        io->load(save());
    }
}

void AbstractIOUi::setUiEnabled(bool enabled)
{
    setEnabled(enabled);
}
