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
#include "page/panels/outputpanels/datarecords/datarecordsview.h"
#include "page/panels/outputpanels/search/searchpanel.h"
#include "page/panels/outputpanels/xymodem/xymodemreceiver.h"
#include "utilities/iconengine.h"

#ifdef X_ENABLE_CHARTS
#include "charts/bar/barpanel.h"
#include "charts/line/linepanel.h"
#endif

OutputPanelsManager::OutputPanelsManager(QWidget *parent)
    : PanelManager(parent)
{
    addPanel<DataRecordsView>(tr("Data Records"), xIcon(":/res/icons/list.svg"));
    addPanel<SearchPanel>(tr("Search"), xIcon(":/res/icons/search.svg"));
    addPanel<LuaPanel>(tr("Lua Script"), xIcon(":/res/icons/lua.svg"));
#ifdef X_ENABLE_CHARTS
    addPanel<LinePanel>(tr("Line Chart"), xIcon(":/res/icons/line_series.svg"));
    addPanel<BarPanel>(tr("Bar Chart"), xIcon(":/res/icons/bar.svg"));
#endif
#if 0
    addPanel<XYModemReceiver>(tr("XY-Modem Receiver"), xIcon(":/res/icons/xy.svg"));
#endif
}

OutputPanelsManager::~OutputPanelsManager()
{
    // Destructor implementation can be added here if needed
}