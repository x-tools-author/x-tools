/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "inputpanelsmanager.h"

#include "common/iconengine.h"
#include "page/panels/common/luapanel.h"
#include "page/panels/inputpanels/framserialnumber/frameserialnumber.h"
#include "page/panels/inputpanels/xymodem/xymodemsender.h"

InputPanelsManager::InputPanelsManager(QWidget *parent)
    : PanelManager(parent)
{
    addPanel<LuaPanel>(tr("Lua Script"), xIcon(":/res/icons/lua.svg"));
#if 0
    addPanel<FrameSerialNumber>(tr("Frame Serial Number"), ":/res/icons/counter_1.svg");
    addPanel<XYModemSender>(tr("XYModem Sender"), ":/res/icons/xy.svg");
#endif
}

InputPanelsManager::~InputPanelsManager() {}