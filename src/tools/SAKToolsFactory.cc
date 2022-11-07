/******************************************************************************
 * Copyright 2022 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#include <QCoreApplication>
#include "SAKToolsFactory.hh"

#ifdef SAK_IMPORT_MODULE_FILECHECKER
#include "SAKToolFileChecker.hh"
#endif
#ifdef SAK_IMPORT_MODULE_CRCCALCULATOR
#include "SAKToolCRCCalculator.hh"
#endif
#ifdef SAK_IMPORT_MODULE_QRCODE
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

SAKToolsFactory::SAKToolsFactory(QObject *parent)
{
#ifdef SAK_IMPORT_MODULE_CRCCALCULATOR
    mToolMetaObjCtxVector.append(SAKToolMetaObjCtx{
                    SAKToolCRCCalculator::staticMetaObject,
                tr("CRC Assistant")});
#endif
#ifdef SAK_IMPORT_MODULE_FILECHECKER
    mToolMetaObjCtxVector.append(
                SAKToolMetaObjCtx{
                    SAKToolFileChecker::staticMetaObject,
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
}

QVector<SAKToolsFactory::SAKToolMetaObjCtx>
SAKToolsFactory::supportedToolsContext()
{
    return mToolMetaObjCtxVector;
}
