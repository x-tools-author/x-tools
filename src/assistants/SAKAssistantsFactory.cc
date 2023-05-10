/******************************************************************************
 * Copyright 2022-2023 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#include <QCoreApplication>
#include "SAKAssistantsFactory.hh"

#ifdef SAK_IMPORT_MODULE_FILECHECKASSISTANT
#include "SAKToolFileCheckAssistant.hh"
#endif
#ifdef SAK_IMPORT_MODULE_CRCASSISTANT
#include "SAKToolCRCAssistant.hh"
#endif
#ifdef SAK_IMPORT_MODULE_QRCODEASSISTANT
#include "SAKToolQRCodeCreator.hh"
#endif
#ifdef SAK_IMPORT_MODULE_FLOATASSISTANT
#include "SAKToolFloatAssistant.hh"
#endif
#ifdef SAK_IMPORT_MODULE_STRINGASSISTANT
#include "SAKToolStringAssistant.hh"
#endif
#ifdef SAK_IMPORT_MODULE_ATASSISTANT
#include "SAKAtAssistant.hh"
#endif
#ifdef SAK_IMPORT_MODULE_ASCIIASSISTANT
#include "SAKToolAsciiAssistant.hh"
#endif
#ifdef SAK_IMPORT_MODULE_BROADCASTASSISTANT
#include "SAKToolBroadcastAssistant.hh"
#endif

SAKAssistantsFactory::SAKAssistantsFactory(QObject *parent)
{
    Q_UNUSED(parent);
#ifdef SAK_IMPORT_MODULE_CRCASSISTANT
    mToolMetaObjCtxVector.append(SAKToolMetaObjCtx{
                    SAKToolCRCAssistant::staticMetaObject,
                tr("CRC Assistant")});
#endif
#ifdef SAK_IMPORT_MODULE_FILECHECKASSISTANT
    mToolMetaObjCtxVector.append(
                SAKToolMetaObjCtx{
                    SAKToolFileCheckAssistant::staticMetaObject,
                    tr("File Assistant")});
#endif
#ifdef SAK_IMPORT_MODULE_FLOATASSISTANT
    mToolMetaObjCtxVector.append(
                SAKToolMetaObjCtx{
                    SAKToolFloatAssistant::staticMetaObject,
                    tr("Float Assistant")});
#endif
#ifdef SAK_IMPORT_MODULE_STRINGASSISTANT
    mToolMetaObjCtxVector.append(
                SAKToolMetaObjCtx{
                    SAKToolStringAssistant::staticMetaObject,
                    tr("String Assistant")});
#endif
#ifdef SAK_IMPORT_MODULE_ATASSISTANT
    mToolMetaObjCtxVector.append(
                SAKToolMetaObjCtx{
                    SAKAtAssistant::staticMetaObject,
                    tr("AT Assistant")});
#endif
#ifdef SAK_IMPORT_MODULE_ASCIIASSISTANT
    mToolMetaObjCtxVector.append(
                SAKToolMetaObjCtx{
                    SAKToolAsciiAssistant::staticMetaObject,
                    tr("ASCII Assistant")});
#endif
#ifdef SAK_IMPORT_MODULE_BROADCASTASSISTANT
    mToolMetaObjCtxVector.append(
                SAKToolMetaObjCtx{
                    SAKToolBroadcastAssistant::staticMetaObject,
                    tr("Broadcast Assistant")});
#endif
}

QVector<SAKAssistantsFactory::SAKToolMetaObjCtx>
SAKAssistantsFactory::supportedToolsContext()
{
    return mToolMetaObjCtxVector;
}
