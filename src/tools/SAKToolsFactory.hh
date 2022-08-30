/******************************************************************************
 * Copyright 2022 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTOOLSFACTORY_HH
#define SAKTOOLSFACTORY_HH

#include <QObject>
#include "base/SAKSigleton.hh"

#define sakToolsFactory SAKToolsFactory::instance()

class SAKToolsFactory : public QObject, public SAKSigleton<SAKToolsFactory>
{
    Q_OBJECT
    friend SAKSigleton<SAKToolsFactory>;
private:
    SAKToolsFactory(QObject *parent = Q_NULLPTR);
public:
    struct SAKToolMetaObjCtx {
        QMetaObject metaObject;
        QString title;
    };
private:
    QVector<SAKToolMetaObjCtx> mToolMetaObjCtxVector;
public:
    QVector<SAKToolMetaObjCtx> supportedToolsContext();
};

#endif // SAKTOOLSFACTORY_HH
