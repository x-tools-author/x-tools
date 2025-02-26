/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "iofactory.h"

#include <QCoreApplication>

#ifdef X_TOOLS_ENABLE_BLUETOOTH
#include "device/blecentral.h"
#endif
#ifdef X_TOOLS_ENABLE_SERIAL_PORT
#include "device/serialport.h"
#endif
#include "device/tcpclient.h"
#include "device/tcpserver.h"
#include "device/udpclient.h"
#include "device/udpserver.h"
#ifdef X_TOOLS_ENABLE_WEB_SOCKET
#include "device/websocketclient.h"
#include "device/websocketserver.h"
#endif
#include "common/xtools.h"
#include "device/chartstest.h"

namespace xTools {

IOFactory::IOFactory() {}

IOFactory &IOFactory::singleton()
{
    static IOFactory instance;
    return instance;
}

} // namespace xTools
