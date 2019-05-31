/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
 */
#include "SAKGlobal.hh"

#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QDebug>

SAKGlobal::SAKGlobal(QObject* parent)
    :QObject (parent)
{

}

QString SAKGlobal::logFile()
{
    QString fileName = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    QDir dir;
    if (!dir.exists(fileName)){
        SAKGlobal::mkMutiDir(fileName);
    }    

    fileName.append("/");
    fileName.append("QtSwissArmyKnife.txt");

    return fileName;
}

QString SAKGlobal::mkMutiDir(const QString path){

    QDir dir(path);
    if (dir.exists(path)){
        return path;
    }

    QString parentDir = mkMutiDir(path.mid(0,path.lastIndexOf('/')));
    QString dirname = path.mid(path.lastIndexOf('/') + 1);
    QDir parentPath(parentDir);

    if ( !dirname.isEmpty() ){
        parentPath.mkpath(dirname);
    }

    return parentDir + "/" + dirname;
}

QString SAKGlobal::getIODeviceTypeName(int type)
{
    QString name = "none";
    switch (type) {
    case SAKEnumIODeviceTypeUDP:
        name = tr("UDP助手");
        break;
    case SAKEnumIODeviceTypeTCPClient:
        name = tr("TCP客户端");
        break;
    case SAKEnumIODeviceTypeTCPServer:
        name = tr("TCP服务器");
        break;
    case SAKEnumIODeviceTypeSerialport:
        name = tr("串口助手");
        break;
    default:
        break;
    }

    return name;
}
