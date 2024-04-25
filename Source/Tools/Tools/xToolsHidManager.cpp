/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsHidManager.h"

#include <hidapi.h>

#include <QDebug>

xToolsHidManager::xToolsHidManager(QObject *parent)
    : QObject(parent)
{
    const struct hid_api_version *ctx = hid_version();
    qInfo() << "The version of hid api is:"
            << QString("%1.%2.%3").arg(ctx->major).arg(ctx->minor).arg(ctx->patch);

    if (hid_init() != 0) {
        qWarning() << qPrintable(QString("The function(%1) return error...").arg("hid_init()"));
        return;
    }

    struct hid_device_info *info = hid_enumerate(0x0, 0x0);
    for (; info; info = info->next) {
        qInfo() << "The hid device info is:"
                << QString("path:%1, vendor_id:%2, product_id:%3, serial_number:%4, "
                           "release_number:%5, manufacturer_string:%6, product_string:%7, "
                           "usage_page:%8, usage:%9, interface_number:%10, bus_type:%11")
                       .arg(info->path)
                       .arg(info->vendor_id)
                       .arg(info->product_id)
                       .arg(QString::fromStdWString(info->serial_number))
                       .arg(info->release_number)
                       .arg(info->manufacturer_string)
                       .arg(QString::fromStdWString(info->product_string))
                       .arg(info->usage_page)
                       .arg(info->usage)
                       .arg(info->interface_number)
                       .arg(info->bus_type);
    }
}

xToolsHidManager::~xToolsHidManager()
{
    if (hid_exit() != 0) {
        qWarning() << qPrintable(QString("The function(%1) return error...").arg("hid_exit()"));
    }
}

xToolsHidManager &xToolsHidManager::singleton()
{
    static xToolsHidManager instance;
    return instance;
}