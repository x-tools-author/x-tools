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
#include "SAKToolCRCCalculator.hh"
#ifdef SAK_IMPORT_MODULE_QRCODE
#include "SAKToolQRCodeCreator.hh"
#endif
#include "SAKToolFloatAssistant.hh"
#include "SAKToolStringAssistant.hh"

SAKToolsFactory::SAKToolsFactory(QObject *parent)
{
    setParent(parent ? parent : qApp);

#ifdef SAK_IMPORT_MODULE_FILECHECKER
    mToolMetaObjCtxVector.append(
                SAKToolMetaObjCtx{
                    SAKToolFileChecker::staticMetaObject,
                    tr("File Assistant")});
#endif
    mToolMetaObjCtxVector.append(
                SAKToolMetaObjCtx{
                    SAKToolCRCCalculator::staticMetaObject,
                    tr("CRC Assistant")});
    mToolMetaObjCtxVector.append(
                SAKToolMetaObjCtx{
                    SAKToolFloatAssistant::staticMetaObject,
                    tr("Float Assistant")});
    mToolMetaObjCtxVector.append(
                SAKToolMetaObjCtx{
                    SAKToolStringAssistant::staticMetaObject,
                    tr("String Assistant")});
}

QVector<SAKToolsFactory::SAKToolMetaObjCtx>
SAKToolsFactory::supportedToolsContext()
{
    return mToolMetaObjCtxVector;
}
