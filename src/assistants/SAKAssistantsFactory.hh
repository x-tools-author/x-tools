/******************************************************************************
 * Copyright 2022-2023 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#ifndef SAKASSISTANTSFACTORY_HH
#define SAKASSISTANTSFACTORY_HH

#include <QObject>
#include "base/SAKSigleton.hh"

#define sakToolsFactory SAKAssistantsFactory::instance()

class SAKAssistantsFactory : public QObject, public SAKSigleton<SAKAssistantsFactory>
{
    Q_OBJECT
    friend SAKSigleton<SAKAssistantsFactory>;
private:
    SAKAssistantsFactory(QObject *parent = Q_NULLPTR);
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

#endif // SAKASSISTANTSFACTORY_H
