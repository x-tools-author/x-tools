/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKMASKERTOOL_H
#define SAKMASKERTOOL_H

#include <atomic>
#include <QMutex>

#include "sakbasetool.h"

class SAKMaskerTool : public SAKBaseTool
{
    Q_OBJECT
public:
    explicit SAKMaskerTool(QObject *parent = Q_NULLPTR);
    ~SAKMaskerTool();

    Q_INVOKABLE void setMaskCode(qint8 maskCode);

    virtual void inputBytes(const QByteArray &bytes, const QVariant &context = QJsonObject()) final;

protected:
    void run();

private:
    struct InputParameters
    {
        QByteArray bytes;
        QVariant context;
    };

private:
    std::atomic<quint8> mMask;
    QList<InputParameters> mInputParametersList;
    QMutex mInputParametersListMutex;
};

#endif // SAKMASKERTOOL_H
