/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDMASKERTOOL_H
#define EDMASKERTOOL_H

#include "EDBaseTool.hpp"

class EDMaskerTool : public EDBaseTool
{
    Q_OBJECT
public:
    explicit EDMaskerTool(QObject *parent = Q_NULLPTR);

    Q_INVOKABLE void setMaskCode(qint8 maskCode);

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void inputBytesHandler(const QByteArray &bytes) final;

private:
    quint8 mMask;
};

#endif // EDMASKERTOOL_H
