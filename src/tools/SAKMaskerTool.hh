/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKMASKERTOOL_HH
#define SAKMASKERTOOL_HH

#include <atomic>
#include "SAKBaseTool.hh"

class SAKMaskerTool : public SAKBaseTool
{
    Q_OBJECT
public:
    explicit SAKMaskerTool(QObject *parent = Q_NULLPTR);

    Q_INVOKABLE void setMaskCode(qint8 maskCode);

    virtual void inputBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;

private:
    std::atomic<quint8> mMask;
};

#endif // SAKMASKERTOOL_H
