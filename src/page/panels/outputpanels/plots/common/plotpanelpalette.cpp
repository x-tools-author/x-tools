/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "plotpanelpalette.h"

PlotPanelPalette::PlotPanelPalette(QObject *parent)
    : QObject(parent)
{}

PlotPanelPalette::~PlotPanelPalette() {}

PlotPanelPalette &PlotPanelPalette::singleton()
{
    static PlotPanelPalette instance;
    return instance;
}

void PlotPanelPalette::notifyAll()
{
    emit paletteChanged();
}