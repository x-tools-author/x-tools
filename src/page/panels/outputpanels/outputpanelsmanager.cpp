/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "outputpanelsmanager.h"

#include "page/panels/common/luapanel.h"
#include "page/panels/outputpanels/search/searchpanel.h"
#include "page/panels/outputpanels/xymodem/xymodemreceiver.h"

OutputPanelsManager::OutputPanelsManager(QWidget *parent)
    : PanelManager(parent)
{
    addPanel<SearchPanel>(tr("Search"), ":/res/icons/search.svg");
    addPanel<LuaPanel>(tr("Lua Script"), ":/res/icons/lua.svg");
#if 0
    addPanel<XYModemReceiver>(tr("XY-Modem Receiver"), ":/res/icons/xy.svg");
#endif
}

OutputPanelsManager::~OutputPanelsManager()
{
    // Destructor implementation can be added here if needed
}