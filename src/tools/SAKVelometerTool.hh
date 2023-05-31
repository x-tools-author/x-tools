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
    Q_PROPERTY(QString rxV READ rxV NOTIFY rxVChanged)
    Q_PROPERTY(QString txV READ txV NOTIFY txVChanged)
public:
    explicit SAKVelometerTool(QObject *parent = nullptr);

    virtual void inputBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;

protected:
    void run();

private:
    struct InputBytesContext {
        const QByteArray &bytes;
        const QVariant &context;
    };
    QList<struct InputBytesContext> mInputBytesContextList;
    QMutex mInputBytesContextListMutex;
    QString mRxV;
    QString mTxV;
    QMutex mRxVMutex;
    QMutex mTxVMutex;

private:
    QString rxV();
    QString txV();

signals:
    void rxVChanged(const QString &v);
    void txVChanged(const QString &v);
};

#endif // SAKVELOMETERTOOL_H
