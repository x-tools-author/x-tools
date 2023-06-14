/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKVELOMETERTOOL_HH
#define SAKVELOMETERTOOL_HH

#include <QList>
#include <QMutex>
#include "SAKBaseTool.hh"

class SAKVelometerTool : public SAKBaseTool
{
    Q_OBJECT
    Q_PROPERTY(QString velocity READ velocity NOTIFY velocityChanged)
public:
    explicit SAKVelometerTool(QObject *parent = nullptr);

    virtual void inputBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;

protected:
    void run();

private:
    struct InputBytesContext {
        QByteArray bytes;
        QVariant context;
    };
    QList<struct InputBytesContext> mInputBytesContextList;
    QMutex mInputBytesContextListMutex;
    QString mVelocity;
    QMutex mVelocityMutex;

private:
    QString velocity();

signals:
    void velocityChanged(const QString &velocity);
};

#endif // SAKVELOMETERTOOL_H
